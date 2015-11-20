#include "../include/Env.h"
#include "../include/Expr.h"
#include "../include/Stmt.h"
#include "../include/Lexer.h"
#include <iostream>
#include <string>
#ifndef PARSER_H
#define PARSER_H
class Parser{
	private :
		Lexer *lex;
		Token *look;
		Env * top = nullptr;
		int used = 0;
		Parser(Lexer* l) {lex = l; move();}
		void move(){look = lex->scan();}
		void error(std::string s){;}     //unimplemented
		void match(Tag t);
		void program();
		Stmt* block();

		void decl_constants();
		Token* constant();
		void constants();
		void constDeclaration();
		void seq_constDeclaration();

		void decl_variables();
		void variables();
		void variableDeclaration();
		void seq_constDeclaration();
		Type* type();

		void seq_identDecl();


		void decl_procandfunc();
		Stmt* compoundstmt();
		Stmt* statement();
		
};
//#define Max_Variables_In_OneDecl  (100)
#endif
/***************************************    template
	//First = {};
	static Tag_Set Follow = {};
	switch (look->tag){
		case :
			break;
	}
	auto search = Follow.find(look.tag);
	if(search == Follow.end())
		;          // throw exception
	// look in follow .
***************************************/
