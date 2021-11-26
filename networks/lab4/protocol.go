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
		log.Error("decoding message from server", "error", err)
	}
	if message.Id == id {
		*flag = true
	}
}

func send(conn *net.UDPConn, message string) {
	messageId := rand.Intn(100000)
	messageEncoded, _ := json.Marshal(Message{messageId, message})

	if _, err := conn.Write([]byte(string(messageEncoded))); err != nil {
		log.Error("sending request to server", "error", err, "message", message)
	} 

	// Waiting for response
	buff := make([]byte, 32)
	isReceived := false
	var delay int64 = 10
	time := makeTimestamp() - delay
	for {
		if isReceived {break}
		if makeTimestamp() - time >= delay {
			time = makeTimestamp()
			conn.Write([]byte(string(messageEncoded)))
			go receive(conn, buff, &isReceived, messageId)
		}
	}
	
	log.Info("successful interaction with server", "message", string(buff))
}