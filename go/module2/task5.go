package main
import "fmt"
import "github.com/skorobogatov/input"

func min(a, b int) int  {
	if a < b {
		return a
	}
	return b
}

type Vertex struct {
	neighbors []int
	dists []int
}

func bfs(graph []Vertex, root, id int) {
	queue := make([]int, 0)
	begin := 0
	queue = append(queue, root)
	graph[root].dists[id] = 0
	for ;begin < len(queue); {
		v := queue[begin]
		begin++
		//fmt.Printf("%d\n", v)
		for i := 0; i < len(graph[v].neighbors); i++ {
			to := graph[v].neighbors[i]
			if graph[to].dists[id] < graph[v].dists[id] + 1 { continue }
			graph[to].dists[id] = graph[v].dists[id] + 1
			queue = append(queue, to)
		}
	}
}

func main() {
	var graph []Vertex
	var n, m int
	input.Scanf("%d %d", &n, &m)
	
	// Creating blank graph
	for i := 0; i < n; i++ {
		var ver Vertex
		ver.neighbors = make([]int, 0)
		ver.dists = make([]int, 0)
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

	var k int
	input.Scanf("%d", &k)
	
	// Coloring
	for i := 0; i < k; i++ {
		var v int
		input.Scanf("%d", &v)
		for j := 0; j < n; j++ {
			graph[j].dists = append(graph[j].dists, n)
		}
		bfs(graph, v, i)
	}

	// Displaying
	count := 0
	for i := 0; i < n; i++ {
		equal := true
		for j := 1; j < k; j++ {
			if graph[i].dists[j] != graph[i].dists[j - 1] || graph[i].dists[j] == n {
				equal = false
				break
			}
		}
		if equal {
			//fmt.Printf("%d : len = %d\n", i, graph[i].dists[0])
			fmt.Printf("%d ", i)
			count ++
		}
	}
	if count == 0 {
		fmt.Printf("-")
	}
}
