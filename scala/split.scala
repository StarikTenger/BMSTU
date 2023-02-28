// 27. Функция split: (List[Int], Int ^> Boolean) ^> List[List[Int]],
// выполняющая разбиение последовательности целых чисел на подпоследовательности,
// разделённые числами, удовлетворяющими предикат

val split: (List[Int], Int => Boolean) => List[List[Int]] = 
(xs, pred) => { xs match
	case List() => List(List())
	case x::xs => {
		val res = split(xs, pred)
		if pred(x) then List()::res else {res match
			case y::ys => (x::y)::ys
			case Nil => List(List(x))
		}
	}
}