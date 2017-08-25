package command

import (
	"encoding/binary"
	"net"
)

func NewCmdPing() (ICommand, error) {
	return &cmdPing{code: CmdPing, rs: true}, nil
}
func NewCmdPingNo() (ICommand, error) {
	return &cmdPing{code: CmdPingNo, rs: false}, nil
}

type cmdPing struct {
	rs   bool
	code uint16
}

func (cmd *cmdPing) Code() uint16 {
	return cmd.code
}
func (cmd *cmdPing) Execute(order binary.ByteOrder, c net.Conn, session *Session, param IParam) error {
	if cmd.rs {
		return WriteCmd(order, c, cmd.Code(), nil)
	}
	return nil
}
