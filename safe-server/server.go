package main

import (
	"encoding/binary"
	"errors"

	"go-safe-gateway/safe-server/command"
	"king-go/net/echo"
	"net"
)

func getOrder() binary.ByteOrder {
	return binary.LittleEndian
}

//服務器特化
type Server struct {
	byteOrder binary.ByteOrder
	cmds      map[uint16]command.ICommand
}

func initCmds() (map[uint16]command.ICommand, error) {
	cmds := make(map[uint16]command.ICommand)

	//ping
	cmd, e := command.NewCmdPing()
	if e != nil {
		return nil, e
	}
	cmds[cmd.Code()] = cmd

	//ping no rs
	cmd, e = command.NewCmdPingNo()
	if e != nil {
		return nil, e
	}
	cmds[cmd.Code()] = cmd

	//reject
	cmd, e = command.NewReject()
	if e != nil {
		return nil, e
	}
	cmds[cmd.Code()] = cmd
	return cmds, nil
}

//創建 特化的 服務器
func NewServer(byteOrder binary.ByteOrder) (echo.IServerTemplate, error) {
	//創建 指令 集
	cmds, e := initCmds()
	if e != nil {
		return nil, e
	}

	//創建服務器
	return &Server{
		byteOrder: byteOrder,
		cmds:      cmds,
	}, nil
}
func (s *Server) GetHeaderSize() int {
	return command.HeaderSize
}
func (s *Server) GetMessageSize(session0 echo.Session, b []byte) (int, error) {
	byteOrder := s.byteOrder

	//header size
	if len(b) != HeaderSize {
		e := errors.New("header size not match")
		logError.Println(e)
		return 0, e
	}
	//flag
	if byteOrder.Uint16(b) != HeaderFlag {
		e := errors.New("header flag not match")
		logError.Println(e)
		return 0, e
	}
	//msg size
	n := byteOrder.Uint16(b[2:])
	if n < HeaderSize {
		e := errors.New("message size not match")
		logError.Println(e)
		return 0, e
	}

	//cmd
	cmd := byteOrder.Uint16(b[4:])
	if _, ok := s.cmds[cmd]; !ok {
		e := errors.New("not found command")
		logError.Println(e)
		return 0, e
	}
	return int(n), nil
}
func (s *Server) NewSession(c net.Conn) (session echo.Session, e error) {
	//fmt.Println("one in", c.RemoteAddr())
	return &command.Session{}, e
}
func (s *Server) DeleteSession(c net.Conn, session echo.Session) {
	//fmt.Println("one out", c.RemoteAddr())
}
func (s *Server) Message(c net.Conn, session echo.Session, b []byte) error {
	byteOrder := s.byteOrder
	param := command.NewParam(b)
	cmd := param.Code(byteOrder)

	if v, ok := s.cmds[cmd]; ok {
		return v.Execute(byteOrder, c, session.(*command.Session), param)
	}
	return errors.New("command unknow")
}
