package main

import (
	"fmt"
	"github.com/sparrc/go-ping"
)

func defenestrate() {
	pinger, err := ping.NewPinger("github.com")
	if err != nil {
		panic(err)	
	}

	pinger.Count = 3	
	pinger.Run() // blocks until finished

	stats := pinger.Statistics() // get send/receive/rtt stat
	fmt.Println(stats)
	go defenestrate()
}

func main() {
	for i := 0; i < 666; i++ {
		go defenestrate()
	}

	exit := ""
	fmt.Scanf("%s", exit)
}