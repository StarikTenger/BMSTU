import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.Scanner;
import java.util.Optional;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

enum TokenType {
    LITERAL, IDENTIFIER, KEYWORD, ERROR
}

class Token {
    public TokenType type;
    public String value;
    public int begin;
    public int end;
}

class Lexer {
    private String str;
    private Matcher matcher;
    public Lexer(String _str) {
        str = _str;
        String pattern = "";
        try {
            BufferedReader reader = new BufferedReader(new FileReader("regex.txt"));
            pattern = reader.readLine();
        } catch (IOException e) {
			e.printStackTrace();
		}
        Pattern p = Pattern.compile(pattern);
        matcher = p.matcher(str);
    }

    public Optional<Token> next() {
        Token tkn = new Token();
        if (matcher.find()) {
            if (matcher.group("literal") != null) {tkn.type = TokenType.LITERAL;}
            if (matcher.group("keyword") != null) {tkn.type = TokenType.KEYWORD;}
            if (matcher.group("identifier") != null) {tkn.type = TokenType.IDENTIFIER;}
            if (matcher.group("divider") != null) {return next();}
            if (matcher.group("error") != null) {tkn.type = TokenType.ERROR;}
            //System.out.println(matcher.group() + " [" + matcher.start() + " - " + matcher.end() + "]");
            tkn.value = matcher.group();
            tkn.begin = matcher.start();
            tkn.end = matcher.end();
            return Optional.of(tkn);
        }
        
        return Optional.empty();
    }
}



public class Main {
    public static void main(String args[]) {
        Scanner sc=new Scanner(System.in);
        while(sc.hasNextLine()){
            //Lexer lxr = new Lexer("'1232saf\\n' z for forward backward forsaken '\\n\\1234' '\\g'");
            Lexer lxr = new Lexer(sc.nextLine());
            Optional<Token> tkn = lxr.next();
            while (tkn.isPresent()) {
                System.out.println(tkn.get().type + " (" + tkn.get().begin + ", " +  tkn.get().end + "): " +  tkn.get().value);
                tkn = lxr.next();
            }
        }
    }
}