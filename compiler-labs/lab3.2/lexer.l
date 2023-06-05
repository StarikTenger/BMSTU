%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]
symbol " "|[a-zA-Z_0-9]
text  [a-zA-Z_0-9](" "|[a-zA-Z_0-9])*
comment "<!--"{symbol}*"-->"

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}

{blank}+   loc.step ();
{comment}+ loc.step ();
\n+        loc.lines (yyleng); loc.step ();


"</"{id}">" return yy::parser::make_CLOSE_TAG  (yytext, loc);
"<"{id}     return yy::parser::make_TAG_NAME  (yytext, loc);
">"         return yy::parser::make_R_BRACKET  (loc);
"="         return yy::parser::make_ASSIGN   (loc);

{int}|("\""{symbol}*"\"")     return yy::parser::make_VALUE (yytext, loc);
{id}       return yy::parser::make_IDENTIFIER (yytext, loc);

.          {
             throw yy::parser::syntax_error
               (loc, "invalid character: " + std::string(yytext));
}

{text}/"<" return yy::parser::make_TEXT (yytext, loc);

<<EOF>>    return yy::parser::make_YYEOF (loc);
%%



void driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
    }
}


void driver::scan_end ()
{
  fclose (yyin);
}
