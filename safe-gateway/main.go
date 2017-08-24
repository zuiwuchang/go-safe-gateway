package main

import (
	"king-go/net/basic"
	"log"
	"time"
)

const (
	ConfigureFile = "safe-gateway.json"
)

func main() {
	//加載配置
	e := initConfigure()
	if e != nil {
		log.Fatalln(e)
	}
	cnf := getConfigure()
	//初始化 日誌
	initLog()

	cnf.Safe.ShowLog()
	logInfo.Println("forward to", cnf.Addr)

	//初始化 過濾規則
	initFilter()

	//創建服務器 模板
	server := &Server{}

	//創建服務器
	s, e := basic.NewServer(cnf.LAddr, cnf.Timeout*time.Second, server)
	if e != nil {
		logFault.Println(e)
		return
	}
	logInfo.Println("work at", cnf.LAddr)

	//運行服務器
	s.Run()

	//等待服務器 停止
	s.Wait()
}
