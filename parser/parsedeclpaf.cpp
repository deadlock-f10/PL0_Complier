#include "../include/Parser.h"
#include <unordered_set>
#include <vector>
#include <fstream>
Program* Parser::seq_paf(){
	//First = {T_PROCEDURE,T_FUNCTION,\epsilon};
	//static Tag_Set Follow = {T_BEGIN};
	try{
E:		switch (look->tag){
		 	case T_PROCEDURE:{
                 Proc * p = proc_decl();
		 		return new Seq_PAF(p,seq_paf());
		 		}
		 	case T_FUNCTION:{
                 Func * f = func_decl();
		 		return new Seq_PAF(f,seq_paf());
		 		}
		 	default:
		 		;
		}
		if(look->tag != T_BEGIN)
			throw TokenMatchException(look,T_BEGIN,lex->line);          // throw exception
		return Program::Null;// look in follow .
	}
	catch(const Exception &e){
		std::cout<<e.print()<<endl;
		if(++error_count >= max_errors)
			throw new ToomucherrorException();
		while(look->tag != T_SEMICOLON && look->tag != T_PROCEDURE && look->tag != T_FUNCTION && look->tag != T_BEGIN) 
			move();
		if(look->tag == T_SEMICOLON)
			goto E;
	}
}

Proc* Parser::proc_decl(){
	match(T_PROCEDURE);
	Program * save = top;
	if(look->tag != T_IDENT)
		throw TokenMatchException(look,T_IDENT,lex->line);          // throw exception
	Word * word = (Word *)look;
	move();
	Proc * p = new Proc(top,word,top->level + 1);
	top->put(word,p);              // something like a declaration rather than definitoin
	top = p;
	optional_para();
/*	//p->type = Type::Int;                                      just for consistency with Func 
	top->put(word,new Id(word,Type::Int,p->used,true,top->level));	  keep proc const, so it can't be assign a value like a function 
	top->used += Type::Int->width ;                              */
	match(T_SEMICOLON);
	p->block = block();
	match(T_SEMICOLON);
	top = save;
	return p;
}

Func* Parser::func_decl(){
	match(T_FUNCTION);
	Program * save = top;
	if(look->tag != T_IDENT)
		throw TokenMatchException(look,T_IDENT,lex->line);          // throw exception
	Word * word = (Word *)look;
	move();
	Func * f = new Func(top,word,top->level + 1);
	top->put(word,f);
	top = f;
	optional_para();
	match(T_COLON);
	if(look->tag != T_INT && look->tag != T_CHARACTER)
		throw InappropriateException(look,lex->line);             // throw exception . Cause function type must be basic type
	f->type = type();
	top->put(word,new Id(word,f->type,f->used,top->level));
	top->used += f->type->width;
	match(T_SEMICOLON);
	f->block = block();
	match(T_SEMICOLON);
	top = save;
	return f;
}

void Parser::optional_para(){
	//First = {T_OPENPARENTHESIS};
	//static Tag_Set Follow = {T_COLON,T_CONST,T_VAR,T_PROCEDURE,T_FUNCTION,T_BEGIN};
	switch (look->tag){
		case T_OPENPARENTHESIS:
			move();
			form_para();
			match(T_CLOSEPARENTHESIS);
			return ;
		default:
			;
	}
	if(look->tag != T_COLON && look->tag != T_CONST && look->tag != T_VAR && look->tag != T_PROCEDURE && look->tag != T_FUNCTION && look->tag !=T_BEGIN)
		throw InappropriateException(look,lex->line);          // throw exception
	// look in follow .
}

void Parser::form_para(){
	form_para_seg();
	seq_formpara_seg();
}

void Parser::form_para_seg(){
	bool isref = false;
	switch(look->tag){
		case T_VAR:
			isref = true;                 // fall through
			move();
		case T_IDENT:{
			std::vector<Word *> identifier_list;
			if(look->tag != T_IDENT)
				throw TokenMatchException(look,T_IDENT,lex->line);         // throw exception
			Word * tok = (Word *)look;
			move();
			identifier_list.push_back(tok);
			while(look->tag != T_COLON){
				match(T_COMMA);
				if(look->tag != T_IDENT)
					throw TokenMatchException(look,T_IDENT,lex->line);         // throw exception
				tok = (Word *)look;
				move();
				identifier_list.push_back(tok);
			}
			move();
			if(look->tag != T_INT && look->tag != T_CHAR)
				throw InappropriateException(look,lex->line);                 // throw exception , cause para must be of basic type;
			Type * t = type();
			Id * id;
			if(Proc * p = dynamic_cast<Proc*>(top)){
				for(unsigned int i = 0; i < identifier_list.size(); i++){
					tok = identifier_list[i];
					p->para_used -= t->width;
					id = new Id((Word*)tok,t,p->para_used,false,isref,p->level);
					p->paralist.push_back(id);
					p->put(tok,id);
				}
			}
			else if(Func *p = dynamic_cast<Func*>(top)){
				for(unsigned int i = 0; i < identifier_list.size(); i++){
					tok = identifier_list[i];
					p->para_used -= t->width;
					id = new Id((Word*)tok,t,p->para_used,false,isref,p->level);
					p->paralist.push_back(id);
					//p->used += t->width;
					p->put(tok,id);
				}
			}
			else{
				;                         // throw exception
			}
		}
		default:
			;
	}
}

void Parser::seq_formpara_seg(){
	//First = {T_SEMICOLON,\epsilon};
	//static Tag_Set Follow = {T_CLOSEPARENTHESIS};
	switch (look->tag){
		case T_SEMICOLON:
			move();
			form_para_seg();
			seq_formpara_seg();
			break;
		default:
			;
	}
	if(look->tag != T_CLOSEPARENTHESIS)
		throw TokenMatchException(look,T_CLOSEPARENTHESIS,lex->line);          // throw exception
	// look in follow .
}
