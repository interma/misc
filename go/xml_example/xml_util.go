package main

import (
	"encoding/xml"
	"io/ioutil"
	"os"
)

type HawqConfiguration struct {
	XMLName		xml.Name `xml:"configuration"`
	Property	[]HawqProperty	`xml:"property"`
}

type HawqProperty struct {
	XMLName		xml.Name `xml:"property"`
	Name	string	`xml:"name"`
	Value	string	`xml:"value"`
}

func GetHawqConfiguration(filepath string) (*HawqConfiguration, error) {
	file, err := os.Open(filepath)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	data, err := ioutil.ReadAll(file)
	if err != nil {
		return nil, err
	}

	conf := HawqConfiguration{}
	err = xml.Unmarshal(data, &conf)
	if err != nil {
		return nil, err
	}

	return &conf, nil
}

func GetXmlContent(conf *HawqConfiguration) (string, error) {
	output, err := xml.MarshalIndent(conf, "", "	")
	str := string(output[:])
	str = xml.Header + str
	return str, err
}
