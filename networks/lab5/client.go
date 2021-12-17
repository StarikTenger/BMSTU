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
	// "encoding/json"
)

type Client struct {
	username string
	port int
	addresses []string
}

func (client *Client) add_address (addr string) {
	client.addresses = append(client.addresses, addr)
}

func (client *Client) serve() {
	fmt.Println("Launching server...")

	// Устанавливаем прослушивание порта
	ln, _ := net.Listen("tcp", ":" + strconv.Itoa(client.port))

	// Запускаем цикл
	for {
		conn, _ := ln.Accept()
		// Будем прослушивать все сообщения разделенные \n
		message, _ := bufio.NewReader(conn).ReadString('\n')
		// Распечатываем полученое сообщение
		fmt.Println(string(message))
	}
}

func (client *Client) send(message string) {
	message = client.username + ": " + message
	for i := 0; i < len(client.addresses); i++ {
		go func(j int) {
			conn, _ := net.Dial("tcp", client.addresses[j])
			if conn == nil {return}
			conn.Write([]byte(message))
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
