package command

import (
	"log"
)

var logTrace *log.Logger
var logDebug *log.Logger
var logInfo *log.Logger
var logError *log.Logger
var logFault *log.Logger

func InitLog(trace, debug, info, err, fault *log.Logger) {
	logTrace = trace
	logDebug = debug
	logInfo = info
	logError = err
	logFault = fault
}
