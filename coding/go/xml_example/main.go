package main

import (
	"fmt"
)

var defaultHawqsitePath = "hawq-site.xml"

func printConfiguration(conf *HawqConfiguration)  {
	for i, property := range conf.Property {
		fmt.Printf("property[%d] %s:%s\n", i, property.Name, property.Value)
	}
}

func mergeConfiguration(m map[string]string, conf *HawqConfiguration) {
	for i, property := range conf.Property {
		for key, value := range m {
			if key == property.Name {
				fmt.Printf("find one: %s\n", key)
				//property.Value = value
				conf.Property[i].Value = value
			}
		}
	}
}


func main() {
	conf, err := GetHawqConfiguration(defaultHawqsitePath)
	if err != nil {
		fmt.Printf("get conf failed, error:%v\n", err)
		return
	}

	printConfiguration(conf)

	m := make(map[string]string)
	m["hawq_master_address_host"] = "1.2.3.4"
	m["hawq_rps_address_port"] = "1234"

	mergeConfiguration(m, conf)

	fmt.Println("new hawq file content:")
	content, err := GetXmlContent(conf)
	fmt.Println(content)

	return
}
