package main

import (
	"go-safe-gateway/protocol"
	"go-safe-gateway/safe-server/command"
	"go-safe-gateway/safe-server/db/manipulator"
	"king-go/net/echo"
	"log"
	"time"
)

const (
	ConfigureFile = "safe-server.json"

	HeaderSize = protocol.HeaderSize
	HeaderFlag = protocol.HeaderFlag
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
	command.InitLog(logTrace, logDebug, logInfo, logError, logFault)

	logInfo.Println("DB :", cnf.DB)
	logInfo.Println("Duration :", cnf.Duration)

	//初始化 數據庫
	e = manipulator.Initialize(&cnf.DB)
	if e != nil {
		log.Fatalln(e)
	}

	//創建服務器 模板
	server, e := NewServer(getOrder())
	if e != nil {
		log.Fatalln(e)
	}

	//創建服務器
	s, e := echo.NewServer(cnf.LAddr, cnf.Timeout, server)
	if e != nil {
		logFault.Fatalln(e)
	}
	logInfo.Println("work at", cnf.LAddr)
	logInfo.Println("timeout", cnf.Timeout)
	go timeOut()

	//運行服務器
	s.Run()

	//等待服務器 停止
	s.Wait()
}
func timeOut() {
	cnf := getConfigure()
	var m manipulator.GoSafe
	for {
		m.Expired(cnf.Duration)
		time.Sleep(time.Hour)
	}
}
