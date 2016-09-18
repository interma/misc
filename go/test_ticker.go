package main

import (
    "time"
    "log"
)

func test1() {
    ticker := time.NewTicker(1 * time.Second)
    go func(){
        for _ = range ticker.C {
            log.Println("tick")
        }
        log.Println("stopped")
    }()
    time.Sleep(3 * time.Second)
    log.Println("stopping ticker")
    ticker.Stop() //don't close C
    time.Sleep(3 * time.Second)
}
func test2() {
	//confuse follow code
    ticker := time.NewTicker(1 * time.Second)
    go func(){
		time.Sleep(3 * time.Second)
        for _ = range ticker.C {
            log.Println("tick")
        }
        log.Println("stopped")
    }()
    time.Sleep(6 * time.Second)
    log.Println("stopping ticker")
    ticker.Stop() //don't close C
}
func test3() {
	//this is enough from my raft 
    log.Println("timer seted!") 
	timer := time.NewTimer(2*time.Second) 
    time.Sleep(3 * time.Second)
	//timer.Reset(2*time.Second)
	go func(){
		<- timer.C 
		log.Println("timer expired!")
	}()
    time.Sleep(4 * time.Second)
}
func test4() {
	//this is enough from my raft 
	timer := time.NewTimer(2*time.Second) 
    log.Println("timer seted!") 
	go func(){
		<- timer.C 
		log.Println("timer expired!")
	}()
    time.Sleep(4 * time.Second)
}


func main() {
	//test4()
	test3()
}
