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
	Expr *x1 = e1->reduce(p);
	if(Constant* c = dynamic_cast<Constant*>(x1))
		emit(I_COPY,Arg_int(c->value),nullptr,Arg_id(id),p);
	else
		emit(I_COPY,Arg_id(x1),nullptr,Arg_id(id),p);
	label test = newlabel();
	label after = newlabel();
	emitlabel(test,p);
	Rel * r;
	if(is_to == true)
		r = new Rel(Word::le,id,e2);
	else 
		r = new Rel(Word::ge,id,e2);
	emit(I_IFFALSE,Arg_rel(r),nullptr,after,p);
	s->gen(p);
	if(is_to == true)
		emit(I_ADD,Arg_id(id),Arg_int(1),Arg_id(id),p);
	else
		emit(I_MINUS,Arg_id(id),Arg_int(1),Arg_id(id),p);
	emit(I_GOTO,nullptr,nullptr,test,p);
	emitlabel(after,p);
}

Assign::Assign(Id *id,Expr *e) {
	this->id = id;
	this->e = e;
	if( !check(id->type,e->type))
		; //throw exception
}
void Assign::gen(Program *p){
	Expr * x = e->reduce(p);
	if(Constant * c = dynamic_cast<Constant *> (x))
		emit(I_COPY,Arg_int(c->value),nullptr,Arg_id(id),p);
	else
		emit(I_COPY,Arg_id(x),nullptr,Arg_id(id),p);
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
	Expr* e2 = index->reduce(p);
	Expr* e1 = e->reduce(p);
	//emit(I_INDCOPY,Arg_id(e->reduce(p)),Arg_id(index->reduce(p)),Arg_id(array),p);
	if((Constant *c1 = dynamic_cast<Constant*>(e1)) && (Constant *c2 =dynamic_cast<Constant*>(e2)))
		emit(o,Arg_int(c1->value),Arg_int(c2->value),Arg_id(t),p);
	else if((Constant *c = dynamic_cast<Constant*>(e1)))
		emit(o,Arg_int(c->value),Arg_id(a->e2),Arg_id(t),p);
	else if((Constant *c = dynamic_cast<Constant*>(e2)))
		emit(o,Arg_id(a->e1),Arg_int(c->value),Arg_id(t),p);
	else
		emit(o,Arg_id(a->e1),Arg_id(e2),Arg_id(t),p);
}

void Seq::gen(Program *p){
	/*if(s1 == Stmt::Null) 
		s2->gen(p);
	else if(s2 == Stmt::Null)
		s1->gen(p);
	else if(s1 != Stmt::Null && s2 != Stmt::Null){
		label l = newlabel();
		s1->gen(p);
		emitlabel(l,p);
		s2->gen(p);
	}*/
	s1->gen(p);
	s2->gen(p);
}
void Input::gen(Program *p){
	for(int i = 0 ; i < idlist->size() ;i++ )
		emit(I_READ,nullptr,nullptr,Arg_id(idlist->at(i)),p);
}
void Output::gen(Program *p){
	if(s != nullptr && e != nullptr){
		Expr *x = e->reduce(p);
		if(Constant * c = dynamic_cast<Constant *> (x))
			emit(I_WRITE,Arg_string(s),Arg_int(c->value),nullptr,p);
		else
			emit(I_WRITE,Arg_string(s),Arg_id(x),nullptr,p);
	}
	else if(s != nullptr)
			emit(I_WRITE,Arg_string(s),nullptr,nullptr,p);
	else if(e != nullptr){
		Expr *x = e->reduce(p);
		if(Constant * c = dynamic_cast<Constant *> (x))
			emit(I_WRITE,nullptr,Arg_int(c->value),nullptr,p);
		else
			emit(I_WRITE,nullptr,Arg_id(x),nullptr,p);
	}
}
void Callproc::gen(Program *p){
	std::vector<Expr*> list;
	for(unsigned int i = 0; i < actuallist->size() ; i++)
		list.push_back(actuallist->at(i)->reduce());
	for(int i = actuallist->size()-1 ; i >= 0 ; i--)
		emit(I_PARAM,list[i],nullptr,nullptr,p);
	emit(I_CALL,Arg_proc(p),Arg_int(actuallist->size()));
}
