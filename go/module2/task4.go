package main
import "fmt"
import "github.com/skorobogatov/input"

type Vertex struct {
	neighbors []int
	used bool
	in, up int
}

func min(a, b int) int  {
	if a < b {
		return a
	}
	return b
}

func dfs(v, p int, graph []Vertex, timer, count *int, bridges *map[int]bool) {
	n :=  len(graph)
	graph[v].used = true
	(*timer)++
	graph[v].in = *timer
	graph[v].up = *timer
	for i := 0; i < len(graph[v].neighbors); i++ {
		to := graph[v].neighbors[i]
		if to == p {  continue }
		if graph[to].used {
			graph[v].up = min (graph[v].up, graph[to].in)
		} else {
			dfs (to, v, graph, timer, count, bridges);
			graph[v].up = min (graph[v].up, graph[to].up);
			if graph[to].up > graph[v].in && !(*bridges)[to * n + v]{
				(*count) ++
				//fmt.Printf("bridge: %d %d\n", to, v);
				(*bridges)[to * n + v] = true
				(*bridges)[v * n + to] = true
			}
		}
	}
}

func main() {
	var graph []Vertex
	var n, m int
	input.Scanf("%d %d", &n, &m)
	
	for i := 0; i < n; i++ {
		var ver Vertex
		ver.neighbors = make([]int, 0)
		ver.used = false
		ver.in = 0
		ver.up = 0
		graph = append(graph, ver)
	}

	for i := 0; i < m; i++ {
		var a, b int
		input.Scanf("%d %d", &a, &b)
		graph[a].neighbors = append(graph[a].neighbors, b)
		graph[b].neighbors = append(graph[b].neighbors, a)
	}

	var count, timer int
	count = 0
	timer = 0
	bridges := make(map[int]bool)
	for i := 0; i < n; i++ {
		if !graph[i].used {
			dfs(i, -1, graph, &timer, &count, &bridges)
		}
	}

	fmt.Printf("%d\n", count)
}
