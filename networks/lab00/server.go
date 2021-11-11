package main

import (
	"fmt" // пакет для форматированного ввода вывода
	"net/http" // пакет для поддержки HTTP протокола
	"strings" // пакет для работы с UTF-8 строками
	"log" // пакет для логирования
)

var state = true;

func HomeRouterHandler(w http.ResponseWriter, r *http.Request) {
	r.ParseForm() //анализ аргументов,
	fmt.Println(r.Form) // ввод информации о форме на стороне сервера
	if _, ok := r.Form["biba"]; ok {
		if r.Form["biba"][0] == "true" {
			fmt.Printf("boba");
			state = !state;
		}
	}
	fmt.Println("path", r.URL.Path)
	fmt.Println("scheme", r.URL.Scheme)
	fmt.Println(r.Form["url_long"])
	for k, v := range r.Form {
		fmt.Println("key:", k)
		fmt.Println("val:", strings.Join(v, ""))
	}

	if state {
		fmt.Fprintf(w, "<div><a href='localhost:9000?biba=true'>Биба)</a></div>") // отправляем данные на клиентскую сторону
	} else {
		fmt.Fprintf(w, "<div><a href='localhost:9000?biba=true'>Боба)</a></div>") // отправляем данные на клиентскую сторону
	}
	
}

func main() {
	http.HandleFunc("/", HomeRouterHandler) // установим роутер
	err := http.ListenAndServe(":9000", nil) // задаем слушать порт
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
} 