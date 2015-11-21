#include "Parser.h"
#include <unordered_set>
#include <queue>

typedef std::unordered_set<Tag> Tag_Set;


void Parser::match(Tag t){
	if(look->tag = t)
		move();
	else
}

void Parser::program(){
	Stmt* s = block();	
	match(T_DOT);
	label begin = s.newlabel();
	label after = s.newlabel();
	s.emitlabel(begin);
	s.gen(begin , after);
	s.emitlabel(after);
}

Stmt* Parser::block(){
	Env * saveEnv = top;	
	top = new Env(top);
	decl_constants();
	decl_variables();
	decl_procandfunc();
	Stmt* s = compoundstmt();
	top = saveEnv;
	return s;
}

void Parser::decl_constants(){
	//First = {T_CONST,\epsilon};
	static bool DeriveEpsilon = true;
	static Tag_Set Follow = {T_VAR,T_PROCEDURE,T_FUNCTION,T_BEGIN};
	switch (look->tag){
		case T_CONST:
			constants();
			return ;
	}
	if(DeriveEpsilon == true){
		auto search = Follow.find(look.tag);
		if(search == Follow.end())
			;          // throw exception
		// look in follow .
	}
}

void Parser::constants(){
	match(T_CONST);
	constDeclaration();
	seq_constDeclaration();
	match(T_SEMICOLON);
}

Token* Parser::constant(){
	switch(look->tag){
		case T_PLUS:
			move();
			Token *t = look;
			match(T_NUMBER);
			return (Num*)t;
		case T_MINUS:
			move();
			Token *t = look;
			match(T_NUMBER);
			((Num*)t)->value *= (-1);
			return (Num*)t;
		case T_CHARACTER:
			move();
			return (Character *)look;
		default:
			;           // throw exception
	}
			
}
void Parser::constDeclaration(){        //  imcomplete due to lack of protaction for const.
	Token * tok = look;
	match(T_IDENT);
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
	id = new Id((Word*)tok,t,used);  // Should assign Const.Value to this ID
	used += t->width;
	top.put(tok,id);
}
void Parser::seq_constDeclaration(){
	//First = {",",\epsilon};
	static Tag_Set Follow = {T_SEMICOLON};
	switch (look->tag){
		case T_COMMA:
			move();
			constDeclaration();
			seq_constDeclaration();
			return ;
	}
	auto search = Follow.find(look.tag);
	if(search == Follow.end())
		;          // throw exception
	// look is in follow .
}

void Parser::decl_variables(){
	//First = {"var",\epsilon};
	static Tag_Set Follow = {T_PROCEDURE,T_FUNCTION,T_BEGIN};
	switch (look->tag){
		case T_VAR:
			variables();
			return ;
	}
	auto search = Follow.find(look.tag);
	if(search == Follow.end())
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
	static Tag_Set Follow = {T_PROCEDURE,T_FUNCTION,T_BEGIN};
	switch (look->tag){
		case T_IDENT:
			variableDeclaration();
			return;
	}
	auto search = Follow.find(look.tag);
	if(search == Follow.end())
		;          // throw exception
	// look in follow .
}


void Parser::variableDeclaration(){
	std::queue<Token*> identifier_list;
	Token * tok = look;
	match(T_IDENT);
	identifier_list.push(tok);
	while(look->tag != ":"){
		match(T_COMMA);
		tok = look;
		match(T_IDENT);
		identifier_list.push(tok);
	}
	move();
	Type * t = type();
	Id * id;
	while(identifier_list.empty() == false){
		tok = identifier_list.pop();				
		id = new Id((Word*)tok,t,used);
		used += t->width;
		top.put(tok,id);
	}
}

Type* Parser::type(){
	switch(look->type){
		case T_INT:
			move();
			return Type::Int ;
		case T_CHAR:
			move();
			return Type::Char ;
		case Array:
			move();
			match(T_OPENBRACKET);
			Num * t = (Num*)look;
			match(T_NUMBER);
			int size = t->value;
			match(T_CLOSEBRACKET);
			match(T_OF);
			Type * type = (Type*) look;
			if(look->tag == T_INT || look->tag == T_CHAR){
				move();
				return new Array(size,type);
			}
			else 
				;              // throw exception
		default:
			;             //throw exception
	}

}
