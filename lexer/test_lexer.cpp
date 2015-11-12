#include <iostream>
#include <fstream>
#include "Lexer.h"
int main()
{
	Lexer l("local.pl0");
	int i = 0;
	Token*  t;
	do{
		t = l.scan();
		std::cout <<  t->toString()<< endl;
	}while(t->tag != T_DOT);
	return 0;
}
