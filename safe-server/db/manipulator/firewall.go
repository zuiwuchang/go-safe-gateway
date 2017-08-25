package manipulator

import (
	"fmt"
	"os/exec"
)

func Add(ip string) error {
	name := fmt.Sprintf("0-no-%v", ip)
	str := fmt.Sprintf(`netsh advfirewall firewall add rule name="%v" remoteip="%v" dir=in action=block`,
		name,
		ip,
	)
	cmd := exec.Command("cmd", "/C", str)
	e := cmd.Run()
	if e != nil {
		return e
	}
	cmd.Wait()
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
	cmd.Wait()
	return nil
}
