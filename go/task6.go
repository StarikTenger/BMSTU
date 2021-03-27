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
    Tag 
    Image string 
}



func main() {
	
}
