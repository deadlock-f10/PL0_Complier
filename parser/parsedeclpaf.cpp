
#include "../include/Parser.h"
#include <unordered_set>
#include <vector>

//typedef std::unordered_set<Tag> Tag_Set;

Program* Parser::seq_paf(){
	//First = {T_PROCEDURE,T_FUNCTION,\epsilon};
	//static Tag_Set Follow = {T_BEGIN};
	switch (look->tag){
		case T_PROCEDURE:
			return new Seq_PAF(proc_decl(),seq_paf());
		case T_FUNCTION:
			return new Seq_PAF(func_decl(),seq_paf());
		default:
			;
	}
	if(look->tag != T_BEGIN)
		;          // throw exception
	return Program::Null;// look in follow .
}

Proc* Parser::proc_decl(){
	match(T_PROCEDURE);
	Program * save = top;
	Word * word = (Word *)look;
	match(T_IDENT);
	Proc * p = new Proc(top,word,top->level + 1); 
	top = p;
	p->put(word,p);
	optional_para();
	match(T_SEMICOLON);
	p->block = block();
	match(T_SEMICOLON);
	top = save;
	return p;
}

Func* Parser::func_decl(){
	match(T_PROCEDURE);
	Program * save = top;
	Word * word = (Word *)look;
	match(T_IDENT);
	Func * f = new Func(top,word,top->level + 1); 
	top = f;
	optional_para();
	match(T_COLON);
	if(look->tag != T_INT && look->tag != T_CHARACTER)
		;             // throw exception . Cause function type must be basic type
	f->type = type();
	f->block = block();
	match(T_SEMICOLON);
	top->put(word,f);
	top->used += f->type->width ;
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
		;          // throw exception
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
			isref = true;
		case T_IDENT:{
			std::vector<Word *> identifier_list;
			match(T_IDENT);
			Word * tok = (Word *)look;
			identifier_list.push_back(tok);
			while(look->tag != T_COLON){
				match(T_COMMA);
				tok = (Word *)look;
				match(T_IDENT);
				identifier_list.push_back(tok);
			}
			move();
			if(look->tag != T_INT && look->tag != T_CHAR)
				;                 // throw exception , cause para must be of basic type;
			Type * t = type();
			Id * id;
			if(Proc * p = dynamic_cast<Proc*>(top)){
				for(unsigned int i = 0; i < identifier_list.size(); i++){
					tok = identifier_list[i];
					id = new Id((Word*)tok,t,top->used,false,isref);
					p->paralist.push_back(id);
					p->used += t->width;
					p->put(tok,id);
				}
			}
			else if(Func *p = dynamic_cast<Func*>(top)){
				for(unsigned int i = 0; i < identifier_list.size(); i++){
					tok = identifier_list[i];
					id = new Id((Word*)tok,t,top->used,false,isref);
					p->paralist.push_back(id);
					p->used += t->width;
					p->put(tok,id);
				}
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
		;          // throw exception
	// look in follow .
}
