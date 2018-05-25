package main

import (
	"flag"
	"fmt"
)

var (
	hawqImage   string
)

func main() {
	flag.Parse()
	fmt.Printf("I am running, image: %s\n", hawqImage)
}

func init() {
	flag.StringVar(&hawqImage, "hawqimage", "hawqbeijing/hawq_proxy", "hawq docker image which used by operator")
}

