#include "../include/Stmt.h"

Stmt* Stmt::Null = new Stmt();

void IfElse::gen(label before, label after)
{
	label iftrue = newlabel();
	label iffalse = newlabel();

	e->jumping(0,iffalse);
	emitlabel(iftrue);
	s1->gen(iftrue,after);
	emit("goto L"+after);
	emitlabel(iffalse);
	s2->gen(iffalse,after);
}
void If::gen(label before, label after)
{
	label iftrue = newlabel();								
	e->jumping(0,after);
	emitlabel(iftrue);
	s->gen(iftrue,after);
}
void DoWhile::gen(label before, label after)
{
	label test = newlabel();
	s->gen(before,test);
	emitlabel(test);
	e->jumping(before,0);
}

Assign::Assign(Id *id,Expr *e) {
	this->id = id;
	this->e = e;
	if( !check(id->type,e->type))
		; //throw exception
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
void AssignElem::gen(label begin,label after){
	std::string s1 = index->reduce()->toString();
	std::string s2 = e->reduce()->toString();
	emit(array->toString() + "["+ s1 +"]" +" = " + s2);
}

void Seq::gen(label begin , label after){
	if(s1 == Stmt::Null) 
		s2->gen(begin,after);
	else if(s2 == Stmt::Null)
		s1->gen(begin,after);
	else if(s1 != Stmt::Null && s2 != Stmt::Null){
		label l = newlabel();
		s1->gen(begin,l);
		emitlabel(l);
		s2->gen(l,after);
	}
}
