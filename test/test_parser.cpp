#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/PAF.h"
int main(){
	Lexer* lex = new Lexer("/home/f10/pl0-compiler/my_compiler/test/stack.pl0");
	Parser p(lex);
	Program *x = p.program();
	return 0;
}
