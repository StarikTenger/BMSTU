package main

import (
    "fmt"
    "go/ast"
    "go/parser"
    "go/token"
	"go/format"
    "os"
	"strconv"
	//"astutil"
	//"reflect"
)


func isIdent(expr ast.Expr, ident string) bool {
	id, ok := expr.(*ast.Ident)
	return ok && id.Name == ident
}

func insertHello(file *ast.File) {
    // Вызываем обход дерева, начиная от корня
    ast.Inspect(file, func(node ast.Node) bool {
        // Для каждого узла дерева
        if expr, ok := node.(*ast.CallExpr); ok && isIdent(expr.Fun, "make") {
            if len(expr.Args) == 2 {
                expr.Args = append(expr.Args, expr.Args[len(expr.Args) - 1])
            }
            lit_val, ok := expr.Args[len(expr.Args) - 1].(*ast.BasicLit)			
            if ok {
                intval, _ := strconv.Atoi(lit_val.Value)
                intval *= 2
                new_expr := ast.BasicLit{0, token.INT, strconv.Itoa(intval)}
                expr.Args[len(expr.Args) - 1] = &new_expr
            } else {
                lit2 := ast.BasicLit{0, token.INT, "2"}
                new_expr := ast.BinaryExpr{expr.Args[len(expr.Args) - 1], 0, token.MUL, &lit2}
                expr.Args[len(expr.Args) - 1] = &new_expr
            }
            
			
        }
        // Возвращая true, мы разрешаем выполнять обход
        // дочерних узлов
        return true
    })
}

func main() {
    if len(os.Args) != 2 {
        fmt.Printf("usage: astprint <filename.go>\n")
        return
    }

    // Создаём хранилище данных об исходных файлах
    fset := token.NewFileSet()

    // Вызываем парсер
    if file, err := parser.ParseFile(
        fset,                 // данные об исходниках
        os.Args[1],           // имя файла с исходником программы
        nil,                  // пусть парсер сам загрузит исходник
        parser.ParseComments, // приказываем сохранять комментарии
    ); err == nil {
        // Если парсер отработал без ошибок, печатаем дерево
		insertHello(file)
        //ast.Fprint(os.Stdout, fset, file, nil)
		format.Node(os.Stdout, fset, file)
    } else {
        // в противном случае, выводим сообщение об ошибке
        fmt.Printf("Error: %v", err)
    }
}