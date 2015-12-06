#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/PAF.h"
#include "../include/Optimizer.h"
int main(){
	Lexer* lex = new Lexer("program/stack.pl0");
	Parser p(lex);
	Program *x = p.program();
	x->gen();
	/*x->print();*/
	Optimizer *o = new Optimizer(x);
	o->optimize();
	x->print_block();
	return 0;
}
