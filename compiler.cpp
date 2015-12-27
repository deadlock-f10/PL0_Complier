#include "include/Lexer.h"
#include "include/Parser.h"
#include "include/PAF.h"
#include "include/Optimizer.h"
#include "include/Codegen.h"
#include "include/Exception.h"
#include <fstream>

int main(int argn , char * arg[]){
	try{
		Lexer* lex; 
		bool isoptimize = false;
		if(argn == 2)
			lex = new Lexer(std::string(arg[1]));
		else if(argn == 3){
			lex = new Lexer(std::string(arg[1]));
			if(arg[2][0] == '1')			
				isoptimize = true;
		}
		else
			lex = new Lexer("program/stack.pl0");
		Parser p(lex);
		Program *x = p.program();
		if( p.error_count != 0){
			std::cout<<"compiler error , abort"<<endl;
			return 0;
		}
		x->gen();
		x->print();
//		Optimizer *o = new Optimizer(x,isoptimize);
		Optimizer *o = new Optimizer(x,true);
		o->optimize();
		x->print_block();
		Codegenerator *c = new Codegenerator(x);
		c->gen(x);
		c->print(x);
		return 0;
	}
	catch(InappropriateException *e){
		std::cout<<e->print()<<endl;
	}
	catch(Exception *e){
		std::cout<<e->print()<<endl;
		return 0;
	}
}
