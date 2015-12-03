#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/PAF.h"
#include "../include/Optimizer.h"
#include "../include/Codegen.h"
#include <fstream>

std::ofstream targetcode("/home/f10/pl0-compiler/my_compiler/target.asm");
int main(){
	Lexer* lex = new Lexer("test/stack.pl0");
	Parser p(lex);
	Program *x = p.program();
	x->gen();
	/*x->print();*/
	Optimizer *o = new Optimizer(x);
	o->optimize();
	Codegenerator *c = new Codegenerator(x);
	c->gen(x);
	c->print(x);
	return 0;
}

