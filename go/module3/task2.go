package main

import (
	"github.com/skorobogatov/input"
	"fmt"
)

type Pair struct {
	to     int
	signal string
}

func main() {
	var n, m, q0 int
	input.Scanf("%d %d %d", &n, &m, &q0)
	table := make([][]Pair, n)
	for i := 0; i < n; i++ {
		table[i] = make([]Pair, m)
	}

	// Input
	for i := 0; i < n; i++ {
		for j := 0; j < m; j ++ {
			input.Scanf("%d", &table[i][j].to)	
		}
	}
	for i := 0; i < n; i++ {
		for j := 0; j < m; j ++ {
			input.Scanf("%s", &table[i][j].signal)	
		}
	}

	// Output
	fmt.Println("digraph { \n rankdir = LR\n dummy [label = \"\", shape = none]\n")
	// Verticies
	for i := 0; i < n; i++ {
		fmt.Println(i, "[shape = circle]")
	}
	// Entery
	fmt.Println("dummy ->", q0)
	alphabet := "abcdefghijklmnopqrstuvwxyz"
	// Connections
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			fmt.Printf("%d -> %d [label = \"%c(%s)\"]\n", i, table[i][j].to, alphabet[j], table[i][j].signal)
		}
	}
	fmt.Println("}")
}
