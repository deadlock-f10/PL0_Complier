#include "../include/Parser.h"
#include "../include/Exception.h"
#include <unordered_set>
#include <queue>
#include <iostream>


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
		throw new InappropriateException(look,lex->line);          // throw exception
}

void Parser::constants(){
	try{
		match(T_CONST);
		constDeclaration();
		seq_constDeclaration();
		match(T_SEMICOLON);
	}
	catch (Exception * e){
		std::cout<<e->print()<<endl;
		if(++error_count >= max_errors)
			throw new ToomucherrorException();
		while(look->tag != T_PROCEDURE && look->tag != T_FUNCTION && look->tag != T_BEGIN && look->tag != T_VAR) 
			move();
/*		if(look->tag == T_COMMA)
			goto E;
		else
			goto F;*/
	}
}

Token* Parser::constant(){
	switch(look->tag){
		case T_PLUS:{
				move();
				if(look->tag != T_NUMBER)
					throw new TokenMatchException(look,T_NUMBER,lex->line);
				Num *t = (Num *)look;
				move();
				return t;
			}
		case T_MINUS:{
			move();
			if(look->tag != T_NUMBER)
				throw new TokenMatchException(look,T_NUMBER,lex->line);
			Num *t = (Num *)look;
			t->value *= (-1);
			move();
			return t;
		 }
		case T_CHARACTER:{
			Character * c = (Character *)look;
			move();
			return c;
		}
		case T_NUMBER:{
			Num * n = (Num *)look;
			move();
			return n;
		}
		default:
			throw new InappropriateException(look,lex->line);           // throw exception
	}
	return nullptr;       //never excuted
}
void Parser::constDeclaration(){        //  imcomplete due to lack of protaction for const. deal with it
	if(look->tag != T_IDENT)
		throw new TokenMatchException(look,T_IDENT,lex->line);                // throw exception
	Word * tok = (Word*)look;
	move();
	match(T_EQ);
	Token * CONST = constant();
	Type * t;
	Id * id;
	int constvalue=0;
	if(CONST->tag == T_NUMBER){
		t = Type::Int;
		constvalue = ((Num*)CONST)->value;

	}
	else if(CONST->tag == T_CHARACTER){
		t = Type::Char;
		constvalue = ((Character*)CONST)->value;
	}
	else{
		;           // throw exception
	}
	id = new Id(tok,t,top->used,true,constvalue,top->level);  // Should assign Const.Value to this ID      deal with it
	top->used += t->width;
	top->put(tok,id);
}
void Parser::seq_constDeclaration(){
	//static Tag_Set Follow = {T_SEMICOLON};
	//try{
		switch (look->tag){
			case T_COMMA:
				move();
				constDeclaration();
				seq_constDeclaration();
				return ;
			default:
				;
		}
		if(look->tag != T_SEMICOLON)
			throw new TokenMatchException(look,T_SEMICOLON,lex->line);          // throw exception
	// look is in follow .
	//}
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
		throw new InappropriateException(look,lex->line);          // throw exception
	// look in follow .

}
void Parser::variables(){
	try{
	match(T_VAR);
	variableDeclaration();
	match(T_SEMICOLON);
	seq_variableDeclaration();
	}
	catch(Exception *e){
		std::cout<<e->print()<<endl;
		if(++error_count >= max_errors)
			throw new ToomucherrorException();
		while(look->tag != T_PROCEDURE && look->tag != T_FUNCTION && look->tag != T_BEGIN) 
			move();
		/*if(look->tag == T_SEMICOLON)
			goto E;*/
	}
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
		throw new InappropriateException(look,lex->line);          // throw exception
	// look in follow .
}


void Parser::variableDeclaration(){
	std::queue<Token*> identifier_list;
	if(look->tag != T_IDENT)
		throw new TokenMatchException(look,T_IDENT,lex->line);          // throw exception
	Word * tok = (Word *)look;
	move();
	identifier_list.push(tok);
	while(look->tag != T_COLON){
		match(T_COMMA);
		if(look->tag != T_IDENT)
			throw new TokenMatchException(look,T_IDENT,lex->line);          // throw exception
		tok = (Word *)look;
		move();
		identifier_list.push(tok);
	}
	move();
	Type * t = type();
	Id * id;
	while(identifier_list.empty() == false){
		tok = (Word *)identifier_list.front();
		identifier_list.pop();
		id = new Id((Word*)tok,t,top->used,top->level);
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
				if(look->tag != T_NUMBER)
                    throw new TokenMatchException(look,T_NUMBER,lex->line);           // throw exception
				Num * t = (Num*)look;
				move();
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
				throw new InappropriateException(look,lex->line);              // throw exception
			}
		default:
			throw new InappropriateException(look,lex->line);             //throw exception
	}
	return nullptr;             // never excuted.
}
