package manipulator

import (
	"time"
)

type Configure struct {
	//數據庫 驅動 mysql
	Driver string
	//連接字符串
	ConStr string
	//是否需要顯示 sql 指令
	ShowSql bool
	//數據庫 保活時間 單位 分鐘
	Live time.Duration
}
