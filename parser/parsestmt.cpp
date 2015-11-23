#include "../include/Parser.h"
#include <unordered_set>
#include <queue>

//typedef std::unordered_set<Tag> Tag_Set;


void Parser::match(Tag t){
	if(look->tag == t && t != T_DOT)
		move();
	else if(t == T_DOT)
        ;
	else
		;                  // throw exception
}

Program* Parser::program(){
	std::cout<<"this is the whole program"<<endl;
	Program * p  = new Program();
	top = p;
	p->block = block();
	match(T_DOT);
	label begin = p->newlabel();
	label after = p->newlabel();
	p->emitlabel(begin);
	p->gen(begin , after);
	p->emitlabel(after);
	return p;
}

Block* Parser::block(){
	Block * b = new Block(top);
	decl_constants();
	decl_variables();
	b->seq_paf = (Seq_PAF*)seq_paf();
	//decl_procandfunc();
	b->seq_stmt = (Seq *)compoundstmt();
	std::cout<<"this is a block"<<endl;
	return b;
}

Stmt* Parser::compoundstmt(){
	std::cout<<"this is a compoundstmt"<<endl;
	match(T_BEGIN);
	Seq * c = new Seq(statement(),seq_statement()) ;
	match(T_END);
	return c;
}

Stmt* Parser::statement(){
	//First = {T_IDENT,T_IF,T_DO,T_BEGIN,T_WRITE,T_READ,T_FOR,\epsilon};
	//static Tag_Set Follow = {T_ELSE,T_WHILE,T_SEMICOLON,T_END};
	switch (look->tag){
		case T_IF:
			return ifstatement();
		case T_DO:
			return dowhilestatement();
		case T_BEGIN:
			return compoundstmt();
		case T_WRITE:
			return outputstatement();
		case T_READ:
			return inputstatement();
		case T_FOR:
			return forstatement();
		case T_IDENT:{
			Word * tok = (Word *)look;
			Node * nod = top->get(tok);
			if(dynamic_cast<Id *>(nod))
				return assignstatement();
			else if(dynamic_cast<Proc*>(nod)){
				return callprocstatement();
			}
			else{
				;          // throw exception
			}
		}
		default:
			;
	}
	if(look->tag != T_ELSE && look->tag != T_WHILE && look->tag != T_SEMICOLON && look->tag != T_END)
		;          // throw exception
	// look in follow .
	return nullptr;
}

Stmt* Parser::inputstatement(){
	match(T_READ);
    std::cout<<"this is a readstatement"<<endl;
	match(T_OPENPARENTHESIS);
	std::queue<Word*> * list = new std::queue<Word*>();
	if(look->tag != T_IDENT)
		;         // throw exception
	Word * tok = (Word*)look;
	move();
	list->push(tok);
	while(look->tag != T_CLOSEPARENTHESIS){
		match(T_COMMA);
		if(look->tag != T_IDENT)
			;         // throw exception
		tok = (Word *)look;
		move();
		list->push(tok);
	}
	move();
	return new Input(list);
}

Stmt* Parser::outputstatement(){
	//static Tag_Set tag= {T_PLUS,T_MINUS,T_IDENT,T_NUMBER,T_OPENPARENTHESIS};
	STring * s;
	Expr * e;
	std::cout<<"this is a writestatement"<<endl;
	match(T_WRITE);
	match(T_OPENPARENTHESIS);
	if(look->tag == T_STRING){
		if(look->tag != T_STRING)
			;              // throw exception
		s = (STring *) look;
		move();
		if(look->tag == T_CLOSEPARENTHESIS){
			move();
			return new Output(nullptr,s);
		}
		else if(look->tag == T_COMMA){
			e = expr();
			match(T_CLOSEPARENTHESIS);
			return new Output(e,s);
		}
		else{
			;           // throw exception
		}
	}
	if(look->tag != T_PLUS && look->tag != T_MINUS && look->tag != T_IDENT && look->tag != T_NUMBER && look->tag != T_NUMBER && T_OPENPARENTHESIS)
		;           // throw exception
	e = expr();
	match(T_CLOSEPARENTHESIS);
	return new Output(e,nullptr);
}

Stmt* Parser::assignstatement(){    // incomplete
	if(look->tag != T_IDENT)
		;         // throw exception
	Word * dest = (Word*)look;
    std::cout<<"this is a assignstatement"<<endl;
	move();
	Id *id ;
	id = (Id *)top->get(dest);
	if(look->tag == T_ASSIGN){       // check is function or variable,check if const
		move();
		return new Assign(id,expr());
	}
	else if(look->tag == T_OPENBRACKET){
		move();
		Access* ac = new Access(id,id->type,expr());
		match(T_CLOSEBRACKET);
		match(T_ASSIGN);
		return new AssignElem(ac,expr());
	}
	else
		;           // throw exception
	return nullptr;
}

Stmt* Parser::ifstatement(){
	std::cout<<"this is a ifstatement"<<endl;
	match(T_IF);
	Rel * r = condition();
	match(T_THEN);
	Stmt * s = statement();
	if(look->tag != T_ELSE){
		return new If(r,s);
	}
	else{
		match(T_ELSE);
		return new IfElse(r,s,statement());
	}
}

Stmt* Parser::forstatement(){        // incomlete . unchecked identifier information
	match(T_FOR);
    std::cout<<"this is a forstatement"<<endl;
	if(look->tag != T_IDENT)
		;         // throw exception
	Word *tok = (Word*)look;
	move();
	Node * nod = top->get(tok);
	if(Id *id = dynamic_cast<Id *>(nod)){
		match(T_ASSIGN);
		Expr* e1 = expr();
		bool is_to;
		if(look->tag == T_TO)
			is_to = true;
		else if(look->tag == T_DOWNTO)
			is_to = false;
		else{
			;           // throw exception
		}
		move();
		Expr* e2 = expr();
		match(T_DO);
		return new For(id,e1,is_to,e2,statement());
	}
	else {
		;        //throw exception
	}
	return nullptr;
}

Stmt* Parser::dowhilestatement(){
	match(T_DO);
    std::cout<<"this is a dowhilestatement"<<endl;
	Stmt * s = statement();
	match(T_WHILE);
	return new DoWhile(condition(),s);
}

Stmt* Parser::callprocstatement(){
	if(look->tag != T_IDENT)
		;         // throw exception
	Word * w = (Word*)look;
    std::cout<<"this is a callstatement"<<endl;
	move();
	Node * nod = top->get(w);
	Expr * e;
	if(Proc * proc = dynamic_cast<Proc*>(nod)){
		if(proc->paralist.size() == 0)
			return new Callproc(proc,nullptr);
		else{
			std::vector<Expr*> *list = new std::vector<Expr*>();
			match(T_OPENPARENTHESIS);
			e = expr();
			list->push_back(e);
			if(proc->paralist.size() == 1){
				match(T_CLOSEPARENTHESIS);
				return new Callproc(proc,list);
			}
			else{
				for(unsigned int i = 1; i < proc->paralist.size() ; i++){
					match(T_COMMA);
					e = expr();
					list->push_back(e);
				}
				match(T_CLOSEPARENTHESIS);
				return new Callproc(proc,list);
			}
		}
	}
	else {
		; // throw exception
	}
	return nullptr;
}

Stmt* Parser::seq_statement(){
	//First = {T_SEMICOLON,\epsilon};
	//static Tag_Set Follow = {T_END};
	switch (look->tag){
		case T_SEMICOLON:
			move();
			return new Seq(statement(),seq_statement());
		default:
			;
	}
	if(look->tag != T_END)
		;          // throw exception
	// look in follow .
	return nullptr;
}
