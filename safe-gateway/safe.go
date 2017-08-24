package main

import (
	"encoding/binary"
	"errors"
	"go-safe-gateway/protocol"
	"king-go/net/echo"
	"net"
	"time"
)

//client 模板實現
type clientTemplate struct {
}

func (clientTemplate) GetHeaderSize() int {
	return protocol.HeradrSize
}
func (clientTemplate) GetMessageSize(b []byte) (int, error) {
	if len(b) != protocol.HeradrSize {
		return 0, errors.New("header size not match")
	}
	order := getOrder()
	if order.Uint16(b) != protocol.HeradrFlag {
		return 0, errors.New("header flag not match")
	}
	n := order.Uint16(b[2:])
	if n < protocol.HeradrSize {
		return 0, errors.New("message size not match")
	}
	return int(n), nil
}

func getOrder() binary.ByteOrder {
	return binary.LittleEndian
}

//講指定ip 封鎖
func rejectIP(ip string) {
	cnf := getConfigure()
	c, e := echo.NewClient(cnf.Safe.Addr, clientTemplate{})
	if e != nil {
		logError.Println(e)
		return
	}
	defer c.Close()

	e = writeCmd(c, protocol.CommandReject, []byte(ip))
	if e != nil {
		logError.Println(e)
		return
	}
	c.GetMessage(time.Second * 5)
}
func writeCmd(c net.Conn, cmd uint16, data []byte) error {
	n := len(data) + protocol.HeradrSize
	b := make([]byte, n)

	order := getOrder()
	order.PutUint16(b, protocol.HeradrFlag)
	order.PutUint16(b[2:], uint16(n))
	order.PutUint16(b[4:], cmd)
	copy(b[protocol.HeradrSize:], data)

	_, e := c.Write(b)
	return e
}
