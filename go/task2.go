package main
import "fmt"

func partition(left int, right int,
		less func(i, j int) bool, 
		swap func(i, j int)) int {
	border := left
	for i := left + 1; i < right; i++ {
		if (less(i, border)) {
			if i != border + 1 {
				swap(border, border + 1)
			}
			swap(i, border)
			border++
		}
	}
	return border
}

func qsort(left int, right int,
		less func(i, j int) bool, 
		swap func(i, j int)) { 
	if left < right {
		pi := partition(left, right, less, swap)
		qsort(left, pi, less, swap)
		qsort(pi + 1, right, less, swap)
	}
} 

func concat(a, b int) int {
	digits := 1
	for _a := b; _a > 0; _a /= 10{
		digits *= 10
	}
	return a * digits + b
}

func main() {
	var n int
	fmt.Scanf("%d %d", &n);

	// Input
	arr := make([]int, n);
	for i := 0; i < n; i++ {
		fmt.Scanf("%d", &arr[i]);
	}

  	less := func(i, j int) bool {
		return concat(arr[i], arr[j]) > concat(arr[j], arr[i])
	}

	swap := func(i, j int) {
		arr[i], arr[j] = arr[j], arr[i]
	}

	qsort(0, n, less, swap)

	for i := 0; i < n; i++ {
		fmt.Printf("%d", arr[i])
	}
}