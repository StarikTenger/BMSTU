package main
import "fmt"
//import "math"

var primes []int
var visited map[int]bool
var vertices []int

func is_prime(n int) bool {
	counter := 0
	for i := 0; primes[i] * primes[i] <= n; i++ {
		if n % primes[i] == 0 {
			counter++
		}
	}
	return counter == 0
}

func dividers(n int) {
	if visited[n] {
		return
	}
	visited[n] = true
	vertices = append(vertices, n)
	count := 0
	var divs []int
	for i := 1; i * i <= n ; i++ {		
		if n % i == 0 {
			divs = append(divs, i)
			divs = append(divs, n / i)
		}
	}
	visited_local := make(map[int]bool)
	for i := 0; i < len(divs); i++ {
		if !is_prime(divs[i]) || n / divs[i] == n || visited_local[n / divs[i]]{
			continue
		}
		
		fmt.Printf("%d -- %d\n", n, n / divs[i])
		count++
		dividers(n / divs[i])
		visited_local[n / divs[i]] = true
	}	

	if count == 0 && n != 1 {
		fmt.Printf("%d -- %d\n", n, 1)
	}
}

func main() {
	// Eratosthenes
	limit := 1000000
	seive := make([]bool, limit)
	for i := 2; i < limit; i++ {
		if !seive[i] {
			primes = append(primes, i)
            for j := i; j < limit; j += i  {
                seive[j] = true
            }
        }	
	}

	visited = make(map[int]bool)

	n := 0
	fmt.Scanf("%d", &n)

	fmt.Printf("graph {\n")
	
	dividers(n)
	for i := 0; i < len(vertices); i++ {
		fmt.Printf("%d\n", vertices[i])
	} 
	fmt.Printf("}")
}
