package main

import (
	"errors"
	"king-go/net/basic"
	"net"
)

type Server struct {
}

func (s *Server) NewSession(c net.Conn) (basic.Session, error) {
	if !getFilter().SafeConnect(c.RemoteAddr()) {
		return nil, errors.New("connect attack")
	}

	cs, e := net.Dial("tcp", getConfigure().Addr)
	if e != nil {
		return nil, e
	}

	go func() {
		b := make([]byte, 1024)
		for {
			n, e := cs.Read(b)
			if e != nil {
				break
			}
			_, e = c.Write(b[:n])
			if e != nil {
				break
			}
		}
		c.Close()
	}()
	session := &Session{cs: cs, cc: c}
	logTrace.Println("one in :", session)
	return session, nil
}
func (s *Server) DeleteSession(c net.Conn, session basic.Session) {
	logTrace.Println("one out :", session)
	s0 := session.(*Session)
	s0.Close()
}
func (s *Server) Message(c net.Conn, session basic.Session, b []byte) error {
	s0 := session.(*Session)
	_, e := s0.cs.Write(b)
	return e
}
