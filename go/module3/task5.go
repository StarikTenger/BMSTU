package main

import (
	"github.com/skorobogatov/input"
	"fmt"
)

type Pair struct {
	to     int
	out int
}


func main() {
	// Input
	// In alphabet
	var in_alphabet_count int
	input.Scanf("%d", &in_alphabet_count)
	in_alphabet := make([]string, in_alphabet_count)
	for i := 0; i < in_alphabet_count; i++ {
		input.Scanf("%s", &in_alphabet[i])
	}

	// Out alphabet
	var out_alphabet_count int
	input.Scanf("%d", &out_alphabet_count)
	out_alphabet := make([]string, out_alphabet_count)
	out_alphabet_mapping := make(map[string]int)
	for i := 0; i < out_alphabet_count; i++ {
		input.Scanf("%s", &out_alphabet[i])
		out_alphabet_mapping[out_alphabet[i]] = i
	}

	var n, m int
	m = in_alphabet_count
	input.Scanf("%d", &n)
	table := make([][]Pair, n)
	for i := 0; i < n; i++ {
		table[i] = make([]Pair, m)
	}

	for i := 0; i < n; i++ {
		for j := 0; j < m; j ++ {
			input.Scanf("%d", &table[i][j].to)	
		}
	}
	for i := 0; i < n; i++ {
		for j := 0; j < m; j ++ {
			var symbol string
			input.Scanf("%s", &symbol)
			table[i][j].out = out_alphabet_mapping[symbol]
		}
	}

	
	moore_inputs := make([]map[int]bool, len(table))
	out_mapping := make([][]int, 0)
	for i := 0; i < len(table); i++ {
		out_mapping = append(out_mapping, make([]int, len(table[i])))
		moore_inputs[i] = make(map[int]bool)
	}
	for i := 0; i < len(table); i++ {
		for j := 0; j < len(table[i]); j++ {
			moore_inputs[i][table[i][j].out] = true
		}
	}

	// Splitting
	moore_table := make([][]int, 0)
	moore_outs := make([]int, 0)
	moore_mapping := make([]int, 0)
	moore_backmapping := make(map[int]map[int]int)
	moore_count := 0
	for i := 0; i < len(table); i++ {
		moore_backmapping[i] = make(map[int]int)
		for out := range out_alphabet {
			moore_backmapping[i][out] = moore_count
			moore_table = append(moore_table, make([]int, len(table[i])))
			moore_outs = append(moore_outs, out)
			moore_mapping = append(moore_mapping, i)
			for j := 0; j < len(table[i]); j++ {
				moore_table[moore_count][j] = table[i][j].to
				out_mapping[table[i][j].to][out] = moore_count
			}
			moore_count++
		}
	}


	was := make(map[int]bool)
	for i := 0; i < len(table); i++ {
		for j := 0; j < len(table[i]); j++ {
			v_new := moore_backmapping[table[i][j].to][table[i][j].out]
			was[v_new] = true
			for v, _ := range moore_backmapping[i] {
				moore_table[moore_backmapping[i][v]][j] = v_new
			}
		}
	}

	/*fmt.Println(table)
	fmt.Println(moore_table)
	fmt.Println(moore_outs)
	fmt.Println(moore_mapping)
	fmt.Println(moore_backmapping)*/

	fmt.Println("digraph {\nrankdir = LR")
	for i := 0; i < moore_count; i++ {
		if !was[i] {
			continue
		}
		fmt.Printf("%d [label = \"(%d,%s)\"]\n", i, moore_mapping[i], out_alphabet[moore_outs[i]])
		for j := 0; j < len(moore_table[i]); j++ {
			fmt.Printf("%d -> %d [label = \"%s\"]\n", i, moore_table[i][j], in_alphabet[j])
		}
	}


	
	fmt.Println("}")
}
