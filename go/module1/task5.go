package main
import "fmt"
import "math/big"
//import "github.com/skorobogatov/input"

type M2x2 struct {
	x00, x01, 
	x10, x11 *big.Int
}

var M2x2_ident = M2x2{
	big.NewInt(1), big.NewInt(0), 
	big.NewInt(0), big.NewInt(1), 
}

func mult(a, b M2x2) M2x2{
	return M2x2{
		new(big.Int).Add(new(big.Int).Mul(a.x00, b.x00), new(big.Int).Mul(a.x01, b.x10)), 
		new(big.Int).Add(new(big.Int).Mul(a.x00, b.x01), new(big.Int).Mul(a.x01, b.x11)), 
		new(big.Int).Add(new(big.Int).Mul(a.x10, b.x00), new(big.Int).Mul(a.x11, b.x10)), 
		new(big.Int).Add(new(big.Int).Mul(a.x10, b.x01), new(big.Int).Mul(a.x11, b.x11)), 
	}
}


func binpow(a M2x2, n int) M2x2 {
	if n == 0 {
		return M2x2_ident
	}
	if n % 2 == 1 {
		return mult(binpow(a, n - 1), a)
	}
	b := binpow(a, n / 2)
	return mult(b, b)
}

func main() {
	var n int
	fmt.Scanf("%d", &n)

	m0 := M2x2{
		big.NewInt(1), big.NewInt(1), 
		big.NewInt(1), big.NewInt(0), 
	}
	
	m1 := binpow(m0, n - 1)
	fmt.Printf("%d", m1.x00)
}
