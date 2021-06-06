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

func minimize_raw(table [][]Pair) ([][]Pair, []int, []int) {
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

func minimize(table [][]Pair, q0 int) [][]Pair {
	n := len(table)
	parent = make([]int, n)
	rank = make([]int, n)

	table_new, backmapping, arr := minimize_raw(table)
	numeration := make([]int, n)
	antinumeration := make([]int, n)
	visited := make([]bool, n)
	count := 0
	q0 = backmapping[arr[q0]]
	dfs(q0, &count, table_new, visited, numeration, antinumeration)
	q0 = 0

	table_can := make([][]Pair, count)
	for i := 0; i < len(table_can); i++ {
		table_can[i] = make([]Pair, len(table[0]))
	}

	for i := 0; i < len(table_can); i++ {
		for j := 0; j < len(table_can[i]); j++ {
			table_can[i][j].to = antinumeration[table_new[numeration[i]][j].to]
			table_can[i][j].signal = table_new[numeration[i]][j].signal
		}
	}

	return table_can
}

func from_input() ([][]Pair, int) {
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
	return table, q0
}

func print(table [][]Pair, q0 int) {
	count := len(table)
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
		for j := 0; j < len(table[i]); j++ {
			fmt.Printf("%d -> %d [label = \"%c(%s)\"]\n", i, table[i][j].to, alphabet[j], table[i][j].signal)
		}
	}
	fmt.Println("}\n")
}

func comp(table_a, table_b [][]Pair, qa, qb int) bool {
	table_a = minimize(table_a, qa)
	table_b = minimize(table_b, qb)

	//print(table_a, 0)
	//print(table_b, 0)

	if len(table_a) != len(table_b) || len(table_a[0]) != len(table_b[0]) {
		return false
	}
	for i := 0; i < len(table_a); i++ {
		for j := 0; j < len(table_a[i]); j++ {
			if table_a[i][j].to != table_b[i][j].to || table_a[i][j].signal != table_b[i][j].signal {
				return false
			}
		}
	}

	return true
}

func main() {
	table_a, qa := from_input()
	table_b, qb := from_input()
	
	if comp(table_a, table_b, qa, qb) {
		fmt.Println("EQUAL")
	} else {
		fmt.Println("NOT EQUAL")
	}
}
