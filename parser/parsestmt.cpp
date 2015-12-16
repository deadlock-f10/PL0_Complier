#include "../include/Parser.h"
#include "../include/OP.h"
#include "../include/Tag.h"
#include <unordered_set>
#include <queue>
#include <iostream>
#include "../include/Exception.h"

//typedef std::unordered_set<Tag> Tag_Set;


void Parser::match(Tag t){
	if(look->tag == t)
		move();
	else
		//cout<<"unmatched "<<t<<endl;                  // throw new exception
		throw new TokenMatchException(look,t,lex->line);
}

class Parser::labelcounter{
	public:
		Word* x;
		int count = 1;
		labelcounter(Word* m){x = m;}
};
std::unordered_map<Word*,Parser::labelcounter*>  Parser::lcounter;
std::string Parser::getlabel(Word *w){
		std::unordered_map<Word*,labelcounter*>::const_iterator found = lcounter.find(w);
		if(found == lcounter.end()){
			lcounter.insert(std::make_pair(w,new labelcounter(w)));
			return w->lexeme;
		}
		else{
			labelcounter *x = found->second;
			x->count++;
			return w->lexeme + patch::to_string(x->count);
		}
}

Program* Parser::program(){
	Program * p  = new Program();
	top = p;
	p->block = block();
	if(look->tag != T_DOT)
		throw new TokenMatchException(look,T_DOT,lex->line);                 // throw exception
	//label begin = p->newlabel();
	//label after = p->newlabel();        //suppose type checking , name lookup have already done in parse phase
	//p->emitlabel(begin);
	//p->emitlabel(after,p);
	//p->gen(p);
	return p;
}

Block* Parser::block(){
	Block * b = new Block();
	decl_constants();
	decl_variables();
	b->seq_paf = (Seq_PAF*)seq_paf();
	//decl_procandfunc();
	b->seq_stmt = (Seq *)compoundstmt();
	return b;
}

Stmt* Parser::compoundstmt(){
	try{
		match(T_BEGIN);
		Seq * c = new Seq(statement(),seq_statement()) ;
        match(T_END);
		return c;
	}
	catch(Exception *e){
		std::cout<<e->print()<<endl;
		if(++error_count >= max_errors)
			throw new ToomucherrorException();
		while(look->tag != T_ELSE && look->tag != T_WHILE && look->tag != T_SEMICOLON && look->tag != T_END && look->tag != T_DOT) 
			move();
/*		if(look->tag == T_END)
			goto F;
		else
			goto E;*/
	}
	return Stmt::Null;
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
			else
				throw new InappropriateException(look,lex->line);          // throw exception
		}
		default:
			;
	}
	if(look->tag != T_ELSE && look->tag != T_WHILE && look->tag != T_SEMICOLON && look->tag != T_END)
		throw new InappropriateException(look, lex->line);          // throw exception
	// look in follow .
	return Stmt::Null;
}

Stmt* Parser::inputstatement(){
	match(T_READ);
	match(T_OPENPARENTHESIS);
	std::vector<Id*> * list = new std::vector<Id*>();
	if(look->tag != T_IDENT)
		throw new TokenMatchException(look,T_IDENT,lex->line);         // throw exception
	Word * tok = (Word*)look;
	move();
	Node * nod = top->get(tok);
	if(Id * id = dynamic_cast<Id*>(nod))                 // check if const
	{
		if(id->isConst == true)
			throw new MiscellaneousException(id->to_string()+"is const",lex->line);
		list->push_back(id);
	}
	else
		throw new NoSuchIdentException(tok,lex->line);          // throw exception
	while(look->tag != T_CLOSEPARENTHESIS){
		match(T_COMMA);
		if(look->tag != T_IDENT)
			throw new TokenMatchException(look,T_IDENT,lex->line);         // throw exception
		tok = (Word *)look;
		move();
		Node * nod = top->get(tok);
		if(Id * id = dynamic_cast<Id*>(nod))               // check if const
		{
			if(id->isConst == true)
				throw new MiscellaneousException(id->to_string()+"is const",lex->line);
			list->push_back(id);
		}
		else
			throw new NoSuchIdentException(tok,lex->line);          // throw exception
	}
	move();
	return new Input(list);
}

Stmt* Parser::outputstatement(){                // a little bit tricky here...
	//static Tag_Set tag= {T_PLUS,T_MINUS,T_IDENT,T_NUMBER,T_OPENPARENTHESIS};
	STring * s;
	Expr * e;
	match(T_WRITE);
	match(T_OPENPARENTHESIS);
	if(look->tag == T_STRING){
		if(look->tag != T_STRING)
			throw new TokenMatchException(look,T_STRING,lex->line);              // throw exception
		s = (STring *) look;
		move();
		if(look->tag == T_CLOSEPARENTHESIS){
			move();
			return new Output(nullptr,s);
		}
		else if(look->tag == T_COMMA){
			move();
			if(look->tag == T_IDENT){
				Word * tok = (Word *)look;
				Node * nod = top->get(tok);
				if(Id * id = dynamic_cast<Id*>(nod)){
					if(id->isConst == true){
						Token* save = look;
						move();
						if(look->tag != T_CLOSEPARENTHESIS){
							push_back(look);
							look = save;
							goto L1;
						}
						else
							e = id;
					}
					else
						e = expr();
				}
				else
					e = expr();
			}
			else{
L1:				e = expr();
			}
	//		e = expr();
			match(T_CLOSEPARENTHESIS);
			return new Output(e,s);
		}
		else{
			throw new InappropriateException(look,lex->line);           // throw exception
		}
	}
	if(look->tag != T_PLUS && look->tag != T_MINUS && look->tag != T_IDENT && look->tag != T_NUMBER && look->tag != T_NUMBER && look->tag != T_OPENPARENTHESIS)
		throw new InappropriateException(look,lex->line);           // throw exception
	if(look->tag == T_IDENT){
		Word * tok = (Word *)look;
		Node * nod = top->get(tok);
		if(Id * id = dynamic_cast<Id*>(nod)){
			if(id->isConst == true){
				Token* save = look;
				move();
				if(look->tag != T_CLOSEPARENTHESIS){
					push_back(look);
					look = save;
					goto L2;
				}
				else
					e = id;
			}
			else
				e = expr();
		}
		else
			e = expr();
	}
	else{
L2:		e = expr();
	}
	//e = expr();
	match(T_CLOSEPARENTHESIS);
	return new Output(e,nullptr);
}

Stmt* Parser::assignstatement(){    // incomplete
	if(look->tag != T_IDENT)
		throw new TokenMatchException(look,T_IDENT,lex->line);         // throw exception
	Word * dest = (Word*)look;
	move();
	if(Id *id  = dynamic_cast<Id *>(top->get(dest))){
		if(look->tag == T_ASSIGN){       // check is function or variable,check if const
			move();
			Expr* e = expr();
			if(id->type == Type::Char && e->type != Type::Char)
				throw new TypeMatchException(e->type,id->type,lex->line);
			if(id->isConst == true)
				throw new MiscellaneousException("can not assign to const variable",lex->line);
			if(dynamic_cast<Array*>(id->type))
				throw new MiscellaneousException("can not assign to array variable itself",lex->line);
			return new Assign(id,e);
		}
		else if(look->tag == T_OPENBRACKET){
			/*move();
			Access* ac = new Access(id,id->type,expr());
			match(T_CLOSEBRACKET);*/
			Access * ac = offset(id);
			match(T_ASSIGN);
			Expr* e = expr();
			if(((Array*)(id->type))->of == Type::Char && e->type != Type::Char)
				throw new TypeMatchException(e->type,Type::Char,lex->line);
			return new AssignElem(ac,e);
		}
		else
			throw new InappropriateException(look,lex->line);           // throw exception
	}
	else
		throw new InappropriateException(look,lex->line);           // throw exception
	return Stmt::Null;      // should never excuted
}

Stmt* Parser::ifstatement(){
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
	if(look->tag != T_IDENT)
		throw new TokenMatchException(look,T_IDENT,lex->line);         // throw exception
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
			throw new InappropriateException(look,lex->line);           // throw exception
		}
		move();
		Expr* e2 = expr();
		match(T_DO);
		return new For(id,e1,is_to,e2,statement());
	}
	else 
		throw new NoSuchIdentException(tok,lex->line);          // throw exception
	return Stmt::Null;// should never excuted
}

Stmt* Parser::dowhilestatement(){
	match(T_DO);
	Stmt * s = statement();
	match(T_WHILE);
	return new DoWhile(condition(),s);
}

Stmt* Parser::callprocstatement(){
	if(look->tag != T_IDENT)
		throw new TokenMatchException(look,T_IDENT,lex->line);         // throw exception
	Word * w = (Word*)look;
	move();
	Node * nod = top->get(w);
	Expr * e;
	if(Proc * proc = dynamic_cast<Proc*>(nod)){
		Id * id;
		if(proc->paralist.size() == 0)
			return new Callproc(proc,new std::vector<Expr*>());
		else{
			std::vector<Expr*> *list = new std::vector<Expr*>();
			match(T_OPENPARENTHESIS);
			e = expr();
			id = proc->paralist[0];
			if(id->type != e->type)
				throw new TypeMatchException(e->type,id->type,lex->line);
			list->push_back(e);
			if(proc->paralist.size() == 1){
				match(T_CLOSEPARENTHESIS);
				return new Callproc(proc,list);
			}
			else{
				for(unsigned int i = 1; i < proc->paralist.size() ; i++){          // checking  type !
					match(T_COMMA);
					e = expr();
					id = proc->paralist[i];
					if(id->type != e->type)
						throw new TypeMatchException(e->type,id->type,lex->line);
					list->push_back(e);
				}
				match(T_CLOSEPARENTHESIS);
				return new Callproc(proc,list);
			}
		}
	}
	else {
		; // throw exception  no need . have checked before
	}
	return Stmt::Null;// should never excuted
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
		throw new TokenMatchException(look,T_END,lex->line);          // throw exception
	// look in follow .
	return Stmt::Null;
}
