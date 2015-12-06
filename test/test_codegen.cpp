#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/PAF.h"
#include "../include/Optimizer.h"
#include "../include/Codegen.h"
#include <fstream>

int main(int argn , char * arg[]){
	Lexer* lex; 
	if(argn == 2)
		lexer = new Lexer(std::string(arg[1]));
	else
		lexer = new Lexer("program/stack.pl0");
	Parser p(lex);
	Program *x = p.program();
	x->gen();
	x->print();
	Optimizer *o = new Optimizer(x);
	o->optimize();
	x->print_block();
	Codegenerator *c = new Codegenerator(x);
	c->gen(x);
	c->print(x);
	return 0;
}
