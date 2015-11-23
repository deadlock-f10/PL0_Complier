#include "../include/Env.h"
#include "../include/Expr.h"
#include "../include/Stmt.h"
#include "../include/Lexer.h"
#include <iostream>
#include <string>
#include <PAF.h>
#ifndef PARSER_H
#define PARSER_H
class Parser{
	private :
		Lexer *lex;
		Token *look;
		Program * top = nullptr;
//		int used = 0;
		Parser(Lexer* l) {lex = l; move();}
		void move(){look = lex->scan();}
		void error(std::string s){;}     //unimplemented
		void match(Tag t);
		Program* program();
		Block* block(Program *p);

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
		Proc* proc_decl();
		Func* func_decl();
		void optional_para();
		void form_para();
		void seq_formpara_seg();
		void formpara_seg();
		

		Stmt* compoundstmt();
		Stmt* statement();
		Stmt* seq_statement();
		Stmt* inputstatement();
		Stmt* outputstatement();
		Stmt* assignstatement(Word * dest);        //incomplete .no type checking
		Stmt* ifstatement();
		Stmt* forstatement();
		Stmt* dowhilestatement();
		Stmt* callprocstatement(Word * procname);

		Expr* expr();
		Expr* unary();
		Expr* unsignedexpr();
		Expr* factor();
		Expr* term();
		Expr* parentheisfactor();
		Access* offset(Id *id);
		Callfunc* callfunc(Func* fc);

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
