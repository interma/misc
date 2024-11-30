package main
import (
    "time"
    "log"
)

func test(ch chan int) {
	cnt := 0
	for i := 0; i < 3; i++ {
		go func(ch chan int) {
			time.Sleep(3 * time.Second)
			cnt++ //cnt also can visited after test() exited
			log.Println("in test",i,cnt)
			ch<-0
		}(ch)
	}
    log.Println("test exited")
}
func test1(ch chan int) {
	cnt := 0
	for i := 0; i < 3; i++ {
		go func(ch chan int, cnt int) {
			time.Sleep(3 * time.Second)
			cnt++
			log.Println("in test1",i,cnt)
			ch<-0
		}(ch,cnt)
	}
    log.Println("test1 exited")
}

func main() {
	ch := make(chan int)

	test(ch)
	for i := 0; i < 3; i++ {
		<-ch
	}
	test1(ch)
	for i := 0; i < 3; i++ {
		<-ch
	}
    log.Println("main exited")
}

