#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/PAF.h"
int main(){
	Lexer* lex = new Lexer("test/stack.pl0");
	Parser p(lex);
	Program *x = p.program();
	x->gen();
	x->print();
	return 0;
}
