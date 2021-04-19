package main
import "fmt"
import "github.com/skorobogatov/input"

func gcd (a int, b int) int {
	for ; b > 0 ; {
		a %= b;
		a, b = b, a
	}
	return a;
}

func Abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

type Fraction struct {
	num, den int
}

func (a Fraction) Normalize() Fraction {
	if a.num == 0 {
		return Fraction{0, 1}
	}
	sign := 1
	if (a.num > 0) != (a.den > 0) {
		sign = -1
	}
	divider := gcd(Abs(a.num), Abs(a.den))
	return Fraction{sign * Abs(a.num) / divider, Abs(a.den) / divider}
}

func (a Fraction) Add(b Fraction) Fraction {
	a = a.Normalize()
	b = b.Normalize()
	return Fraction{a.num * b.den + b.num * a.den, a.den * b.den}.Normalize()
}

func (a Fraction) Mul(b Fraction) Fraction {
	a = a.Normalize()
	b = b.Normalize()
	return Fraction{a.num * b.num, a.den * b.den}.Normalize()
}

func (a Fraction) Reverse() Fraction {
	return Fraction{a.den, a.num}
}

func (a Fraction) Negative() Fraction {
	return Fraction{-a.num, a.den}
}

func mat_display(mat [][]Fraction) {
	for _, line := range(mat) {
		for _, el := range(line) {
			fmt.Printf("%d/%d ", el.num, el.den)
		}
		fmt.Printf("\n")
	}
}

func add_line(line, line_to_add []Fraction, modifier Fraction) {
	for i := 0; i < len(line); i++ {
		line[i] = line[i].Add(line_to_add[i].Mul(modifier))
	}
}

func mul_line(line []Fraction, a Fraction) {
	for i := 0; i < len(line); i++ {
		line[i] = line[i].Mul(a)
	}
}

func swap_lines(line_a, line_b []Fraction) {
	for i := 0; i < len(line_a); i++ {
		line_a[i], line_b[i] = line_b[i], line_a[i]
	}
}

func triangulate_matrix(mat [][]Fraction) {
	n := len(mat)
	for i := 0; i < n; i++ {
		for j := i + 1; j < n && mat[i][i].num == 0; j++ {
			swap_lines(mat[i], mat[j])
		}
		mul_line(mat[i], mat[i][i].Reverse())
		for j := i + 1; j < n; j++ {
			if mat[j][i].num == 0 {
				continue
			}
			add_line(mat[j], mat[i], mat[j][i].Negative())
		}
	}
}

func solve_matrix(mat [][]Fraction) (bool, []Fraction) {
	is_solution := true
	n := len(mat)
	solution := make([]Fraction, n)
	for i := n - 1; i >= 0; i-- {
		if  mat[i][i].num == 0 {
			is_solution = false
		}
		value := mat[i][n]
		for j := i + 1; j < n; j ++ {
			value = value.Add(mat[i][j].Negative().Mul(solution[j]))
		}
		solution[i] = value
	}
	return is_solution, solution
}

func main() {
	var n int
	fmt.Scanf("%d", &n)
	var mat [][]Fraction
	for i := 0; i < n; i++ {
        mat = append(mat, make([]Fraction, n + 1))
        for j := 0; j < n + 1; j++ {
            input.Scanf("%d", &mat[i][j].num)
			mat[i][j].den = 1
        }
    }

	triangulate_matrix(mat)
	//mat_display(mat)

	is_solution, solution := solve_matrix(mat)
	if !is_solution {
		fmt.Printf("No solution")
	} else {
		for _, el := range(solution) {
			fmt.Printf("%d/%d\n", el.num, el.den)
		}
	}
}
