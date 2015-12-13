#include "../include/Expr.h"
#include "../include/Quadruple.h"


//void helper::emitlabel(label l,Program *p) {p->addlabel(l);}
void helper::emitlabel(std::string s, Program *p) {p->addlabel(s);}
void helper::emit(OP op,Arg1* arg1 , Arg2* arg2,Result * result,Program *p){p->addinstr(op,arg1 , arg2, result);}

int Node::labels = 0;
int Temp::count = 0;
Expr* Op::reduce(Program*p){
	return this;
}

Expr* Callfunc::reduce(Program *p){
	std::vector<Expr*> list;
	for(unsigned int i = 0; i < actuallist->size() ; i++){
		Expr* e = actuallist->at(i);
		if(Access * ace = dynamic_cast<Access*>(e))
			list.push_back(ace->gen(p));
		else
			list.push_back((Expr*)(e->reduce(p)));
	}
	emit(I_INVOKE,new Arg_func(f),new Arg_int(actuallist->size()),nullptr,p);
	for(int i = actuallist->size()-1 ; i >= 0 ; i--)
		if(Constant *c1 = dynamic_cast<Constant*>(list[i]))
			emit(I_PARAM,new Arg_int(c1->c),nullptr,nullptr,p);
		else if(Access * ace = dynamic_cast<Access*>(list[i]))
			emit(I_PARAM,new Arg_ace(ace),nullptr,nullptr,p);
		else
			emit(I_PARAM,new Arg_id((Id *)list[i]),nullptr,nullptr,p);
	Temp *t = new Temp(type,p->level);
	emit(I_CALLFUNC,new Arg_func(f),new Arg_int(actuallist->size()),new Arg_id(t),p);
	return t;
}

Arith::Arith(Token* tok , Expr *x1 , Expr* x2) : Op(tok, nullptr){
	e1 = x1;
	e2 = x2;
	type = Type::max(e1->type,e2->type);
	if( type == nullptr)
		; //throw out exception
}
class Constant;
Expr* Arith::reduce(Program *p){
	Arith * a = (Arith *)gen(p);
	OP o;
	switch(op->tag){
		case T_MINUS:
			o = I_MINUS;
			break;
		case T_MULT:
			o = I_MULT;
			break;
		case T_PLUS:
			o = I_ADD;
			break;
		case T_DIV:
			o = I_DIV;
			break;
		default :
			;      // will never excuted
	}
	Temp *t = new Temp(type,p->level);
	if(Constant * c1 = dynamic_cast<Constant*>(a->e1)){
		if(Constant * c2 = dynamic_cast<Constant*>(a->e2))
			emit(o,new Arg_int(c1->c),new Arg_int(c2->c),new Arg_id(t),p);
		else 
			emit(o,new Arg_int(c1->c),new Arg_id((Id *)a->e2),new Arg_id(t),p);
	}
	else{
		if(Constant * c = dynamic_cast<Constant*>(a->e2))
			emit(o,new Arg_id((Id *)a->e1),new Arg_int(c->c),new Arg_id(t),p);
		else
			emit(o,new Arg_id((Id *)a->e1),new Arg_id((Id *)a->e2),new Arg_id(t),p);
	}
	return t;
}
Unary::Unary(Token *tok, Expr *x) : Op(tok,nullptr) {
	e = x;
	type = Type::max(Type::Char , x->type);
	if(type == nullptr) 
		; //throw exception;
}
Expr* Unary::reduce(Program *p){               // return constant if u->e is a constant. otherwise return a temp
	Unary * u = (Unary*)gen(p);
	if(Constant *c = dynamic_cast<Constant*>(u->e)){
		c->c *= (-1);
		return c;
	}
	Temp* t= new Temp(type,p->level);
	emit(I_MULT,new Arg_int(-1),new Arg_id((Id*)u->e),new Arg_id(t),p);
	return t;
	//emit(I_COPY,Arg_id())
}

Rel::Rel (Token *t ,Expr *x1 , Expr *x2) : Expr(t,nullptr){
	e1 = x1;
	e2 = x2;
	if( !check(e1->type,e2->type))
		; // throw exception
	type = Type::Int; 
}
Expr* Rel::gen(Program *p){
	return new Rel(op,e1->reduce(p),e2->reduce(p));
}

bool Rel::check(Type *p1 , Type *p2){
	if(!Type::numeric(p1) || !Type::numeric(p2))
		return false;
	else 
		return true;
}

Expr* Access::reduce(Program *p){
	Access *a = (Access *)gen(p);
	Temp *t = new Temp(type,p->level);
	if(Constant *c = dynamic_cast<Constant*>(a->index))
		emit(I_COPYIND,new Arg_id((Id *)a->array),new Arg_int(c->c),new Arg_id(t),p);
	else 
		emit(I_COPYIND,new Arg_id((Id *)a->array),new Arg_id((Id *)a->index),new Arg_id(t),p);
	return t;
}
