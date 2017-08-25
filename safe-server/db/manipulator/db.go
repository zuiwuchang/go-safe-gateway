package manipulator

import (
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"github.com/go-xorm/xorm"
	"go-safe-gateway/safe-server/db/data"
	"time"
)

var g_engine *xorm.Engine

func Initialize(cnf *Configure) (ers error) {
	defer func() {
		if e := recover(); e != nil {
			//將異常包裝爲error
			ers = fmt.Errorf("%v", e)
		}

	}()
	//create engine
	var err error
	g_engine, err = xorm.NewEngine(cnf.Driver, cnf.ConStr)
	if err != nil {
		panic(err)
	}
	if err = g_engine.Ping(); err != nil {
		panic(err)
	}

	if cnf.ShowSql {
		g_engine.ShowSQL(true)
	}

	//keep live
	go func(live time.Duration) {
		for {
			time.Sleep(live)
			g_engine.Ping()
		}
	}(cnf.Live)

	//init table
	var bean data.GoSafe
	initTable(&bean)
	return
}
func initTable(bean interface{}) {
	engine := GetEngine()
	if ok, err := engine.IsTableExist(bean); err != nil {
		panic(err)
	} else if ok {
		engine.Sync2(bean)
	} else {
		if err = engine.CreateTables(bean); err != nil {
			panic(err)
		}
		if err = engine.CreateIndexes(bean); err != nil {
			panic(err)
		}
		if err = engine.CreateUniques(bean); err != nil {
			panic(err)
		}
	}
}
func GetEngine() *xorm.Engine {
	return g_engine
}
func NewSession() *xorm.Session {
	return g_engine.NewSession()
}
