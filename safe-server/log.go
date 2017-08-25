package main

import (
	"log"
	"os"
)

var logTrace *log.Logger
var logDebug *log.Logger
var logInfo *log.Logger
var logError *log.Logger
var logFault *log.Logger

type nullOut struct {
}

func (nullOut) Write(b []byte) (int, error) {
	return len(b), nil
}

func initLog() {
	cnf := getConfigure()
	flag := log.LstdFlags
	if cnf.LogLine {
		flag |= log.Lshortfile
	}

	if _, ok := cnf.Log["TRACE"]; ok {
		logTrace = log.New(os.Stdout, "[TRACE] ", flag)
	} else {

		logTrace = log.New(nullOut{}, "[TRACE] ", flag)
	}

	if _, ok := cnf.Log["DEBUG"]; ok {
		logDebug = log.New(os.Stdout, "[DEBUG] ", flag)
	} else {

		logDebug = log.New(nullOut{}, "[DEBUG] ", flag)
	}

	if _, ok := cnf.Log["INFO"]; ok {
		logInfo = log.New(os.Stdout, "[INFO] ", flag)
	} else {

		logInfo = log.New(nullOut{}, "[INFO] ", flag)
	}

	if _, ok := cnf.Log["ERROR"]; ok {
		logError = log.New(os.Stdout, "[ERROR] ", flag)
	} else {

		logError = log.New(nullOut{}, "[ERROR] ", flag)
	}

	if _, ok := cnf.Log["FAULT"]; ok {
		logFault = log.New(os.Stdout, "[FAULT] ", flag)
	} else {

		logFault = log.New(nullOut{}, "[FAULT] ", flag)
	}
}
