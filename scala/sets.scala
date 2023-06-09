// Множество целых чисел, конструируемое на основе предиката, определяющего принадлежность числа множеству.

// Операции: объединение двух множеств («+»), пересечение двух множеств («*»), дополнение множества («!»), умножение всех элементов множества на целое число («*»), целочисленное деление всех элементов множества на целое число («/»), проверка принадлежности числа множеству («contains»).

class NumberSet(var pred: Int => Boolean) {
	def contains(value: Int): Boolean = pred(value)
	def +(other: NumberSet) = NumberSet((x: Int) => other.contains(x) || contains(x))
	def *(other: NumberSet) = NumberSet((x: Int) => other.contains(x) && contains(x))
	def *(n: Int) = NumberSet((x: Int) => contains(x / n) && x % n==0)
	def /(n: Int) = NumberSet((x: Int) => contains(x * n))
	def unary_! = NumberSet((x: Int) => !contains(x))
	def check_interval(l: Int, r: Int) = Range.inclusive(l, r).filter(contains)
}

def MultiplesOf(k: Int) = NumberSet((n: Int) => n % k==0)