package main

import (
	"flag"
	"fmt"
	// "github.com/mgutz/logxi/v1"
	// "math/rand"
	"net"
	"os"
	"bufio"
	// "strings"
	"strconv"
	"encoding/json"
	"time"
)

func makeTimestamp() int64 {
    return time.Now().UnixNano() / int64(time.Millisecond)
}

type Client struct {
	username string
	port int
	addresses []string
}

type Message struct {
	Time string
	Username string
	Text string
}

func (client *Client) add_address (addr string) {
	client.addresses = append(client.addresses, addr)
}

func (client *Client) serve() {
	fmt.Println("Launching server...")

	// Setup port listener
	ln, _ := net.Listen("tcp", ":" + strconv.Itoa(client.port))

	// Запускаем цикл
	for {
		conn, _ := ln.Accept()
		// Listening
		message_encoded, _ := bufio.NewReader(conn).ReadString('\n')
		// Decoding
		var message Message
		json.Unmarshal([]byte(message_encoded), &message)
		if message.Username != client.username {
			fmt.Print(message.Username + "[" + message.Time + "]: " + message.Text)
		}
	}
}

func (client *Client) send(text string) {
	var message Message
	message.Text = text
	message.Time = time.Now().Format("2006-01-02 15:04:05")
	message.Username = client.username
	for i := 0; i < len(client.addresses); i++ {
		go func(j int) {
			conn, _ := net.Dial("tcp", client.addresses[j])
			if conn == nil {return}
			message_encoded, _ := json.Marshal(message)
			message_encoded = append(message_encoded, '\n')
			conn.Write(message_encoded)
		}(i)
	}
}

func (client *Client) input_processing() {
	for {
		reader := bufio.NewReader(os.Stdin)
		text, _ := reader.ReadString('\n')
		client.send(text)
	}
}

func main() {
	var client Client

	// Parsing from command line
	flag.StringVar(&client.username, "username", "username", "set username")
	flag.IntVar(&client.port, "port", 8080, "set port")
	flag.Parse()

	

	// TODO: Address list parsing
	client.add_address("localhost:8081")
	client.add_address("localhost:8082")
	client.add_address("localhost:8083")

	fmt.Println(client)

	go client.serve()
	client.input_processing()
}
