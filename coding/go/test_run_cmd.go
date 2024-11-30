
package main

import (
	"fmt"
	"log"
	"os/exec"
	"strconv"
	"strings"
)


func runcmd(cmd string, shell bool) []byte {
	if shell {
		out, err := exec.Command("bash", "-c", cmd).Output()
		if err != nil {
			log.Fatal(err)
			panic("some error found")
		}
		return out
	}
	out, err := exec.Command(cmd).Output()
	if err != nil {
		log.Fatal(err)
	}
	return out
}

func main() {
	var hawq_path string = "/Users/hma/hawq/install"
	var cmd string
	cmd = fmt.Sprintf("source %s/greenplum_path.sh; hawq stop cluster -a", hawq_path)
	//out := runcmd(cmd, true)
	//fmt.Printf("output: %s\n", string(out))
	
	//cmd = fmt.Sprintf("source %s/greenplum_path.sh; psql postgres -c \"\\d\"", hawq_path)
	cmd = fmt.Sprintf("ps aux | grep postgres | grep master | wc -l")
	out := runcmd(cmd, true)
	outlines,_ := strconv.Atoi(strings.Trim(string(out), "\t\n\r "))
	fmt.Printf("%d lines, output: %s\n", outlines, string(out))
}
