package main

import (
	"github.com/emicklei/go-restful"
	"fmt"
	"io"
	"log"
	"net/http"
)

// This example shows the minimal code needed to get a restful.WebService working.
//
// GET http://localhost:8080/hello

func main() {
	ws := new(restful.WebService)
	ws.Route(ws.GET("/").To(hello))
	ws.Route(ws.GET("/get").To(get))
	//ws.Route(ws.POST("/post").Consumes("application/x-www-form-urlencoded").To(post))
	ws.Route(ws.POST("/post").To(post))
	restful.Add(ws)
	log.Fatal(http.ListenAndServe(":8080", nil))
}

func hello(req *restful.Request, resp *restful.Response) {
	io.WriteString(resp, "hello world")
}

func get(req *restful.Request, resp *restful.Response) {
	output_json := "{\"ret\":0, \"err_msg\":\"I am running GET\"}"
	io.WriteString(resp, output_json)
}

//curl -d rm_data="{'str':123, 'other':'xx'}" 127.0.0.1:8080/post
func post(req *restful.Request, resp *restful.Response) {
	err := req.Request.ParseForm()
	if err != nil {
		resp.WriteErrorString(http.StatusBadRequest, err.Error())
		return
	}
	fmt.Printf("%v\n", req.Request.PostForm);
	rm_data := req.Request.PostForm["rm_data"][0]
	if rm_data == "" {
		rm_data = "EMPTY"
	}
	io.WriteString(resp.ResponseWriter, fmt.Sprintf("rm_data=%s", rm_data))
}
