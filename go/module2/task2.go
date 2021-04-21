package main
import "fmt"
import "github.com/skorobogatov/input"


func min(a, b int) int  {
	if a < b {
		return a
	}
	return b
}

type Component struct {
	vertices int
	edges int 
	vertex_min int
	vertices_set map[int]bool
}

type Vertex struct {
	neighbors []int
	used bool
}

func dfs(v, id int, graph []Vertex, components []Component) {
	graph[v].used = true
	components[id].vertices++
	components[id].vertices_set[v] = true
	if components[id].vertex_min > v {
		components[id].vertex_min = v
	}

	for i := 0; i < len(graph[v].neighbors); i++ {
		components[id].edges++
		to := graph[v].neighbors[i]
		if graph[to].used  {
			continue
		}
		dfs(to, id, graph, components);
	}
}

func comp_components(a, b Component) bool {
	if a.vertices != b. vertices {
		return a.vertices < b.vertices
	}
	if a.edges != b. edges {
		return a.edges < b.edges
	}
	return a.vertex_min > b.vertex_min
}

func main() {
	var graph []Vertex
	var n, m int
	input.Scanf("%d %d", &n, &m)
	
	// Creating blank graph
	for i := 0; i < n; i++ {
		var ver Vertex
		ver.neighbors = make([]int, 0)
		ver.used = false
		graph = append(graph, ver)
	}

	// Graph from input
	for i := 0; i < m; i++ {
		var a, b int
		input.Scanf("%d %d", &a, &b)
		if a != b {
			graph[a].neighbors = append(graph[a].neighbors, b)
		}
		graph[b].neighbors = append(graph[b].neighbors, a)
	}

	// Coloring
	components := make([]Component, 0)
	for i, id := 0, 0; i < n; i++ {
		if !graph[i].used {
			var comp Component
			comp.vertices = 0
			comp.edges = 0
			comp.vertex_min = n
			comp.vertices_set = make(map[int]bool)
			components = append(components, comp)
			dfs(i, id, graph, components)
			id++
		}
	}

	// Finding max component
	comp_max := 0
	for i := 1; i < len(components); i++ {
		if comp_components(components[comp_max], components[i]) {
			comp_max = i
		}
	}

	// Displaying graph
	fmt.Printf("graph {\n")
	// Vertices
	for i := 0; i < len(graph); i++ {
		fmt.Printf("%d", i)
		if components[comp_max].vertices_set[i] {
			fmt.Printf(" [color = red]")
		}
		fmt.Printf("\n")
	}
	// Edges
	for i := 0; i < len(graph); i++ {
		for j := 0; j < len(graph[i].neighbors); j++ {
			to := graph[i].neighbors[j]
			if i > to {
				continue
			}
			fmt.Printf("%d -- %d", i, to)
			if components[comp_max].vertices_set[i] {
				fmt.Printf(" [color = red]")
			}
			fmt.Printf("\n")
		}
	}
	fmt.Printf("}\n")
}
