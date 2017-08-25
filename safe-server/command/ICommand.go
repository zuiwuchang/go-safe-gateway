package command

import (
	"encoding/binary"
	"go-safe-gateway/protocol"
	"net"
)

const (
	HeaderSize = protocol.HeaderSize
	HeaderFlag = protocol.HeaderFlag
)

//定義 指令參數 接口
type IParam interface {
	//返回 指令 完整數據
	Data() []byte

	//返回 指令 唯一值
	Code(byteOrder binary.ByteOrder) uint16

	//返回 指令 header
	Header() []byte

	//返回 指令 body
	Body() []byte
}

//定義 指令參數 默認 實現
type Param struct {
	data []byte
}

func NewParam(b []byte) IParam {
	return &Param{data: b}
}
func (p *Param) Data() []byte {
	return p.data
}
func (p *Param) Code(byteOrder binary.ByteOrder) uint16 {
	return byteOrder.Uint16(p.data[4:])
}
func (p *Param) Header() []byte {
	return p.data[:HeaderSize]
}
func (p *Param) Body() []byte {
	return p.data[HeaderSize:]
}

//定義 指令 接口
type ICommand interface {
	//返回 指令 唯一標識碼
	Code() uint16
	//執行指令 返回 error 將 斷開連接
	Execute(order binary.ByteOrder, c net.Conn, session *Session, param IParam) error
}

func WriteCmd(order binary.ByteOrder, c net.Conn, cmd uint16, b []byte) error {
	size := HeaderSize + len(b)
	data := make([]byte, size)

	order.PutUint16(data, HeaderFlag)
	order.PutUint16(data[2:], uint16(size))
	order.PutUint16(data[4:], cmd)

	copy(data[HeaderSize:], b)

	_, e := c.Write(data)
	return e
}
func WriteCmdBytes(order binary.ByteOrder, c net.Conn, cmd uint16, b []byte) error {
	order.PutUint16(b, HeaderFlag)
	order.PutUint16(b[2:], uint16(len(b)))
	order.PutUint16(b[4:], cmd)

	_, e := c.Write(b)
	return e
}
