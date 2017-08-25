package main

import (
	"encoding/json"
	"go-safe-gateway/safe-server/db/manipulator"
	"io/ioutil"
	"time"
)

type Configure struct {
	LAddr   string
	Timeout time.Duration
	DB      manipulator.Configure

	Duration time.Duration

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

	cnf.Timeout *= time.Second
	cnf.Duration *= time.Hour
	cnf.DB.Live *= time.Minute
	return nil
}
