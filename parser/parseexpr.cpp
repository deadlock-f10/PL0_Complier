#include "../include/Expr.h"
#include "../include/Parser.h"
#include "../include/Exception.h"

Expr* Parser::factor(){
	switch(look->tag){
		case T_NUMBER:{
			Token* tok;
			tok = (Num *) look;
			move();
			return new Constant((Num *)tok);
		}
		/*case T_CHARACTER:
			tok = (Character *) look;
			move();
			return new Character(tok);*/        //not allowed
		case T_OPENPARENTHESIS:
			return parentheisfactor();
		case T_IDENT:{
				Word* tok = (Word *)look;
				if(tok->lexeme != top->name->lexeme){
					Node * nod = top->get(tok);
					if(nod == nullptr)
						;             // throw exception
					if(Id * id = dynamic_cast<Id *>(nod)){
						move();
						if(look->tag != T_OPENBRACKET)
							return id;
						else
							return offset(id);
					}
					else if(Func *func = dynamic_cast<Func*>(nod)){
						move();
						return callfunc(func);
					}
					else{
						;              // throw exception
					}
				}
				else{
					move();
					return callfunc(dynamic_cast<Func*>(top->prev->get(tok)));
				}
		}//incomplete . to much things to consider
		default:
			;
	}
	return nullptr ; // will not excuted
}

Callfunc* Parser::callfunc(Func *func){
	Expr * e;
	if(func->paralist.size() == 0)
		return new Callfunc(func,new std::vector<Expr*>());
	else{
		std::vector<Expr*> *list = new std::vector<Expr*>();
		match(T_OPENPARENTHESIS);
		e = expr();
		list->push_back(e);
		if(func->paralist.size() == 1){
			match(T_CLOSEPARENTHESIS);
			return new Callfunc(func,list);
		} else{
			for(unsigned int i = 1; i < func->paralist.size() ; i++){
				match(T_COMMA);
				e = expr();
				list->push_back(e);
			}
			match(T_CLOSEPARENTHESIS);
			return new Callfunc(func,list);
		}
	}
	return nullptr ; // will not excuted
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

Expr* Parser::expr(){
	if(look->tag == T_MINUS)
		return unary();
	else if(look->tag == T_PLUS)
		move();
	return unsignedexpr();
}

Expr* Parser::unary(){
	match(T_MINUS);
	return new Unary(Word::Minus,unsignedexpr());
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
	throw  InappropriateException(look,lex->line);           // throw exception
	return nullptr ; // will not excuted
}

Access * Parser::offset(Id *id){
	match(T_OPENBRACKET);
	Expr * e  = expr();
	Type * t = id->type;
	t = ((Array*)t)->of;
	Expr* w = new Constant(new Num(t->width));
	Expr * e1 = new Arith(Word::Mult,e,w);
	match(T_CLOSEBRACKET);
	return new Access(id,t,e1);
}
