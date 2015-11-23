#include "../include/Expr.h"
#include "../include/Parser.h"

Expr* Parser::expr(){
	if(look->tag == T_MINUS){
		return new Unary(T_MINUS,unary());
	}
}

Expr* Parser::factor(){
	Token* tok;
	switch(look->tag){
		case T_NUMBER:
			tok = (Num *) look;
			move();
			return new Constant(tok);
		/*case T_CHARACTER:
			tok = (Character *) look;
			move();
			return new Character(tok);*/        //not allowed
		case T_OPENPARENTHESIS:
			return parentheisfactor();
		case T_IDENT:             //incomplete . to much things to consider
			tok = (Word *) tok;
			Node * nod = top.get(tok);
			if(nod == nullptr)
				;             // throw exception
			if(Id * id = dynamic_cast<Id *>nod){
				move();
				if(look->tag != T_OPENBRACKET)
					return id;
				else
					return offset(id);
			}
			else if(Func * func = dynamic_cast<Func*>nod){
				move();
				match(T_OPENPARENTHESIS);
				Callfunc * cf = callfunc(func);
				match(T_CLOSEPARENTHESIS);
				return cf;
			}
			else {
				;              // throw exception
			}
	}
}

Expr* Parser::term(){
	Expr *x = factor();
	while(look->tag == T_MULT || look->tag == T_DIV){
		Token *tok = look;
		move();
		x = new Arith(tok,x,factor());
	}
	return x;
}

Expr* expr(){
	if(look->tag == T_MINUS)
		return unary();
	else if(look->tag == T_PLUS)
		move();
	return unsignedexpr();
}

Expr* Parser::unary(){
	match(T_MINUS);
	return new Unary(T_MINUS,unsignedexpr());
}

Expr* Parser::unsignedexpr(){
	Expr* x = term();
	while(look->tag == T_PLUS || look->tag == T_MINUS){
		Token * tok = look;
		move();
		x = new Arith(tok,x,term());
	}
	return x;
}

Expr *Parser::parentheisfactor(){
	match(T_OPENPARENTHESIS);
	Expr* x = expr();
	match(T_CLOSEPARENTHESIS);
	return x;
}

Rel * Parser::condition(){
	Expr *x1 = expr();
	Token * tok = look;
	if(tok->tag == T_EQ ||tok->tag == T_GT ||tok->tag == T_LT ||tok->tag == T_GE ||tok->tag == T_LE ||tok->tag == T_NE){
		move();
		return new Rel(tok,x1,expr());
	}
	;           // throw exception
}
