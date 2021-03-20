package main
import "fmt"
import "github.com/skorobogatov/input"

func get_expr(str string, begin int) int {
	if str[begin] != '(' {
		return begin + 1
	}
	balance := 1
	for begin++; balance > 0; begin++ {
		if str[begin] == '(' {
			balance++;
		}
		if str[begin] == ')' {
			balance--;
		}
	}
	return begin
}

func count(counted map[string]int, str string) int{
	if _, ok := counted[str]; ok {
		return 0
	}
	if str[0] == '(' {
		ln1 := get_expr(str, 2)
		ln2 := get_expr(str, ln1)
		val1 := count(counted, str[2:ln1])
		val2 := count(counted, str[ln1:ln2])
		counted[str] = 1
		return val1 + val2 + 1
	}
	return 0;
}

func main() {
	var str string
	input.Scanf("%s", &str)
	counted := make(map[string]int)
	fmt.Printf("%d\n", count(counted, str));
}
