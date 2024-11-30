package main 
import (
	"os"
	"fmt"
	"net/url"
	"github.com/colinmarc/hdfs"
)

const root_path = "hdfs://127.0.0.1:9000/hawqcluster/"

func basic_read()  {
	client, _ := hdfs.New("127.0.0.1:9000")
	file, _ := client.Open("/1.txt")

	buf := make([]byte, 59)
	file.ReadAt(buf, 0)

	fmt.Println(string(buf))
}


//return: -1 failed (with a non-nil error return), 0 no existed (then create), 1 existed
func createWhenNoExisted(path string) (int, error)  {

	u, err := url.Parse(path)
	if err != nil {
		return -1, err
	}

	fmt.Println(u.Host)
	fmt.Println(u.Path)

	hdfsAddr := u.Host
	hdfsRootPath := u.Path

	client, err := hdfs.New(hdfsAddr)
	if err != nil {
		return -1, err
	}

	//check existed
	fi, err := client.Stat(hdfsRootPath)
	if err == nil && fi.IsDir() {
		return 1, nil
	}

	var mode = 0777 | os.ModeDir
	//create dir
	err = client.MkdirAll(hdfsRootPath, mode)
	if err != nil {
		return -1, err
	}

	//set owner to gpadmin
	err = client.Chown(hdfsRootPath, "gpadmin", "gpadmin")
	if err != nil {
		return -1, err
	}

	return 0, nil
}

func main() {
	//basic_read()
	ret, err := createWhenNoExisted(root_path)
	if ret == 0 {
		fmt.Printf("create successfully\n")
	} else if ret == 1 {
		fmt.Printf("existed\n")
	} else if err != nil {
		fmt.Printf("failed, err:%s\n", err.Error())
	}
}
