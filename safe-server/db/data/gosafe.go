package data

import (
	"time"
)

type GoSafe struct {
	//自動生成的 記錄標識
	Id int64

	//被封鎖的ip地址
	Ip string `xorm:"index unique"`

	//最後封鎖時間
	Last time.Time `xorm:"updated"`
}
