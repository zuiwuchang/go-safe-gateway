package main

import (
	"net"
)

type Session struct {
	//到服務器的連接
	cs net.Conn

	//到客戶端的連接
	cc net.Conn
}

func (s *Session) Close() {
	s.cs.Close()
}
