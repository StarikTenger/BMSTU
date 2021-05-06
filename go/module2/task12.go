package main
import "fmt"
import "github.com/skorobogatov/input"


// Priority queue implementation
import (
	"container/heap"
	"errors"
)

// PriorityQueue represents the queue
type PriorityQueue struct {
	itemHeap *itemHeap
	lookup   map[interface{}]*item
}

// New initializes an empty priority queue.
func New() PriorityQueue {
	return PriorityQueue{
		itemHeap: &itemHeap{},
		lookup:   make(map[interface{}]*item),
	}
}

// Len returns the number of elements in the queue.
func (p *PriorityQueue) Len() int {
	return p.itemHeap.Len()
}

// Insert inserts a new element into the queue. No action is performed on duplicate elements.
func (p *PriorityQueue) Insert(v interface{}, priority float64) {
	_, ok := p.lookup[v]
	if ok {
		return
	}

	newItem := &item{
		value:    v,
		priority: priority,
	}
	heap.Push(p.itemHeap, newItem)
	p.lookup[v] = newItem
}

// Pop removes the element with the highest priority from the queue and returns it.
// In case of an empty queue, an error is returned.
func (p *PriorityQueue) Pop() (interface{}, error) {
	if len(*p.itemHeap) == 0 {
		return nil, errors.New("empty queue")
	}

	item := heap.Pop(p.itemHeap).(*item)
	delete(p.lookup, item.value)
	return item.value, nil
}

type itemHeap []*item

type item struct {
	value    interface{}
	priority float64
	index    int
}

func (ih *itemHeap) Len() int {
	return len(*ih)
}

func (ih *itemHeap) Less(i, j int) bool {
	return (*ih)[i].priority < (*ih)[j].priority
}

func (ih *itemHeap) Swap(i, j int) {
	(*ih)[i], (*ih)[j] = (*ih)[j], (*ih)[i]
	(*ih)[i].index = i
	(*ih)[j].index = j
}

func (ih *itemHeap) Push(x interface{}) {
	it := x.(*item)
	it.index = len(*ih)
	*ih = append(*ih, it)
}

func (ih *itemHeap) Pop() interface{} {
	old := *ih
	item := old[len(old)-1]
	*ih = old[0 : len(old)-1]
	return item
}

type Tile struct {
	cost int
	dist int
}

type Point struct {
	x, y int
}

func main() {
	var n int
	input.Scanf("%d", &n)

	mat := make([][]Tile, n)
	for i := 0; i < n; i++ {
		mat[i] = make([]Tile, n)
		for j := 0; j < n; j++ {
			input.Scanf("%d", &mat[i][j].cost)
			mat[i][j].dist = 1000000
		}
	}

	queue := New()
	mat[0][0].dist = mat[0][0].cost
	queue.Insert(Point{0, 0}, 0)
	shifts := make([]Point, 4)
	shifts[0] = Point{0, 1}
	shifts[1] = Point{0, -1}
	shifts[2] = Point{1, 0}
	shifts[3] = Point{-1, 0}
	for ;queue.Len() > 0; {
		_point, _ := queue.Pop()
		point := _point.(Point)
		for _, shift := range shifts {
			p := Point{
				point.x + shift.x,
				point.y + shift.y,
			}
			if p.x < 0 || p.y < 0 || p.x >= n || p.y >= n {
				continue
			}
			if mat[p.x][p.y].dist > mat[point.x][point.y].dist + mat[p.x][p.y].cost {
				mat[p.x][p.y].dist = mat[point.x][point.y].dist + mat[p.x][p.y].cost
				queue.Insert(p, (float64)(mat[p.x][p.y].dist))
			}
		}
	}

	fmt.Println(mat[n - 1][n - 1].dist)
}
