package main
import (
    "bufio"
    "fmt"
	"os"
	"strconv"
)

type Vertex struct {
	connections []int
	connections_back []int
	connections_used map[int]bool
	weight int
	name string
	dist int
	from []int
	ins int
	used bool
	visited bool
	blue bool
	red bool
}

func skip_space(formula string, n int) int {
	if n >= len(formula) {return n}
	for ; n < len(formula) && (formula[n] == ' ' || formula[n] == '<'); n++ {}
	if n >= len(formula) {return n}
	return n
}

func parse_vertex(formula string, n int) (string, int, int) { // (text, weight, n)
	text := ""
	for ; n < len(formula) && formula[n] != '('  && formula[n] != ' ' && formula[n] != ';'; n++ {
		text += string(formula[n])
	}
	if n < len(formula) && formula[n] != '(' {
		return text, -1, n
	}
	n++
	num := ""
	for ; n < len(formula) && formula[n] != ')'; n++ {
		num += string(formula[n])
	}
	n++
	i, _ := strconv.Atoi(num)
	return text, i, n
}

func refresh(graph []Vertex) {
	for i := 0; i < len(graph); i++ {
		graph[i].used = false
		graph[i].visited = false
		graph[i].dist = graph[i].weight
		graph[i].from = make([]int, 0)
	}
}

func find_blue(graph []Vertex, v int) {
	graph[v].used = true
	for i := 0; i < len(graph[v].connections); i++ {
		to := graph[v].connections[i]
		//fmt.Println(v, to)
		if !graph[to].visited && graph[to].used || to == v { graph[to].blue = true }
		if graph[to].used { continue }
		find_blue(graph, to)
	}
	graph[v].visited = true
}

func color_blue(graph []Vertex, v int) {
	graph[v].blue = true
	for i := 0; i < len(graph[v].connections); i++ {
		to := graph[v].connections[i]
		if graph[to].blue { continue }
		color_blue(graph, to)
	}
}

func color_red(graph []Vertex, v int) {
	graph[v].red = true
	for i := 0; i < len(graph[v].from); i++ {
		from := graph[v].from[i]
		if graph[from].red { continue }
		color_red(graph, from)
	}
}

func topsort(graph []Vertex, v int, order *[]int) {
	graph[v].used = true
	for i := 0; i < len(graph[v].connections); i++ {
		to := graph[v].connections[i]
		if graph[to].used || graph[to].blue { continue }
		topsort(graph, to, order)
	}
	graph[v].visited = true
	*order = append(*order, v)
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	source := ""
	for scanner.Scan() {
		source += scanner.Text()
	}

	// Mapping names to indexes
	names := make(map[string]int)
	// Graph
	graph := make([]Vertex, 0)

	// Parsing
	for n := 0; n < len(source); n++ {
		for prev := -1; n < len(source) && source[n] != ';';{
			n = skip_space(source, n)
			var name string
			var weight int
			name, weight, n = parse_vertex(source, n)
			id := len(graph)
			if _id, ok := names[name]; ok {
				id = _id
			} else {
				var v Vertex
				v.connections = make([]int, 0)
				v.connections_used = make(map[int]bool)
				v.weight = weight
				v.name = name
				v.ins = 0
				names[name] = id
				graph = append(graph, v)
			}
			if prev != -1 && !graph[prev].connections_used[id] {
				graph[prev].connections_used[id] = true
				graph[prev].connections = append(graph[prev].connections, id)
				graph[id].connections_back = append(graph[id].connections_back, prev)
				graph[id].ins++
			}
			prev = id
		}
	}

	// Find blue
	for i := 0; i < len(graph); i++ {
		if !graph[i].used {
			find_blue(graph, i)
		}
	}

	// Color blue
	for i := 0; i < len(graph); i++ {
		if graph[i].blue {
			color_blue(graph, i)
		}
	}

	// Topsort
	refresh(graph)
	order := make([]int, 0)
	for i := 0; i < len(graph); i++ {
		if !graph[i].used && graph[i].ins == 0 {
			topsort(graph, i, &order)
		}
	}

	// Reverse order
	for i := 0; i * 2 < len(order); i++ {
		order[i], order[len(order) - i - 1] = order[len(order) - i - 1], order[i]
	}

	// Counting distances
	max_dist := 0
	max_v := make([]int, 0)
	for i := 0; i < len(order); i++ {
		v := order[i]
		//fmt.Println(graph[v].name)
		dist := 0
		for j := 0; j < len(graph[v].connections_back); j++ {
			from := graph[v].connections_back[j]
			if graph[from].dist > dist {
				dist = graph[from].dist
			}
		}
		for j := 0; j < len(graph[v].connections_back); j++ {
			from := graph[v].connections_back[j]
			if graph[from].dist == dist {
				graph[v].from = append(graph[v].from, from)
			}
		}
		graph[v].dist += dist
		if graph[v].dist >= max_dist {
			max_dist = graph[v].dist
		}
	}

	for i := 0; i < len(order); i++ {
		v := order[i]
		if graph[v].dist == max_dist {
			max_v = append(max_v, v)
		}
	}

	// Restoring path
	for _, _v := range max_v {
		color_red(graph, _v)
	}


	// Output
	fmt.Println("digraph {")
	for i := 0; i < len(graph); i++ {
		fmt.Printf("%s [label = \"%s(%d)\"", graph[i].name, graph[i].name, graph[i].weight)
		if graph[i].blue {
			fmt.Printf(", color = blue ")
		}
		if graph[i].red {
			fmt.Printf(", color = red ")
		}
		fmt.Printf("]\n")
	}
	for i := 0; i < len(graph); i++ {
		for j := 0; j < len(graph[i].connections); j++ {
			fmt.Printf("%s -> %s", graph[i].name, graph[graph[i].connections[j]].name)
			if graph[i].blue {
				fmt.Printf(" [color = blue]")
			}
			find := false
			for _, from := range graph[graph[i].connections[j]].from {
				if from == i {
					find = true
				}
			}
			if len(graph[i].connections) == 0 && len(graph[i].from) == 0 ||
				graph[i].red && graph[graph[i].connections[j]].red && 
				find {
				fmt.Printf(" [color = red]")
			}
			fmt.Printf("\n")
		}
	}
	fmt.Println("}")
}
