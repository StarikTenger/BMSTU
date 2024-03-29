/* This block of code will go into the header file generated by bison */
%code requires{
   class Data;
}

%language "c++"
%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.lac full

%code {
# include "driver.hpp"
}

%define api.token.prefix {TOK_}
%token
  R_BRACKET ">"
  ASSIGN "="
;

%token <std::string> IDENTIFIER "identifier"
%token <std::string> STRING "string"
%token <std::string> TAG_NAME "tag name"
%token <std::string> CLOSE_TAG "close tag"
%token <std::string> TEXT "text"
%token <std::string> VALUE "value"

//%nterm <int> exp
%token YYEOF 0

%printer { yyo << $$; } <*>;

%%
%start element;

// For printing tags
_ASSIGN: ASSIGN {drv.print("=");};
_IDENTIFIER: IDENTIFIER {drv.print(" "); drv.print($1);};
_STRING: STRING {drv.print($1);};
_TAG_NAME: TAG_NAME {drv.print_tab(); drv.print($1);};
_CLOSE_TAG: CLOSE_TAG {drv.nesting--; drv.print_tab(); drv.print($1); drv.newline();};

element: open_tag elements _CLOSE_TAG {}
| TEXT {drv.print_tab(); drv.print($1); drv.newline();};

elements:
  %empty           {}
| elements element {};

open_tag: _TAG_NAME attrs ">" {drv.print(">"); drv.newline(); drv.nesting++;};

attr: _IDENTIFIER _ASSIGN VALUE {drv.print($3);};

attrs:
  %empty     {}
| attrs attr {};

%%

void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}