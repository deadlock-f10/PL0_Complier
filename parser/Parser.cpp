#include "Parser.h"
#include <unordered_set>
#include <queue>

typedef std::unordered_set<Tag> Tag_Set;


void Parser::match(Tag t){
	if(look->tag = t)
		move();
	else
		;                  // throw exception
}

Program* Parser::program(){
	Program * p  = new Program();
	top = p;
	p->block = block();	
	match(T_DOT);
	label begin = s.newlabel();
	label after = s.newlabel();
	p.emitlabel(begin);
	p.gen(begin , after);
	p.emitlabel(after);
	return p;
}

Block* Parser::block(){
	Block * b = new Block(top);
	decl_constants();
	decl_variables();
	b->seq_paf = seq_paf();
	//decl_procandfunc();
	b->seq_stmt = compoundstmt();
	return s;
}

Program* Parser::seq_paf(Program *p){
	//First = {T_PROCEDURE,T_FUNCTION,\epsilon};
	static Tag_Set Follow = {T_BEGIN};
	switch (look->tag){
		case T_PROCEDURE:
			return new Seq_PAF(proc_decl(),seq_paf());
		case T_FUNCTION:
			return new Seq_PAF(func_decl(),seq_paf());
	}
	auto search = Follow.find(look.tag);
	if(search == Follow.end())
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
	if(look->tag != T_INT && look->tag != Char)
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
	static Tag_Set Follow = {T_COMMA,T_CONST,T_VAR,T_PROCEDURE,T_FUNCTION,T_BEGIN};
	switch (look->tag){
		case T_OPENPARENTHESIS:
			move();
			form_para();
			match(T_CLOSEPARENTHESIS);
			return ;
	}
	auto search = Follow.find(look.tag);
	if(search == Follow.end())
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
		case T_IDENT:
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
			if(look->tag != T_INT && look->tag != T_CHAR)
				;                 // throw exception , cause para must be of basic type;
			Type * t = type();
			Id * id;
			while(identifier_list.empty() == false){
				tok = identifier_list.pop();				
				id = new Id((Word*)tok,t,top->used,false,isref);
				top->used += t->width;
				top.put(tok,id);
			}
	}

}

void Parser::seq_formpara_seg(){
	//First = {T_SEMICOLON,\epsilon};
	static Tag_Set Follow = {T_CLOSEPARENTHESIS};
	switch (look->tag){
		case T_SEMICOLON:
			move();
			form_para_seg();
			seq_formpara_seg();
			break;
	}
	auto search = Follow.find(look.tag);
	if(search == Follow.end())
		;          // throw exception
	// look in follow .
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
	id = new Id((Word*)tok,t,top->used);  // Should assign Const.Value to this ID
	top->used += t->width;
	top->put(tok,id);
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
		id = new Id((Word*)tok,t,top->used);
		top->used += t->width;
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
