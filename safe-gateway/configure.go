package main

import (
	"encoding/json"
	"io/ioutil"
	"time"
)

type Safe struct {
	Num      int
	Duration time.Duration
	Addr     string
}

func (s *Safe) ShowLog() {
	logInfo.Println("safe addr", s.Addr)
	logInfo.Println("safe num", s.Num)
	logInfo.Println("safe Duration", s.Duration*time.Millisecond)
}

type Configure struct {
	LAddr   string
	Addr    string
	Timeout time.Duration
	Safe    Safe

	Logs    []string
	Log     map[string]int
	LogLine bool
}

var g_cnf Configure

func getConfigure() *Configure {
	return &g_cnf
}
func initConfigure() error {
	b, e := ioutil.ReadFile(ConfigureFile)
	if e != nil {
		return e
	}
	cnf := getConfigure()
	e = json.Unmarshal(b, cnf)
	if e != nil {
		return e
	}

	cnf.Log = make(map[string]int)
	for _, node := range cnf.Logs {
		cnf.Log[node] = 1
	}
	cnf.Logs = nil
	return nil
}
