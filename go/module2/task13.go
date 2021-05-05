package main
import (
    "bufio"
    "fmt"
	"os"
)

type Vertex struct {
	formula string
	lefts map[string]bool
	rights map[string]bool
	ins int 
	used bool
	visited bool
	connections []int
}

func is_in_string(c byte, s string) bool {
	for i := 0; i < len(s); i++ {
		if c == s[i] {
			return true
		}
	}
	return false
}

func is_num(c byte) bool {
	nums := "0123456789"
	return is_in_string(c, nums)
}

func is_letter(c byte) bool {
	letters := "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
	return is_in_string(c, letters)
}

// Tokens
const ( 
    ERROR = 1 << iota  // Неправильная лексема 
    NUMBER                 // Целое число 
    VAR                    // Имя переменной 
    PLUS                   // Знак + 
    MINUS                  // Знак - 
    MUL                    // Знак * 
    DIV                    // Знак / 
    LPAREN                 // Левая круглая скобка 
    RPAREN                 // Правая круглая скобка
	COMMA
	EQUAL
	BEGIN
	END
) 

func skip_space(formula string, n int) int {
	if n >= len(formula) {return n}
	for ; n < len(formula) && formula[n] == ' '; n++ {}
	if n >= len(formula) {return n}
	return n
}

func parse_number(formula string, n int) (int, bool) {
	correct := true
	if !is_num(formula[n]) { correct = false }
	for ; n < len(formula) && is_num(formula[n]); n++ {}
	return n, correct
}

func parse_varaible(formula string, n int) (int, string, bool) {
	varaible := ""
	//fmt.Println(formula, len(formula), n)
	if !is_letter(formula[n]) { return n, varaible, false }
	for ; n < len(formula) && (is_letter(formula[n]) || is_num(formula[n])); n++ {
		varaible += string(formula[n])
	}
	return n, varaible, true
}

func parse_plus(formula string, n int) (int, bool) {
	return n + 1, (formula[n] == '+')
}

func parse_minus(formula string, n int) (int, bool) {
	return n + 1, (formula[n] == '-')
}

func parse_mul(formula string, n int) (int, bool) {
	return n + 1, (formula[n] == '*')
}

func parse_div(formula string, n int) (int, bool) {
	return n + 1, (formula[n] == '/')
}

func parse_lparen(formula string, n int) (int, bool) {
	return n + 1, (formula[n] == '(')
}

func parse_rparen(formula string, n int) (int, bool) {
	return n + 1, (formula[n] == ')')
}

func parse_comma(formula string, n int) (int, bool) {
	return n + 1, (formula[n] == ',')
}

func parse_equal(formula string, n int) (int, bool) {
	return n + 1, (formula[n] == '=')
}

func tokenize(formula string) []int {
	tokens := make([]int, 0)
	tokens = append(tokens, BEGIN)
	
	for n := skip_space(formula, 0); n < len(formula); n = skip_space(formula, n){	
		//fmt.Println(n)	
		if _n, f := parse_number(formula, n); f {
			n = _n
			tokens = append(tokens, NUMBER)
		} else if _n, _, f := parse_varaible(formula, n); f {
			n = _n
			tokens = append(tokens, VAR)
		} else if _n, f := parse_plus(formula, n); f {
			n = _n
			tokens = append(tokens, PLUS)
		} else if _n, f := parse_minus(formula, n); f {
			n = _n
			tokens = append(tokens, MINUS)
		} else if _n, f := parse_mul(formula, n); f {
			n = _n
			tokens = append(tokens, MUL)
		} else if _n, f := parse_div(formula, n); f {
			n = _n
			tokens = append(tokens, DIV)
		} else if _n, f := parse_lparen(formula, n); f {
			n = _n
			tokens = append(tokens, LPAREN)
		} else if _n, f := parse_rparen(formula, n); f {
			n = _n
			tokens = append(tokens, RPAREN)
		} else if _n, f := parse_comma(formula, n); f {
			n = _n
			tokens = append(tokens, COMMA)
		} else if _n, f := parse_equal(formula, n); f {
			n = _n
			tokens = append(tokens, EQUAL)
		} else {
			tokens = append(tokens, ERROR)
			n++
		}
	}
	tokens = append(tokens, END)
	return tokens
}


func is_correct(formula string) bool {
	equal_signs := 0
	comma_balance := 0
	bracket_balance := 0
	for i := 0; i < len(formula); i++ {
		if formula[i] == '=' { equal_signs++ }
		if formula[i] == '(' { bracket_balance++ }
		if formula[i] == ')' { bracket_balance-- }
		if formula[i] == ',' {
			if equal_signs == 0 { comma_balance++ } else { comma_balance-- }
		}
	}
	if equal_signs != 1 || comma_balance != 0 || bracket_balance != 0 { return false }

	tokens := tokenize(formula)
	equal_reached := false
	//fmt.Println(tokens)
	for i := 0; i < len(tokens) - 1; i++ {
		l := tokens[i]
		r := tokens[i + 1]
		OP := MUL | DIV | PLUS | MINUS
		if l == EQUAL { equal_reached = true }
		if  l == ERROR ||
			((l & (BEGIN | COMMA | VAR | EQUAL) == 0) && !equal_reached) ||
			(l == r && l != MINUS && l != LPAREN && l != RPAREN) ||
			((l & (LPAREN | OP) != 0) && (r & (MUL | DIV | END) != 0)) ||
			((l & (MUL | DIV) != 0) && (r & (RPAREN | OP) != 0)) ||
			((l & (EQUAL) != 0) && (r & (END | RPAREN | DIV | MUL) != 0)) ||
			((l & (BEGIN | LPAREN) != 0) && (r & (EQUAL) != 0)) ||
			((l & (LPAREN) != 0) && (r & (RPAREN) != 0)) ||
			((l & (RPAREN) != 0) && (r & (LPAREN | VAR | NUMBER) != 0)) {
				return false
			}
	}
	return true
}

func skip(formula string, n int) int {
	if n >= len(formula) {return n}
	for ; n < len(formula) && !is_letter(formula[n]) && formula[n] != '='; n++ {}
	if n >= len(formula) {return n}
	if formula[n] == '=' {n++}
	return n
}

func (v Vertex) parse (formula string) (Vertex, bool) {
	syntax_correct := is_correct(formula)
	if !syntax_correct {
		return v, false
	}
	v.formula = formula
	v.lefts = make(map[string]bool)
	v.rights = make(map[string]bool)
	// Finding '='
	center_pos := 0 // pos of '='
	for ; center_pos < len(formula) && formula[center_pos] != '='; center_pos++ {}
	if center_pos == len(formula) { syntax_correct = false }
	// Lefts
	n := 0
	for ; n < center_pos; n = skip(formula, n) {
		var varaible string
		n, varaible, _ = parse_varaible(formula, n)
		if v.lefts[varaible] {
			return v, false
		}
		v.lefts[varaible] = true
	}
	// Rights
	n = skip(formula, n)
	for ; n < len(formula); n = skip(formula, n) {
		var varaible string
		n = skip(formula, n)
		n, varaible, _ = parse_varaible(formula, n)
		v.rights[varaible] = true
	}
	return v, syntax_correct
}

func dfs(graph []Vertex, v int, order *[]string, cycles *bool) {
	graph[v].used = true
	for i := 0; i < len(graph[v].connections); i++ {
		to := graph[v].connections[i]
		if !graph[to].visited && graph[to].used { *cycles = true }
		if graph[to].used { continue }
		dfs(graph, to, order, cycles)
	}
	graph[v].visited = true
	*order = append(*order, graph[v].formula)
}

func main() {
	graph := make([]Vertex, 0)
	scanner := bufio.NewScanner(os.Stdin)
	syntax_correct := true
	for scanner.Scan() {
		var v Vertex
		var _syntax_correct bool
		v, _syntax_correct = v.parse(scanner.Text())
		if !_syntax_correct {
			syntax_correct = false
		}
		graph = append(graph, v)
	}

	// Syntax error
	if !syntax_correct {
		fmt.Println("syntax error")
		return
	}
	
	// Making connections
	varaibles_mask := make(map[string]bool)
	varaibles := make(map[string]int)
	for i := 0; i < len(graph); i++ {
		for varaible := range graph[i].lefts {
			if varaibles_mask[varaible] {
				fmt.Println("syntax error")
				return
			}
			varaibles_mask[varaible] = true
			varaibles[varaible] = i
		}
	}

	for i := 0; i < len(graph); i++ {
		graph[i].connections = make([]int, 0)
		graph[i].ins = 0

		for varaible := range graph[i].rights {
			if !varaibles_mask[varaible] {
				fmt.Println("syntax error")
				return
			}
			graph[i].connections = append(graph[i].connections, varaibles[varaible])
			graph[varaibles[varaible]].ins++
		}
	}

	// Sorting
	order := make([]string, 0)
	cycles := false
	for i := 0; i < len(graph); i++ {
		if !graph[i].used && graph[i].ins == 0 {
			dfs(graph, i, &order, &cycles)
		}
	}


	if cycles || len(order)==0 {
		fmt.Println("cycle")
		return
	}

	for i := 0; i < len(order); i++ {
		fmt.Println(order[i])
	}
}