package main

import (
	"fmt"
    "github.com/astaxie/beego"
)

type MainController struct {
    beego.Controller
}

func (this *MainController) Get() {
    this.Ctx.WriteString("hello world")
}

func (this *MainController) Post() {
	jsoninfo := this.GetString("rm_data")
	if jsoninfo == "" {
		this.Ctx.WriteString("{'ret':-1, 'err':no rm data field, 'data':''}")
		return
	}
	fmt.Printf("rm_data:%s\n", jsoninfo)
	this.Ctx.WriteString("{'ret':0}")
}

func main() {
    beego.Router("/", &MainController{})
    beego.Run()
}
