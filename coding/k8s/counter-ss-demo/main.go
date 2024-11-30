package main

import (
	"fmt"
	"flag"
	"io/ioutil"
	"strings"
	"strconv"
	"time"
)

var (
	StorePath string
)

func main()  {
	flag.Parse()
	fmt.Printf("counter is running, store path:%s\n", StorePath)

	var initialValue int = 0

	//open file to read initial value
	filePath := StorePath+"/data_store"
	data, err := ioutil.ReadFile(filePath)
	if err != nil {
		initialValue = 0
		fmt.Printf("cannot found file:%s, initialValue = 0\n", filePath)
	}

	count := strings.Trim(string(data), "\t\n\r ")
	if count == "" {
		initialValue = 0
	} else {
		initialValue, err = strconv.Atoi(count)
		if err != nil {
			panic(err)
		}
	}
	fmt.Printf("restore initialValue = %d\n", initialValue)

	//loop echo & write
	var interval time.Duration = 1
	curValue := initialValue+1
	for {
		output := fmt.Sprintf("%d", curValue)
		err := ioutil.WriteFile(filePath, []byte(output), 0644)
		if err != nil {
			panic(err)
		}
		fmt.Printf("current value: %d\n", curValue)
		curValue++
		time.Sleep(interval * time.Second)
	}

	fmt.Println("counter is exiting")
}

func init() {
	flag.StringVar(&StorePath, "store_path", "/tmp/", "store_path")
}