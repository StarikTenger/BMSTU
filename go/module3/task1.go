package main

import (
	"github.com/skorobogatov/input"
	"fmt"
)

type Pair struct {
	to     int
	signal string
}

func dfs(v int, count *int, table [][]Pair, visited []bool, numeration []int, antinumeration []int) {
	visited[v] = true
	numeration[ *count] = v
	antinumeration[v] = *count
	(*count)++
	for i := 0; i < len(table[v]); i++ {
		to := table[v][i].to
		if visited[to] { continue }
		dfs(to, count, table, visited, numeration, antinumeration)
	}
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
	numeration := make([]int, n)
	antinumeration := make([]int, n)
	visited := make([]bool, n)
	count := 0
	dfs(q0, &count, table, visited, numeration, antinumeration)

	// Output
	n = count
	fmt.Println(n)
	fmt.Println(m)
	fmt.Println(0)
	for i := 0; i < n; i++ {
		for j := 0; j < m; j ++ {
			fmt.Printf("%d ", antinumeration[table[numeration[i]][j].to])
		}
		fmt.Printf("\n")
	}
	for i := 0; i < n; i++ {
		for j := 0; j < m; j ++ {
			fmt.Printf("%s ", table[numeration[i]][j].signal)
		}
		fmt.Printf("\n")
	}
}
