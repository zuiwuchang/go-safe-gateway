package command

import (
	"encoding/binary"
	"go-safe-gateway/protocol"
	"go-safe-gateway/safe-server/db/manipulator"
	"net"
)

func NewReject() (ICommand, error) {
	return cmdReject{}, nil
}

type cmdReject struct {
}

func (cmdReject) Code() uint16 {
	return protocol.CommandReject
}
func (cmd cmdReject) Execute(order binary.ByteOrder, c net.Conn, session *Session, param IParam) error {
	ip := string(param.Body())

	var m manipulator.GoSafe
	e := m.Add(ip)
	if e == nil {
		return WriteCmd(order, c, cmd.Code(), nil)
	}
	logError.Println(e)
	return e
}
