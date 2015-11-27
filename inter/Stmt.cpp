#include "../include/Stmt.h"

Stmt* Stmt::Null = new Stmt();         // null statement generate emptyinstruction

void IfElse::gen(Program *p)
{
	label iffalse = newlabel();
	label after = newlabel();

	//e->jumping(0,iffalse);
	emit(I_IFFALSE,Arg_rel(e),nullptr,iffalse,p);
	s1->gen(p);
	emit(I_GOTO,nullptr,nullptr,after,p);
	//emit("goto L"+after);
	emitlabel(iffalse,p);
	s2->gen(p);
}
void If::gen(Program *p)
{
	label iffalse = newlabel();								
	s->gen(p);
	emitlabel(iffalse,p);
}
void DoWhile::gen(Program *p)
{
	label test = newlabel();
	emitlabel(test,p);
	s->gen(p);
	emit(I_IF,Arg_rel(e),nullptr,test,p);
}

void For::gen(Program *p){
	
}

Assign::Assign(Id *id,Expr *e) {
	this->id = id;
	this->e = e;
	if( !check(id->type,e->type))
		; //throw exception
}
void Assign::gen(Program *p){

}
bool Assign::check(Type *a,Type *b){
	if(!Type::numeric(a) || !Type::numeric(b))
		return false;
	else if(a == Type::Char && b == Type::Int)
		return false;
	else 
		return true;
}
AssignElem::AssignElem(Access *x , Expr *y){
	array = x->array ;
	index = x->index ; 
	e = y; 
	if(!check(array->type,y->type))
		;            //throw exception
}
bool AssignElem::check(Type *a ,Type *b){
	return true;	        // unimplemented
}
void AssignElem::gen(Program *p){
	//std::string s1 = index->reduce()->toString();
	//std::string s2 = e->reduce()->toString();
	//emit(array->toString() + "["+ s1 +"]" +" = " + s2);
	emit(I_INDCOPY,Arg_id(e->reduce(p)),Arg_id(index->reduce(p)),Arg_id(array),p);
}

void Seq::gen(Program *p){
	if(s1 == Stmt::Null) 
		s2->gen(p);
	else if(s2 == Stmt::Null)
		s1->gen(p);
	else if(s1 != Stmt::Null && s2 != Stmt::Null){
		label l = newlabel();
		s1->gen(p);
		emitlabel(l,p);
		s2->gen(p);
	}
}
void Input::gen(Program *p){
	
}
void Output::gen(Program *p){

}
void Callproc::gen(Program *p){
	std::vector<Expr*> list;
	for(unsigned int i = 0; i < actuallist->size() ; i++)
		list.push_back(actuallist->at(i)->reduce());
	for(int i = actuallist->size()-1 ; i >= 0 ; i--)
		emit(I_PARAM,list[i],nullptr,nullptr,p);
	emit(I_CALL,Arg_proc(p),Arg_int(actuallist->size()));
}
