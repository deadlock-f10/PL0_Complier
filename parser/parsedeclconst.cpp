#include "../include/Parser.h"
#include <unordered_set>
#include <queue>


void Parser::decl_constants(){
	//First = {T_CONST,\epsilon};
	switch (look -> tag){
		case T_CONST:
			constants();
			return ;
		default:
			;
	}
	if(look->tag != T_VAR && look->tag != T_PROCEDURE && look->tag != T_FUNCTION && look->tag != T_BEGIN)
		;          // throw exception
}

void Parser::constants(){
	match(T_CONST);
	constDeclaration();
	seq_constDeclaration();
	match(T_SEMICOLON);
}

Token* Parser::constant(){
	switch(look->tag){
		case T_PLUS:{
				move();
				match(T_NUMBER);
				Num *t = (Num *)look;
				return t;
			}
		case T_MINUS:{
			move();
			match(T_NUMBER);
			Num *t = (Num *)look;
			t->value *= (-1);
			return t;
		 }
		case T_CHARACTER:
			move();
			return (Character *)look;
		default:
			;           // throw exception
	}
	return nullptr;       //never excuted
}
void Parser::constDeclaration(){        //  imcomplete due to lack of protaction for const. deal with it
	match(T_IDENT);
	Word * tok = (Word*)look;
	match(T_EQ);
	Token * CONST = constant();
	Type * t;
	Id * id;
	if(CONST->tag == T_NUMBER)
		t = Type::Int;
	else if(CONST->tag == T_CHARACTER)
		t = Type::Char;
	else{
		;           // throw exception
	}
	id = new Id(tok,t,top->used,true);  // Should assign Const.Value to this ID      deal with it
	top->used += t->width;
	top->put(tok,id);
}
void Parser::seq_constDeclaration(){
	//static Tag_Set Follow = {T_SEMICOLON};
	switch (look->tag){
		case T_COMMA:
			move();
			constDeclaration();
			seq_constDeclaration();
			return ;
		default:
			;
	}
	if(look->tag != T_SEMICOLON);
		;          // throw exception
	// look is in follow .
}



void Parser::decl_variables(){
	//First = {"var",\epsilon};
	//static Tag_Set Follow = {T_PROCEDURE,T_FUNCTION,T_BEGIN};
	switch (look->tag){
		case T_VAR:
			variables();
			return ;
		default:
			;
	}
	if(look->tag != T_PROCEDURE && look->tag != T_FUNCTION && look->tag != T_BEGIN)
		;          // throw exception
	// look in follow .

}
void Parser::variables(){
	match(T_VAR);
	variableDeclaration();
	match(T_SEMICOLON);
	seq_variableDeclaration();
}

void Parser::seq_variableDeclaration(){
	//First = {T_IDENT , \epsilon};
	//static Tag_Set Follow = {T_PROCEDURE,T_FUNCTION,T_BEGIN};
	switch (look->tag){
		case T_IDENT:
			variableDeclaration();
			match(T_SEMICOLON);
			seq_variableDeclaration();
			return;
		default:
			;
	}
	if(look->tag  != T_PROCEDURE && look->tag != T_FUNCTION && look->tag != T_BEGIN)
		;          // throw exception
	// look in follow .
}


void Parser::variableDeclaration(){
	std::queue<Token*> identifier_list;
	match(T_IDENT);
	Word * tok = (Word *)look;
	identifier_list.push(tok);
	while(look->tag != T_COLON){
		match(T_COMMA);
		match(T_IDENT);
		tok = (Word *)look;
		identifier_list.push(tok);
	}
	move();
	Type * t = type();
	Id * id;
	while(identifier_list.empty() == false){
		tok = (Word *)identifier_list.front();				
		identifier_list.pop();				
		id = new Id((Word*)tok,t,top->used);
		top->used += t->width;
		top->put(tok,id);
	}
}

Type* Parser::type(){
	switch(look->tag){
		case T_INT:
			move();
			return Type::Int ;
		case T_CHAR:
			move();
			return Type::Char ;
		case T_ARRAY:{
				move();
				match(T_OPENBRACKET);
				match(T_NUMBER);
				Num * t = (Num*)look;
				int size = t->value;
				match(T_CLOSEBRACKET);
				match(T_OF);
				//Type * type;
				if(look->tag == T_INT){
					move();
					return new Array(size,Type::Int);
				}
				else if(look->tag == T_CHAR){
					move();
					return new Array(size,Type::Char);
				}
				;              // throw exception
			}
		default:
			;             //throw exception
	}
	return nullptr;             // never excuted.
}
