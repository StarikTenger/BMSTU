package main
import "fmt"
import "sort"
import "strconv"
import "github.com/skorobogatov/input"

type Vertex struct {
	neighbors []int
	group int
	used bool
}

type Component struct {
	balance int
	vertices_left map[int]bool
	vertices_right map[int]bool
}

func min(a, b int) int  {
	if a < b {
		return a
	}
	return b
}

func dfs(v int, graph []Vertex, component *Component, possibility *bool) {
	graph[v].used = true
	if graph[v].group == -1 {
		(*component).vertices_left[v] = true
	} else {
		(*component).vertices_right[v] = true
	}
	(*component).balance += graph[v].group
	for i := 0; i < len(graph[v].neighbors); i++ {
		to := graph[v].neighbors[i]
		if graph[v].group * graph[to].group == 1 {
			*possibility = false
			return
		}
		if graph[to].used {  continue }
		graph[to].group = -graph[v].group;
		dfs (to, graph, component, possibility);		
	}
}

// Generates all possible masks of lenth n
func gen_masks(n, n_cur int, mask []bool, masks *[][]bool) {
	if n_cur == n {
		*masks = append(*masks, mask)
		return
	}
	mask_copy_true := make([]bool, len(mask))
	mask_copy_false := make([]bool, len(mask))
	copy(mask_copy_true, mask)
	copy(mask_copy_false, mask)
	mask_copy_true = append(mask_copy_true, true)
	mask_copy_false = append(mask_copy_false, false)
	gen_masks(n, n_cur + 1, mask_copy_true, masks)
	gen_masks(n, n_cur + 1, mask_copy_false, masks)
}

func comp(arr_a, arr_b []int) bool {
	if len(arr_a) < len(arr_b) { return true }
	if len(arr_a) > len(arr_b) { return false }
	for i := 0; i < len(arr_a); i++ {
		if arr_a[i] < arr_b[i] { return true }
		if arr_a[i] > arr_b[i] { return false }
	}
	return false
}

func main() {
	var graph []Vertex
	var n int
	input.Scanf("%d", &n)
	
	// Init vertices
	for i := 0; i < n; i++ {
		var ver Vertex
		ver.neighbors = make([]int, 0)
		ver.group = 0
		ver.used = false
		graph = append(graph, ver)
	}

	// Connections
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			var s string
			input.Scanf("%s", &s)
			if s[0] == '+' {
				graph[i].neighbors = append(graph[i].neighbors, j)
			}
		}
	}

	// Coloring
	components := make([]Component, 0)
	possibility := true
	for i := 0; i < n; i++ {
		if !graph[i].used {
			var component Component
			component.balance = 0
			component.vertices_left = make(map[int]bool)
			component.vertices_right = make(map[int]bool)
			graph[i].group = 1
			dfs(i, graph, &component, &possibility)
			//fmt.Println(component)
			components = append(components, component)
		}
	}

	if !possibility {
		fmt.Println("No solution")
		return
	}

	// Generating masks
	masks := make([][]bool, 0)
	mask := make([]bool, 0)
	gen_masks(len(components), 0, mask, &masks)
	
	// Finding minimum
	mask_sums := make([]int, len(masks))
	sum_min := n
	for i := 0; i < len(masks); i++ {
		sum := 0
		for j := 0; j < len(components); j++ {
			mod := 1
			if !masks[i][j] { mod = -1 }
			sum += components[j].balance * mod
		}
		if sum < 0 { sum = -sum }
		mask_sums[i] = sum
		if sum < sum_min {sum_min = sum}
	}

	// Finding appropriate components
	arr_min := make([]int, 0)
	for i := 0; i < len(masks); i++ {
		if mask_sums[i] == sum_min {
			set := make(map[int]bool)
			// Merging sets
			for j := 0; j < len(components); j++ {
				subset := components[j].vertices_right
				if masks[i][j] {
					subset = components[j].vertices_left
				}
				for key, value := range subset {
					set[key] = value
				}
			}
			
			// Making string
			keys := make([]int, len(set))
			i := 0
			for k, _ := range set {
				keys[i] = k
				i++
			}
			sort.Ints(keys)
			
			if len(arr_min) == 0 || comp(keys, arr_min) {
				arr_min = keys
			}
		}
	}
	str := ""
	for _, vertex := range arr_min {
		str += strconv.Itoa(vertex + 1) + " "
	}
	fmt.Println(str)
}
