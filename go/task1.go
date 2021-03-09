package main
import "fmt"

func main() {
	n := 0
	fmt.Scanf("%d", &n)
	n++
	digits := 1
	biba := 0
	for n_new, i, k := n, 1, 9;;
	i, k = i + 1, k * 10 {
		if n_new - i * k <= 0 {
			break;
		}
		n_new -= i * k;
		digits++
		n = n_new
		biba += k
	}
	number := biba + (n +  digits- 1) / digits
	digit := (n +  digits- 1) % digits
	//fmt.Printf("digits: %d\n n: %d\nnumber: %d\ndigit: %d\n", digits, n, number, digit);
	digit = digits - digit - 1
	for i := 0; i < digit; i++ {
		number /= 10
	}
	fmt.Printf("%d", number%10)
}
