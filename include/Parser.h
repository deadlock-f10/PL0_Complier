#include "../include/Env.h"
#include "../include/Expr.h"
#include "../include/Stmt.h"
#include "../include/Lexer.h"
#include "../include/PAF.h"
#include <iostream>
#include <string>
#include <unordered_map>
#define max_errors  (15)
#ifndef PARSER_H
#define PARSER_H
class Parser{
	private:
		Token *buffer = nullptr;                    // design when check if we are write a const variable.
	public:
		class labelcounter;
		static std::unordered_map<Word*,Parser::labelcounter*>  lcounter;
		static std::string getlabel(Word *w);
		bool cangenerate = true;
		int error_count = 0;
		Lexer *lex;
		Token *look;
		Program * top = nullptr;
//		int used = 0;
		Parser(Lexer* l) {lex = l; move();}
		void move(){
			if(buffer == nullptr)
				look = lex->scan();
			else{
				look = buffer;
				buffer = nullptr;
			}
		}//std::cout<<tagto_string[look->tag]<<endl;}
		void push_back(Token *x){buffer = x;}
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
		
		Stmt* compoundstmt();
		Stmt* statement();
		Stmt* seq_statement();
		Stmt* inputstatement();
		Stmt* outputstatement();
		Stmt* assignstatement();        //incomplete .no type checking
		Stmt* ifstatement();
		Stmt* forstatement();
		Stmt* dowhilestatement();
		Stmt* callprocstatement();

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
