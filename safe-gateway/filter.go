package main

import (
	"net"
	"strings"
	"sync"
	"time"
)

type FilterInfo struct {
	//請求次數
	Num int
	//最後請求時間
	Last time.Time
}

func (f *FilterInfo) SafeConnect() bool {
	now := time.Now()
	cnf := getConfigure()

	//時間安全
	if now.After(f.Last.Add(cnf.Safe.Duration)) {
		f.Num = 1
		f.Last = now
		return true
	}

	//連接次數 安全
	if f.Num < cnf.Safe.Num {
		f.Num++
		f.Last = now
		return true
	}
	return false
}
func newFilterInfo() *FilterInfo {
	now := time.Now()
	return &FilterInfo{Num: 1,
		Last: now,
	}
}

type Filter struct {
	mutex sync.Mutex

	//連接記錄
	nodes map[string]*FilterInfo

	//黑名單
	black map[string]bool
}

var g_filter Filter

//初始化過濾規則
func initFilter() {
	f := getFilter()
	f.nodes = make(map[string]*FilterInfo)
	f.black = make(map[string]bool)
}
func getFilter() *Filter {
	return &g_filter
}
func getIP(addr net.Addr) string {
	strs := strings.SplitN(addr.String(), ":", 2)
	return strs[0]
}

//返回 是否 是 合法連接
func (f *Filter) SafeConnect(addr net.Addr) bool {
	f.mutex.Lock()
	defer f.mutex.Unlock()
	ip := getIP(addr)

	//在黑名單中
	if _, ok := f.black[ip]; ok {
		return false
	}

	//驗證安全
	if info, ok := f.nodes[ip]; ok {
		if info.SafeConnect() {
			return true
		}
		//加入黑名單
		f.black[ip] = true
		delete(f.nodes, ip)

		//通知防火牆 阻止連接
		go rejectIP(ip)

		//解除黑名單
		time.AfterFunc(time.Hour, func() {
			f.mutex.Lock()
			delete(f.black, ip)
			f.mutex.Unlock()
		})
		return false
	}
	f.nodes[ip] = newFilterInfo()
	return true
}
