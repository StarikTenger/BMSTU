package main
import "fmt"
//import "github.com/skorobogatov/input"

type Tag int 
const ( 
    ERROR Tag = 1 << iota  // Неправильная лексема 
    NUMBER                 // Целое число 
    VAR                    // Имя переменной 
    PLUS                   // Знак + 
    MINUS                  // Знак - 
    MUL                    // Знак * 
    DIV                    // Знак / 
    LPAREN                 // Левая круглая скобка 
    RPAREN                 // Правая круглая скобка 
)

type Lexem struct { 
    tag Tag 
    image string 
}

var numbers = map[byte]bool{'0':1,'1':1,'2':1,'3':1,'4':1,'5':1,'6':1,'7':1,'8':1,'9':1,}

func read_lexem(str string) (Lexem, int) {
	size := 1
	var lexem Lexem
	switch str[i] {
	case "+":
		lexem.tag = PLUS
	case "-":
		lexem.tag = MINUS
	case "*":
		lexem.tag = MUL
	case "/":
		lexem.tag = DIV
	case "(":
		lexem.tag = LPAREN
	case ")":
		lexem.tag = RPAREN
	default:
		if numbers[str[i]] {

		}
	}

	lexem.int = str[0:size]
	return lexem, size
}

func main() {
	
}
