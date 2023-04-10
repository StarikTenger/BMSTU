package main

import "fmt"

func main() {
	n := 5
	a := make([]int, 10) // make([]int, 10, 20)
	a1 := make([]int, n)
	printSlice("a", a)
	printSlice("a1", a1)

	b := make([]int, 0, 10 + 5) // make([]int, 0, 2*(10+5))
	printSlice("b", b)

	c := b[:2]
	printSlice("c", c)

	d := c[2:5]
	printSlice("d", d)
}

func printSlice(s string, x []int) {
	fmt.Printf("%s len=%d cap=%d %v\n",
		s, len(x), cap(x), x)
}