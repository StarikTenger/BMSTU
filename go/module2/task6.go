package main
import "fmt"
import "math"
import "github.com/skorobogatov/input"

type Point struct {
	x, y float64
}

func main() {
	var n int
	input.Scanf("%d", &n)

	// Points from input
	points := make([]Point, n)
	for i := 0; i < n; i++ {
		var x, y float64
		input.Scanf("%f %f", &x, &y)
		points[i] = Point{x, y}
	}

	// Graph from points
	graph := make([][]float64, n)
	for i := 0; i < n; i++ {
		graph[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			dx := points[i].x - points[j].x
			dy := points[i].y - points[j].y
			graph[i][j] = math.Sqrt(dx*dx + dy*dy)
		}	
	}

	// 
	used := make([]bool, n)
	min_e := make([]float64, n)
	sel_e := make([]int, n)
	sum := 0.
	INF := 1e10
	for i := 0; i < n; i++ {
		min_e[i] = INF
		sel_e[i] = -1
	}
	min_e[0] = 0
	for i := 0; i < n; i++ {
		v := -1
		for j := 0; j < n; j++ {
			if !used[j] && (v == -1 || min_e[j] < min_e[v]) {
				v = j
			}
		}
		sum += min_e[v]
	 
		used[v] = true;
	 
		for to := 0; to < n; to++ {
			if graph[v][to] < min_e[to] {
				min_e[to] = graph[v][to];
				sel_e[to] = v;
			}
		}
	}


	fmt.Printf("%.2f", math.Round(sum * 100) / 100.)
}
