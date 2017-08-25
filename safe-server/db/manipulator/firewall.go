package manipulator

import (
	"fmt"
)

func Add(ip string) error {
	fmt.Println("firewall add", ip)
	return nil
}
func Erase(ip string) error {
	fmt.Println("firewall erase", ip)
	return nil
}
