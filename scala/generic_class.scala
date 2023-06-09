import scala.compiletime.ops.boolean
class KadaneStack() {
	var data: List[Int] = Nil
	var s1: List[Int] = Nil
	var max_s2: List[Int] = Nil
	var max_sum: List[Int] = Nil
	def push(elem: Int) = {
		data = elem::data
		s1 = (if s1.isEmpty then elem else elem + s1.head)::s1
		val s2 = data.head - s1.head
		max_s2 = (if max_s2.isEmpty then s2 else s2.max(max_s2.head))::max_s2
		max_sum = (if max_sum.isEmpty 
					then (s1.head + max_s2.head) 
					else (s1.head + max_s2.head).max(max_sum.head))::max_sum
	}
	def pop(): Int = {
		data = data.tail
		s1 = s1.tail
		max_s2 = max_s2.tail
		max_sum = max_sum.tail
		data.head
	}
	def maxSum(): Int = {
		max_sum.head
	}
	def empty(): Boolean = {
		data.isEmpty
	}
}

// 1  1  1 -9  1 7 : data
// 1  2  3 -6 -5 2 : S1
// 0 -1 -2 -3  6 5 : S2
// 0  0  0  0  6 6 : max S2
// 1  2  3  3  3 8