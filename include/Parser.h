#include "../include/Env.h"
#include "../include/Expr.h"
#include "../include/Stmt.h"
#include "../include/Lexer.h"
#include "../include/PAF.h"
#include <iostream>
#include <string>
#ifndef PARSER_H
#define PARSER_H
class Parser{
	public:
		Lexer *lex;
		Token *look;
		Program * top = nullptr;
//		int used = 0;
		Parser(Lexer* l) {lex = l; move();}
		void move(){look = lex->scan();}
		void error(std::string s){;}     //unimplemented
		void match(Tag t);
		Program* program();
		Program* seq_paf();
		Block* block();

		void decl_constants();
		Token* constant();
		void constants();
		void constDeclaration();
		void seq_constDeclaration();

		void decl_variables();
		void variables();
		void variableDeclaration();
		void seq_variableDeclaration();
		Type* type();

		void seq_identDecl();


		void decl_procandfunc();
		Proc* proc_decl();
		Func* func_decl();
		void optional_para();
		void form_para();
		void form_para_seg();
		void seq_formpara_seg();
		void formpara_seg();
		

		Seq* compoundstmt();
		Stmt* statement();
		Seq* seq_statement();
		Input* inputstatement();
		Output* outputstatement();
		Stmt* assignstatement();        //incomplete .no type checking
		Stmt* ifstatement();
		For* forstatement();
		DoWhile* dowhilestatement();
		Callproc* callprocstatement();

		Expr* expr();
		Expr* unary();
		Expr* unsignedexpr();
		Expr* factor();              // try some function recursion
		Expr* term();
		Expr* parentheisfactor();
		Access* offset(Id *id);
		Callfunc* callfunc(Func * func);

		Rel * condition();
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
