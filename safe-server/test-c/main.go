package main

import (
	"bufio"
	"fmt"
	"go-safe-gateway/protocol"
	"log"
	"os"
	"strings"
	"time"
)

const (
	Addr       = "127.0.0.1:6666"
	HeaderSize = protocol.HeaderSize
	HeaderFlag = protocol.HeaderFlag
	Timeout    = time.Second * 5
)

func main() {
	r := bufio.NewReader(os.Stdin)
	for {
		fmt.Print("test-c $>")
		b, _, e := r.ReadLine()
		if e != nil {
			log.Fatalln(e)
		}
		str := strings.TrimSpace(string(b))
		if str == "e" {
			break
		}
		doCommand(str)
		fmt.Println()
	}
}
func doCommand(str string) {
	if strings.HasPrefix(str, "reject ") {
		ip := str[len("reject "):]
		ip = strings.TrimSpace(ip)
		doReject(ip)
	} else if str == "p" {
		doPing()
	}
}
func doPing() {
	c, e := getConn()
	if e != nil {
		log.Println(e)
		return
	}
	defer c.Close()

	e = writeCmd(c, protocol.CommandPing, nil)
	if e != nil {
		log.Println(e)
		return
	}

	_, e = c.GetMessage(0)
	if e != nil {
		log.Println(e)
		return
	}
	fmt.Println("ping success")
}
func doReject(ip string) {
	c, e := getConn()
	if e != nil {
		log.Println(e)
		return
	}
	defer c.Close()

	e = writeCmd(c, protocol.CommandReject, []byte(ip))
	if e != nil {
		log.Println(e)
		return
	}
	_, e = c.GetMessage(0)
	if e != nil {
		log.Println(e)
		return
	}
	fmt.Println("reject", ip, "success")
}
