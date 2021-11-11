package main

import (
	"html/template"
	"log"
	"net/http"
	"fmt"
	"golang.org/x/net/html"
)

type Item struct {
	Ref, Time, Title string
}

func getChildren(node *html.Node) []*html.Node {
	var children []*html.Node
	for c := node.FirstChild; c != nil; c = c.NextSibling {
		children = append(children, c)
	}
	return children
}

func getAttr(node *html.Node, key string) string {
	for _, attr := range node.Attr {
		if attr.Key == key {
			return attr.Val
		}
	}
	return ""
}

func isText(node *html.Node) bool {
	return node != nil && node.Type == html.TextNode
}

func isElem(node *html.Node, tag string) bool {
	return node != nil && node.Type == html.ElementNode && node.Data == tag
}

func isSection(node *html.Node, class string) bool {
	return isElem(node, "section") && getAttr(node, "class") == class
}

func isA(node *html.Node, class string) bool {
	return isElem(node, "a") && getAttr(node, "class") == class
}

func isDiv(node *html.Node, class string) bool {
	return isElem(node, "div") && getAttr(node, "class") == class
}

func readItem(node *html.Node) *Item {
	// cs := getChildren(node)
	path := "http://jolt.law.harvard.edu"
	fmt.Println(getAttr(node.Parent.Parent.Parent, "class"))
	for c := node.Parent.Parent.Parent.FirstChild; c != nil; c = c.NextSibling {
		fmt.Println("-- ", getAttr(c, "class"))
		if (getAttr(c, "class") == "digest__home__post__image") {
			fmt.Println("++ ", getAttr(c.FirstChild.NextSibling, "src"))
			path += getAttr(c.FirstChild.NextSibling, "src")
		}
	}
	// fmt.Println(cs)
	return &Item{
		Ref:   getAttr(node, "href"),
		Time:  path, //getAttr(cs[0], "title"),
		Title: node.FirstChild.Data,
	}
}

func downloadNews() []*Item {
	log.Println("sending request")
	if response, err := http.Get("http://jolt.law.harvard.edu/digest"); err != nil {
		log.Println("request failed", "error", err)
	} else {
		defer response.Body.Close()
		status := response.StatusCode
		log.Println("got response", "status", status)
		if status == http.StatusOK {
			if doc, err := html.Parse(response.Body); err != nil {
				log.Println("invalid HTM", "error", err)
			} else {
				log.Println("HTML parsed successfully")
				return search(doc)
			}
		}
	}
	return nil
}

func search(node *html.Node) []*Item {
	var res []*Item
	
	if isA(node, "digest__home__post__title") {
		// fmt.Println(getAttr(node, "href"))
		var items []*Item
		if item := readItem(node); item != nil {
			items = append(items, item)
		}
		// for c := node.FirstChild; c != nil; c = c.NextSibling {
		// 	fmt.Println("--child")
		// 	title := c.FirstChild
		// 	fmt.Println(title)
		// 	if isDiv(c, "item") {
		// 		if item := readItem(c); item != nil {
		// 			items = append(items, item)
		// 		}
		// 	}
		// }
		res = append(res, items...)
	}
	for c := node.FirstChild; c != nil; c = c.NextSibling {
		if items := search(c); items != nil {
			res = append(res, items...)
		}
	}
	return res
}

func HomeRouterHandler(writer http.ResponseWriter, _ *http.Request) {
	items := downloadNews()
	fmt.Println(items)
	if pageTemplate, err := template.ParseFiles("./index.html"); err != nil {
		log.Fatal(err)
	} else {
		pageTemplate.Execute(writer, items)
	}
}


func main() {
	http.HandleFunc("/", HomeRouterHandler)
	err := http.ListenAndServe(":3000", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}