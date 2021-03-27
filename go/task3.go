package main
import "fmt"
import "github.com/skorobogatov/input"

func abs(val int) int {
	if val < 0 {
		val = -val
	}
	return val
}

func main() {
	var str string
	var symb_a_str, symb_b_str string
	input.Scanf("%s\n", &str)
	input.Scanf("%s", &symb_a_str)
	input.Scanf("%s", &symb_b_str)
	symb_a, symb_b := []rune(symb_a_str)[0], []rune(symb_b_str)[0]

	pos_a, pos_b := -1, -1
	min_dist := len(str)
 
	i := 0 
	var symbol rune
	for  _, symbol = range str {
		i++
		if symbol == symb_a {
			pos_a = i
		} else if symbol == symb_b {
			pos_b = i
		} else {
			continue
		}
		if pos_a == -1 || pos_b == -1 {
			continue
		}
		if (abs(pos_a - pos_b) < min_dist) {
			min_dist =abs(pos_a - pos_b)
		}
	}

	fmt.Printf("%d", min_dist - 1)
}
