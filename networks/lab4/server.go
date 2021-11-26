package main

import (
	"flag"
	"fmt"
	"github.com/mgutz/logxi/v1"
	"net"
	"os"
	"encoding/json"
	"time"
)

func parseMessage(m string) (timer int, text string) {
	num := 0
	pwr := 1
	i := 0
	for ; i < len(m); i++ {
		if m[i] >= 48 && m[i] < 58 {
			num += int(m[i] - 48) * pwr
		} else {break}
		pwr *= 10
	}
	return num, m[i:]
}

func action(addr *net.UDPAddr, m string) {
	timer, text := parseMessage(m)
	time.Sleep(time.Duration(timer) * time.Second)
	//time := makeTimestamp()
	connNew, _ := net.DialUDP("udp", nil, addr)
	send(connNew, text)
}

func main() {
	var (
		serverAddrStr string
		helpFlag      bool
	)
	flag.StringVar(&serverAddrStr, "addr", "127.0.0.1:6000", "set server IP address and port")
	flag.BoolVar(&helpFlag, "help", false, "print options list")

	if flag.Parse(); helpFlag {
		fmt.Fprint(os.Stderr, "server [options]\n\nAvailable options:\n")
		flag.PrintDefaults()
	} else if serverAddr, err := net.ResolveUDPAddr("udp", serverAddrStr); err != nil {
		log.Error("resolving server address", "error", err)
	} else if conn, err := net.ListenUDP("udp", serverAddr); err != nil {
		log.Error("creating listening connection", "error", err)
	} else {
		log.Info("server listens incoming messages from clients")
		for {
			buff := make([]byte, 1024)
			if bytesRead, addr, err := conn.ReadFromUDP(buff); err != nil {
				log.Error("receiving message from client", "error", err)
			} else {
				var message Message;
				err := json.Unmarshal(buff[:bytesRead], &message)
				if err == nil {
					log.Info("JSON decoded", "Id", message.Id, "Content", message.Content)
					messageEncoded, _ := json.Marshal(Message{message.Id, message.Content})
					conn.WriteToUDP([]byte(messageEncoded), addr)
					go action(addr, message.Content)
				} else {
					log.Error("decoding message from client", "error", err)
				}
			}
		}
	}
}
