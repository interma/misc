package main

import (
	"fmt"
)

var defaultHawqsitePath = "hawq-site.xml"

func main() {
	conf, err := GetHawqConfiguration(defaultHawqsitePath)
	if err != nil {
		fmt.Printf("get conf failed, error:%v\n", err)
		return
	}

	for i, property := range conf.Property {
		fmt.Printf("property[%d] %s:%s\n", i, property.Name, property.Value)
	}

	return
}
