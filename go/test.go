package main

import (
	"fmt"
	//"math"
    //"time"
    //"log"
)

func main() {
	x := 12
	switch x {
	case 12:
		fallthrough
	case 14:
		fmt.Println("i am 12 or 14")
	case 20:
		fmt.Println("i am 20")
	}
	
	y := 20

	if x-1>100 || 
		y-1>0 {
		fmt.Println("ok")
	}

	s := make([]string, 3)
	//fmt.Println("display s100:", s[100])

	//fmt.Println(math.Min(x,y))
}
