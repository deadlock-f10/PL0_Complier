#include <stdio.h>
#include <fstream>
#include <string>
#include "../include/Lexer.h"
int main(int argn , char *arg[])
{
	Lexer l(arg[1]);
	int i = 0;
	Token*  t;
	do{
		t = l.scan();
		string s;
		if(t->tag == T_NULL)
			s = "T_NULL";
		else if(t->tag == T_INT)
			s = "T_INT";
		else if(t->tag == T_CHAR)
			s = "T_CHAR";
		else if(t->tag == T_IDENT)
			s = "T_IDENT";
		else if(t->tag == T_NUMBER)
			s = "T_NUMBER";
		else if(t->tag == T_STRING)
			s = "T_STRING";
		else if(t->tag == T_CHARACTER)
			s = "T_CHARACTER";
		else if(t->tag == T_OF)
			s = "T_OF";
		else if(t->tag == T_ARRAY)
			s = "T_ARRAY";
		else if(t->tag == T_CONST)
			s = "T_CONST";
		else if(t->tag == T_VAR)
			s = "T_VAR";
		else if(t->tag == T_PROCEDURE)
			s = "T_PROCEDURE";
		else if(t->tag == T_FUNCTION)
			s = "T_FUNCTION";
		else if(t->tag == T_COLON)
			s = "T_COLON";
		else if(t->tag == T_BEGIN)
			s = "T_BEGIN";
		else if(t->tag == T_END)
			s = "T_END";
		else if(t->tag == T_IF)
			s = "T_IF";
		else if(t->tag == T_THEN)
			s = "T_THEN";
		else if(t->tag == T_ELSE)
			s = "T_ELSE";
		else if(t->tag == T_WHILE)
			s = "T_WHILE";
		else if(t->tag == T_DO)
			s = "T_DO";
		else if(t->tag == T_FOR)
			s = "T_FOR";
		else if(t->tag == T_DOWNTO)
			s = "T_DOWNTO";
		else if(t->tag == T_TO)
			s = "T_TO";
		else if(t->tag == T_READ)
			s = "T_READ";
		else if(t->tag == T_WRITE)
			s = "T_WRITE";
		else if(t->tag == T_ASSIGN)
			s = "T_ASSIGN";
		else if(t->tag == T_PLUS)
			s = "T_PLUS";
		else if(t->tag == T_MINUS)
			s = "T_MINUS";
		else if(t->tag == T_MULT)
			s = "T_MULT";
		else if(t->tag == T_DIV)
			s = "T_DIV";
		else if(t->tag == T_EQ)
			s = "T_EQ";
		else if(t->tag == T_GT)
			s = "T_GT";
		else if(t->tag == T_LT)
			s = "T_LT";
		else if(t->tag == T_LE)
			s = "T_LE";
		else if(t->tag == T_GE)
			s = "T_GE";
		else if(t->tag == T_NE)
			s = "T_NE";
		else if(t->tag == T_OPENPARENTHESIS)
			s = "T_OPENPARENTHESIS";
		else if(t->tag == T_CLOSEPARENTHESIS)
			s = "T_CLOSEPARENTHESIS";
		else if(t->tag == T_OPENBRACKET)
			s = "T_OPENBRACKET";
		else if(t->tag == T_CLOSEBRACKET)
			s = "T_CLOSEBRACKET";
		else if(t->tag == T_COMMA)
			s = "T_COMMA";
		else if(t->tag == T_SEMICOLON)
			s = "T_SEMICOLON";
		else if(t->tag == T_DOT)
			s = "T_DOT";
		//std::cout <<++i <<'\t' << s << '\t' <<   t->toString()<< endl;
		printf("%d\t%-20s\t%s\n",++i,s.c_str(),t->to_string().c_str());
	}while(t->tag != T_DOT);
	return 0;
}
