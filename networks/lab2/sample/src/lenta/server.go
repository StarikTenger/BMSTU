package main

import (
	"html/template"
	"github.com/mgutz/logxi/v1"
	"golang.org/x/net/html"
	"net/http"
)

func getAttr(node *html.Node, key string) string {
	for _, attr := range node.Attr {
		if attr.Key == key {
			return attr.Val
		}
	}
	return ""
}

func getChildren(node *html.Node) []*html.Node {
	var children []*html.Node
	for c := node.FirstChild; c != nil; c = c.NextSibling {
		children = append(children, c)
	}
	return children
}

func isElem(node *html.Node, tag string) bool {
	return node != nil && node.Type == html.ElementNode && node.Data == tag
}

func isText(node *html.Node) bool {
	return node != nil && node.Type == html.TextNode
}

func isDiv(node *html.Node, class string) bool {
	return isElem(node, "div") && getAttr(node, "class") == class
}

type Item struct {
	Ref, Time, Title string
}

func readItem(item *html.Node) *Item {
	if a := item.FirstChild; isElem(a, "a") {
		if cs := getChildren(a); len(cs) == 2 && isElem(cs[0], "time") && isText(cs[1]) {
			return &Item{
				Ref:   getAttr(a, "href"),
				Time:  getAttr(cs[0], "title"),
				Title: cs[1].Data,
			}
		}
	}
	return nil
}

func search(node *html.Node) []*Item {
	if isDiv(node, "b-yellow-box__wrap") {
		var items []*Item
		for c := node.FirstChild; c != nil; c = c.NextSibling {
			if isDiv(c, "item") {
				if item := readItem(c); item != nil {
					items = append(items, item)
				}
			}
		}
		return items
	}
	for c := node.FirstChild; c != nil; c = c.NextSibling {
		if items := search(c); items != nil {
			return items
		}
	}
	return nil
}

func downloadNews() []*Item {
	log.Info("sending request to lenta.ru")
	if response, err := http.Get("http://lenta.ru"); err != nil {
		log.Error("request to lenta.ru failed", "error", err)
	} else {
		defer response.Body.Close()
		status := response.StatusCode
		log.Info("got response from lenta.ru", "status", status)
		if status == http.StatusOK {
			if doc, err := html.Parse(response.Body); err != nil {
				log.Error("invalid HTML from lenta.ru", "error", err)
			} else {
				log.Info("HTML from lenta.ru parsed successfully")
				return search(doc)
			}
		}
	}
	return nil
}


const INDEX_HTML = `
    <!doctype html>
    <html lang="ru">
        <head>
            <meta charset="utf-8">
            <title>Последние новости с lenta.ru</title>
        </head>
        <body>
            {{if .}}
                {{range .}}
                    {{.Time}}
                    <a href="http://lenta.ru/{{.Ref}}">{{.Title}}</a>
                    <br/>
                {{end}}
            {{else}}
                Не удалось загрузить новости!
            {{end}}
        </body>
    </html>
    `

var indexHtml = template.Must(template.New("index").Parse(INDEX_HTML))

func serveClient(response http.ResponseWriter, request *http.Request) {
	path := request.URL.Path
	log.Info("got request", "Method", request.Method, "Path", path)
	if path != "/" && path != "/index.html" {
		log.Error("invalid path", "Path", path)
		response.WriteHeader(http.StatusNotFound)
	} else if err := indexHtml.Execute(response, downloadNews()); err != nil {
		log.Error("HTML creation failed", "error", err)
	} else {
		log.Info("response sent to client successfully")
	}
}

func main() {
	http.HandleFunc("/", serveClient)
	log.Info("starting listener")
	log.Error("listener failed", "error", http.ListenAndServe("127.0.0.1:6060", nil))
}
