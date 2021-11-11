package main

import (
	"log" // пакет для логирования
	"fmt" // пакет для логирования
	"io/ioutil"
	"github.com/jlaffaye/ftp"
	"time"
	"bytes"
)

func list(connection *ftp.ServerConn) {
	list, err := connection.NameList("/")
	fmt.Println("Список:", list)
	if err != nil { panic(err) }
}

func main() {
	// host := "students.yss.su:21"
	// login := "ftpiu8"
	// pass := "3Ru7yOTA"
	host := "localhost:2121"
	login := "biba"
	pass := "boba"

	connection, err := ftp.Dial(host, ftp.DialWithTimeout(5*time.Second))
	if err != nil {
		log.Fatal(err)
	}

	err = connection.Login(login, pass)
	if err != nil {
		log.Fatal(err)
	}	

	// Текущтй каталог
	dir, err := connection.CurrentDir()
	fmt.Println(dir)
	if err != nil { panic(err) }

	// Загрузка файла
	fmt.Println("Загрузка файла");
	data := bytes.NewBufferString(":3")
	err = connection.Stor("test.txt", data)
	if err != nil { panic(err) }

	// Скачивание файла
	fmt.Println("Скачивание файла");
	r, err := connection.Retr("test.txt")
	if err != nil { panic(err) }
	buf, err := ioutil.ReadAll(r)
	fmt.Println(string(buf))
	r.Close()


	// Создание директории
	fmt.Println("Создание директории");
	err = connection.MakeDir("biba3000")
	if err != nil { panic(err) }

	list(connection)

	// Удаление директории
	fmt.Println("Удаление директории");
	err = connection.RemoveDir("biba3000")

	list(connection)

	if err := connection.Quit(); err != nil {
		log.Fatal(err)
	}
}