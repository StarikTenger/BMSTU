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
	"github.com/fatih/color"
	tm "github.com/buger/goterm"
)

func makeTimestamp() int64 {
    return time.Now().UnixNano() / int64(time.Millisecond)
}

func Max(x, y int) int {
    if x < y {return y}
    return x
}

type Message struct {
	Time string
	Username string
	Text string
}

type Client struct {
	username string
	port int
	addresses []string
	messages []Message
	shown_messages int
}

func (client *Client) add_address (addr string) {
	client.addresses = append(client.addresses, addr)
}

func (client *Client) serve() {

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
		client.messages = append(client.messages, message)
		client.render(
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
		client.render()
		client.send(text)
	}
}

func (client *Client) render_message(message *Message) {
	if client.username == message.Username {
		color.New(color.FgRed).Print(message.Username + "(You)")
	} else {
		color.New(color.FgYellow).Print(message.Username)
	}
	color.New(color.FgGreen).Print("[" + message.Time + "]: ")
	color.New(color.FgWhite).Print(message.Text)
}

func (client *Client) render() {
	tm.Clear()
	tm.Flush()
	color.New(color.FgCyan).Println("Last " + strconv.Itoa(client.shown_messages) + " messages:")
	for i := Max(0, len(client.messages) - client.shown_messages); i < len(client.messages); i++ {
		message := client.messages[i]
		client.render_message(&message)
	}
	color.New(color.FgRed).Print(client.username + ": ")
}

func (client *Client) load_addresses(path string) {
	file, _ := os.Open(path)
    defer file.Close()
	scanner := bufio.NewScanner(file)
    for scanner.Scan() {
        client.add_address(scanner.Text())
    }
}

func main() {
	var client Client

	// Parsing from command line
	flag.StringVar(&client.username, "username", "username", "set username")
	flag.IntVar(&client.port, "port", 8080, "set port")
	flag.IntVar(&client.shown_messages, "messages", 20, "")
	flag.Parse()

	

	// TODO: Address list parsing
	client.load_addresses("addresses.conf")

	fmt.Println(client)

	go client.serve()
	client.render()
	client.input_processing()
}
