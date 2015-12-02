#include "../include/Stmt.h"

Stmt* Stmt::Null = new Stmt();         // null statement generate emptyinstruction

void IfElse::gen(Program *p)
{
	label iffalse = newlabel();
	label after = newlabel();

	e = (Rel *)e->gen(p);
	emit(I_IFFALSE,new Arg_rel(e),nullptr,new Result_label(iffalse),p);
	s1->gen(p);
	emit(I_GOTO,nullptr,nullptr,new Result_label(after),p);
	//emit("goto L"+after);
	emitlabel(iffalse,p);
	s2->gen(p);
	emitlabel(after,p);
}
void If::gen(Program *p)
{
	label iffalse = newlabel();

	e = (Rel *)e->gen(p);
	emit(I_IFFALSE,new Arg_rel(e),nullptr,new Result_label(iffalse),p);
	s->gen(p);
	emitlabel(iffalse,p);
}
void DoWhile::gen(Program *p)
{
	label test = newlabel();
	emitlabel(test,p);
	s->gen(p);
	emit(I_IF,new Arg_rel(e),nullptr,new Result_label(test),p);
}

void For::gen(Program *p){
	Expr *x1 = e1->reduce(p);
	if(Constant* c = dynamic_cast<Constant*>(x1))
		emit(I_COPY,new Arg_int(c->c),nullptr,new Arg_id(id),p);
	else
		emit(I_COPY,new Arg_id((Id *)x1),nullptr,new Arg_id(id),p);
	label test = newlabel();
	label after = newlabel();
	emitlabel(test,p);
	Rel * r;
	if(is_to == true)
		r = new Rel(Word::le,id,e2);
	else
		r = new Rel(Word::ge,id,e2);
	emit(I_IFFALSE,new Arg_rel(r),nullptr,new Result_label(after),p);
	s->gen(p);
	if(is_to == true)
		emit(I_ADD,new Arg_id(id),new Arg_int(1),new Arg_id(id),p);
	else
		emit(I_MINUS,new Arg_id(id),new Arg_int(1),new Arg_id(id),p);
	emit(I_GOTO,nullptr,nullptr,new Result_label(test),p);
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
		emit(I_COPY,new Arg_int(c->c),nullptr,new Arg_id(id),p);
	else
		emit(I_COPY,new Arg_id((Id *)x),nullptr,new Arg_id(id),p);
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
	if(Constant *c1 = dynamic_cast<Constant*>(e1)){
		if(Constant *c2 =dynamic_cast<Constant*>(e2))
			emit(I_INDCOPY,new Arg_int(c1->c),new Arg_int(c2->c),new Arg_id(array),p);
		else
			emit(I_INDCOPY,new Arg_int(c1->c),new Arg_id((Id *)e2),new Arg_id(array),p);
	}
	else{
		if(Constant *c = dynamic_cast<Constant*>(e2))
			emit(I_INDCOPY,new Arg_id((Id *)e1),new Arg_int(c->c),new Arg_id(array),p);
		else
			emit(I_INDCOPY,new Arg_id((Id *)e1),new Arg_id((Id *)e2),new Arg_id(array),p);
	}
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
	for(unsigned int i = 0 ; i < idlist->size() ;i++ )
		emit(I_READ,nullptr,nullptr,new Arg_id(idlist->at(i)),p);
}
void Output::gen(Program *p){
	if(s != nullptr && e != nullptr){
		Expr *x = e->reduce(p);
		if(Constant * c = dynamic_cast<Constant *> (x))
			emit(I_WRITE,new Arg_string(s->line),new Arg_int(c->c),nullptr,p);
		else
			emit(I_WRITE,new Arg_string(s->line),new Arg_id((Id *)x),nullptr,p);
	}
	else if(s != nullptr)
			emit(I_WRITE,new Arg_string(s->line),nullptr,nullptr,p);
	else if(e != nullptr){
		Expr *x = e->reduce(p);
		if(Constant * c = dynamic_cast<Constant *> (x))
			emit(I_WRITE,nullptr,new Arg_int(c->c),nullptr,p);
		else
			emit(I_WRITE,nullptr,new Arg_id((Id *)x),nullptr,p);
	}
}
void Callproc::gen(Program *p){
	std::vector<Expr*> list;
	for(unsigned int i = 0; i < actuallist->size() ; i++)
		list.push_back((Expr *)actuallist->at(i)->reduce(p));
	emit(I_CALLPROC,new Arg_proc(prc),new Arg_int(actuallist->size()),nullptr,p);
	for(int i = actuallist->size()-1 ; i >= 0 ; i--)
		if(Constant *c1 = dynamic_cast<Constant*>(list[i]))
			emit(I_PARAM,new Arg_int(c1->c),nullptr,nullptr,p);
		else
			emit(I_PARAM,new Arg_id((Id *)list[i]),nullptr,nullptr,p);
}
