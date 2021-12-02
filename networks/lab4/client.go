package main

import (
	"flag"
	"fmt"
	"github.com/mgutz/logxi/v1"
	//"math/rand"
	"net"
	"os"
	"strconv"
	"encoding/json"
)


func main() {
	var (
		serverAddrStr string
		n             uint
		helpFlag      bool
	)
	initProtocol()
	usedIds := make(map[int]bool)

	flag.StringVar(&serverAddrStr, "server", "127.0.0.1:6000", "set server IP address and port")
	flag.UintVar(&n, "n", 10, "set the number of requests")
	flag.BoolVar(&helpFlag, "help", false, "print options list")

	if flag.Parse(); helpFlag {
		fmt.Fprint(os.Stderr, "client [options]\n\nAvailable options:\n")
		flag.PrintDefaults()
	} else if serverAddr, err := net.ResolveUDPAddr("udp", serverAddrStr); err != nil {
		log.Error("resolving server address", "error", err)
	} else if conn, err := net.DialUDP("udp", nil, serverAddr); err != nil {
		log.Error("creating connection to server", "error", err)
	} else {
		defer conn.Close()
		for i := uint(0); i < n; i++ {
			x := 3
			send(conn, serverAddr,string(strconv.Itoa(x)) + " foo", false)
		}
		
		// Receiving
		//var addrudp  = conn.LocalAddr().String()
		net.ListenUDP("udp", conn.LocalAddr().(*net.UDPAddr))
		log.Info("listening on", conn.LocalAddr().(*net.UDPAddr))
		for {
			buff := make([]byte, 1024)
			if bytesRead, addr, err := conn.ReadFromUDP(buff); err != nil {
				log.Error("receiving message from client", "error", err)
			} else {
				var message Message;
				err := json.Unmarshal(buff[:bytesRead], &message)
				//if sentIds[message.Id] {continue}
				if err == nil {
					if !usedIds[message.Id] {
						log.Info("JSON decoded", "Id", message.Id, "Content", message.Content)						
						usedIds[message.Id] = true
					}
					messageEncoded, _ := json.Marshal(Message{message.Id, message.Content})
					conn.WriteToUDP([]byte(messageEncoded), addr)
				} else {
					log.Error("decoding message from server", "error", err)
				}
			}
		}
	}
}
