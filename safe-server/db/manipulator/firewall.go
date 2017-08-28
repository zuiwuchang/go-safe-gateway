package manipulator

import (
	"fmt"
	"os/exec"
)

func Has(name string) bool {
	str := fmt.Sprintf(`netsh advfirewall firewall show rule name="%v" dir=in`,
		name,
	)
	cmd := exec.Command("cmd", "/C", str)
	e := cmd.Run()
	if e != nil {
		return false
	}
	return true
}
func Add(ip string) error {
	name := fmt.Sprintf("0-no-%v", ip)
	if Has(name) {
		return nil
	}
	str := fmt.Sprintf(`netsh advfirewall firewall add rule name="%v" remoteip="%v" dir=in action=block`,
		name,
		ip,
	)
	cmd := exec.Command("cmd", "/C", str)
	e := cmd.Run()
	if e != nil {
		return e
	}
	return nil
}
func Erase(ip string) error {
	name := fmt.Sprintf("0-no-%v", ip)
	str := fmt.Sprintf(`netsh advfirewall firewall delete rule name="%v"`,
		name,
	)
	cmd := exec.Command("cmd", "/C", str)
	e := cmd.Run()
	if e != nil {
		return e
	}
	return nil
}
