package main

import (
	"fmt" // пакет для форматированного ввода вывода
	"net/http" // пакет для поддержки HTTP протокола
	"strings" // пакет для работы с UTF-8 строками
	"log" // пакет для логирования
	"github.com/IzeBerg/rss-parser-go"
)

func forwardRSS(w http.ResponseWriter) {
	rssObject, _ := rss.ParseRSS("http://rss.cnn.com/rss/cnn_topstories.rss")

	// fmt.Fprintf(w, "Title : %s\n", rssObject.Channel.Title)
	// fmt.Fprintf(w, "Generator : %s\n", rssObject.Channel.Generator)
	// fmt.Fprintf(w, "PubDate : %s\n", rssObject.Channel.PubDate)
	// fmt.Fprintf(w, "LastBuildDate : %s\n", rssObject.Channel.LastBuildDate)
	// fmt.Fprintf(w, "Description : %s\n", rssObject.Channel.Description)
	// fmt.Fprintf(w, "Number of Items : %d\n", len(rssObject.Channel.Items))

	var content = "";

	for v := range rssObject.Channel.Items {
		item := rssObject.Channel.Items[v]
		content += "Item Number: " + string(v) + "<br/>"
		content += "Title: " + item.Title + "<br/>"
		content += "Link: <a href=" + item.Link + ">"+item.Description+"</a>" + "<br/>"
		content += "Guid: " + item.Guid.Value + "<br/>"
	}

	fmt.Fprintf(w, "<div>" + content + "</div>")
}

func HomeRouterHandler(w http.ResponseWriter, r *http.Request) {
	r.ParseForm() //анализ аргументов,
	fmt.Println(r.Form) // ввод информации о форме на стороне сервера

	fmt.Println("path", r.URL.Path)
	fmt.Println("scheme", r.URL.Scheme)
	fmt.Println(r.Form["url_long"])
	for k, v := range r.Form {
		fmt.Println("key:", k)
		fmt.Println("val:", strings.Join(v, ""))
	}

	forwardRSS(w);

	//fmt.Fprintf(w, "<div><a href='localhost:9000?biba=true'>Биба)</a></div>") // отправляем данные на клиентскую сторону
	
}

func main() {
	http.HandleFunc("/", HomeRouterHandler) // установим роутер
	err := http.ListenAndServe(":9000", nil) // задаем слушать порт
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
} 