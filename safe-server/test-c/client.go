package main

import (
	"encoding/binary"
	"errors"
	"fmt"
	"king-go/net/echo"
	"net"
)

//client 模板實現
type clientTemplate struct {
}

func (clientTemplate) GetHeaderSize() int {
	return HeaderSize
}
func (clientTemplate) GetMessageSize(b []byte) (int, error) {
	if len(b) != HeaderSize {
		return 0, errors.New("header size not match")
	}
	order := getOrder()
	if order.Uint16(b) != HeaderFlag {
		return 0, errors.New("header flag not match")
	}
	n := order.Uint16(b[2:])
	if n < HeaderSize {
		return 0, errors.New("message size not match")
	}
	return int(n), nil
}

func getOrder() binary.ByteOrder {
	return binary.LittleEndian
}

//講指定ip 封鎖
func getConn() (echo.IClient, error) {
	return echo.NewClient(Addr, clientTemplate{})
}
func writeCmd(c net.Conn, cmd uint16, data []byte) error {
	n := len(data) + HeaderSize
	b := make([]byte, n)

	order := getOrder()
	order.PutUint16(b, HeaderFlag)
	order.PutUint16(b[2:], uint16(n))
	order.PutUint16(b[4:], cmd)
	copy(b[HeaderSize:], data)

	_, e := c.Write(b)
	return e
}
