package main

import (
	"github.com/skorobogatov/input"
	"fmt"
)

var parent []int
var rank []int

func make_set (v int) {
    parent[v] = v
    rank[v] = 0
}
 
func find_set (v int) int{
    if (v == parent[v]) {
        return v
    }
    parent[v] = find_set(parent[v])
    return parent[v]
}
 
func union_sets (a, b int) {
    a = find_set(a)
    b = find_set(b)
    if (a != b) {
        if (rank[a] < rank[b]) {
            c := a
            a = b
            b = c
        }
        parent[b] = a
        if (rank[a] == rank[b]) {
            rank[a]++
        }
    }
}

type Pair struct {
	to     int
	signal string
}

func split1(table [][]Pair) (int, []int){
	m := len(table)
	for i := 0; i < len(table); i++ {
		parent[i] = i
	}
	for i := 0; i < len(table); i++ {
		for j := 0; j < len(table); j++ {
			if find_set(i) != find_set(j) {
				eq := true
				for k := 0; k < len(table[0]); k++ {
					if table[i][k].signal != table[j][k].signal {
						eq = false
						break
					}
				}
				if eq {
					union_sets(i, j)
					m--
				}
			}
		}
	}
	arr := make([]int, len(table))
	for i := 0; i < len(table); i++ {
		arr[i] = find_set(i)
	}
	return m, arr
}

func split(table [][]Pair, arr []int) int {
	m := len(table)
	for i := 0; i < len(table); i++ {
		parent[i] = i
	}
	for i := 0; i < len(table); i++ {
		for j := 0; j < len(table); j++ {
			if arr[i] == arr[j] && find_set(i) != find_set(j) {
				eq := true
				for k := 0; k < len(table[0]); k++ {
					w1 := table[i][k].to
					w2 := table[j][k].to
					if arr[w1] != arr[w2] {
						eq = false
						break
					}
				}
				if eq {
					union_sets(i, j)
					m--
				}
			}
		}
	}
	for i := 0; i < len(table); i++ {
		arr[i] = find_set(i)
	}
	return m
}

func minimize(table [][]Pair) ([][]Pair, []int, []int) {
	m, arr := split1(table)
	for ;; {
		m1 := split(table, arr)
		if m == m1 {
			break
		}
		m = m1
	}

	table_new := make([][]Pair, 0)
	used := make(map[int]bool)
	
	backmapping := make([]int, len(table))
	cur := 0
	for i := 0; i < len(table); i++ {
		i1 := arr[i]
		backmapping[i] = cur
		if !used[i1] {
			used[i1] = true
			cur++
		}
	}

	cur = 0
	used = make(map[int]bool)
	for i := 0; i < len(table); i++ {
		i1 := arr[i]
		
		if !used[i1] {
			used[i1] = true
			table_new = append(table_new, make([]Pair, len(table[0])))
			for j := 0; j < len(table[0]); j++ {
				table_new[cur][j].to = backmapping[arr[table[i][j].to]]
				table_new[cur][j].signal = table[i][j].signal
			}
			cur++
		}
	}
	return table_new, backmapping, arr
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

	parent = make([]int, n)
	rank = make([]int, n)


	table_new, backmapping, arr := minimize(table)
	numeration := make([]int, n)
	antinumeration := make([]int, n)
	visited := make([]bool, n)
	count := 0
	q0 = backmapping[arr[q0]]
	dfs(q0, &count, table_new, visited, numeration, antinumeration)
	q0 = 0

	// Output
	fmt.Println("digraph { \n rankdir = LR\n dummy [label = \"\", shape = none]\n")
	// Verticies
	for i := 0; i < count; i++ {
		fmt.Println(i, "[shape = circle]")
	}
	// Entery
	fmt.Println("dummy ->", q0)
	alphabet := "abcdefghijklmnopqrstuvwxyz"
	// Connections
	for i := 0; i < count; i++ {
		for j := 0; j < len(table_new[i]); j++ {
			fmt.Printf("%d -> %d [label = \"%c(%s)\"]\n", i, antinumeration[table_new[numeration[i]][j].to], alphabet[j], table_new[numeration[i]][j].signal)
		}
	}
	fmt.Println("}")
}
