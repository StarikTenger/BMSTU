package main
import "fmt"
import "github.com/skorobogatov/input"

func main() {
	var k string
	var x1_str, x2_str string
	input.Scanf("%s\n", &k)
	input.Scanf("%s", &x1_str)
	input.Scanf("%s", &x2_str)
	x1, x2 := []rune(x1_str)[0], []rune(x2_str)[0]
	y1 := -len(k) - 1
	y2 := -len(k) - 1
	ans := len(k) + 1
	i := 0
	for _, ki := range k {
		if x1 == ki {
			y1 = i;
			if ans > y1 - y2 - 1 {
				ans = y1 - y2 - 1
			}
		}
		if x2 == ki {
			y2 = i;
			if ans > y2 - y1 - 1 {
				ans = y2 - y1
			}
		}
		i++
	}
	fmt.Println(ans)
}