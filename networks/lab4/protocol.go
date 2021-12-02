package main

import (
    "encoding/json"
	"github.com/mgutz/logxi/v1"
	"time"
	"net"
	"math/rand"
)

type Message struct {
	Id int
	Content string
}

func makeTimestamp() int64 {
    return time.Now().UnixNano() / int64(time.Millisecond)
}

func receive(conn *net.UDPConn, buff []byte, flag *bool, id int) {
	bytesRead, err := conn.Read(buff);
	if err != nil {
		log.Error("receiving answer from server", "error", err)
	}
	var message Message;
	err = json.Unmarshal(buff[:bytesRead], &message)
	if err != nil {
		//log.Error("decoding response", "message", buff[:bytesRead])
	} else if message.Id == id {
		*flag = true
	}
}

var sentIds map[int]bool 

func initProtocol() {
	rand.Seed(time.Now().UnixNano())
	sentIds = make(map[int]bool)
}

func send(conn *net.UDPConn, addr *net.UDPAddr, message string, closeConn bool) {
	messageId := rand.Intn(100000)
	messageEncoded, _ := json.Marshal(Message{messageId, message})

	/*if _, err := conn.Write([]byte(string(messageEncoded))); err != nil {
		log.Error("sending request to server", "error", err, "message", message)
	}*/

	// Waiting for response
	buff := make([]byte, 32)
	isReceived := false
	var delay int64 = 20
	time := makeTimestamp() - delay
	for i := 0; i < 1000; {
		if isReceived {break}
		if makeTimestamp() - time >= delay {
			time = makeTimestamp()
			//conn.WriteToUDP([]byte(string(messageEncoded)), addr)
			conn.Write([]byte(string(messageEncoded)))
			go receive(conn, buff, &isReceived, messageId)
			i++
		}
	}

	if closeConn {
		defer conn.Close()
	}

	sentIds[messageId] = true
	if !isReceived {
		log.Error("Failed to send message", "message", message, "addr", addr)
	} else {
		log.Info("message sent successfully", "message", message, "addr", addr)
	}
}