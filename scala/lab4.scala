// Root of regex
class RegEx

// Symbols
case object Empty extends RegEx
case class Symbol(value: Char) extends RegEx
case class Alt(left: RegEx, right: RegEx) extends RegEx
case class Conc(left: RegEx, right: RegEx) extends RegEx
case class Star(regex: RegEx) extends RegEx

def braces(str: String): String = "(" + str + ")"

def asString(regex: RegEx, isAlt: Boolean = false): String = regex match {
	case Empty => ""
	case Symbol(value) => escapeMetaCharacters(value.toString)
	case Alt(left, right) => {
		val str = s"${asString(left, true)}|${asString(right, true)}"
		if isAlt 
			then str
			else braces(str)
		}
	case Conc(left, right) => s"${asString(left)}${asString(right)}"
	case Star(regex) => s"${asString(regex)}*"
}

// Escape-sequences detection
def escapeMetaCharacters(str: String): String = {
	val metaCharacters = "|*()\\"
	str.flatMap(c => if (metaCharacters.contains(c)) s"\\$c" else c.toString)
}

def check(rgx: RegEx, res: String): Boolean = {
	val str = asString(rgx, true)
	print("expected: \"")
	print(res)
	print("\" res: \"")
	println(str + "\"")
	str == res
}

val a = Symbol('a')
val b = Symbol('b')
val c = Symbol('c')

val checks = List(
	check(Empty, ""),
	check(Alt(Empty, Empty), "|"),
	check(Conc(Empty, Empty), ""),
	check(Alt(Conc(a, Star(b)), c), "ab*|c"),
	check(Conc(Conc(a, b), Conc(b, c)), "abbc"),
	check(Alt(Alt(b, b), Alt(a, b)), "b|b|a|b")
)