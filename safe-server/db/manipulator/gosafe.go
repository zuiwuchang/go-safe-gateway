package manipulator

import (
	"errors"
	"go-safe-gateway/safe-server/db/data"
	"strings"
	"time"
)

type GoSafe struct {
}

//添加一個 ip 到封鎖列表
func (GoSafe) Add(ip string) error {
	ip = strings.TrimSpace(ip)
	if len(ip) == 0 {
		return errors.New("ip cna't be empty")
	}

	session := NewSession()
	defer session.Close()

	e := session.Begin()
	if e != nil {
		return e
	}
	defer func() {
		if e == nil {
			session.Commit()
		} else {
			session.Rollback()
		}
	}()
	var bean data.GoSafe
	bean.Ip = ip

	has, err := session.Get(&bean)
	if err != nil {
		e = err
		return e
	}

	//增加到防火牆設置
	e = Add(ip)
	if e != nil {
		return e
	}

	//增加到 數據庫 記錄
	if has {
		_, e = session.Id(bean.Id).Update(&data.GoSafe{Ip: ip})
	} else {
		_, e = session.Insert(&data.GoSafe{Ip: ip})
	}
	return e
}

//解封ip
func (GoSafe) Expired(duration time.Duration) error {
	session := NewSession()
	defer session.Close()

	e := session.Begin()
	if e != nil {
		return e
	}
	defer func() {
		session.Commit()
	}()

	last := getTimeString(time.Now().Add(-duration))
	var nodes []data.GoSafe
	e = session.Where("last < ? ", last).Find(&nodes)
	if e != nil {
		return e
	}

	for i := 0; i < len(nodes); i++ {
		Erase(nodes[i].Ip)
	}

	if len(nodes) > 0 {
		_, e = session.Where("last < ? ", last).Delete(data.GoSafe{})
	}
	return e
}
func getTimeString(t time.Time) string {
	layout := "2006-01-02 15:04:05"
	return t.Format(layout)
}
