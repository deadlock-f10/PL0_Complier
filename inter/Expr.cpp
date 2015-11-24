#include "../include/Expr.h"


label Node::labels = 0;
int Temp::count = 0;
void Expr::jumping(label iftrue , label iffalse){
	emitjumps(toString(), iftrue , iffalse);
}
void Expr::emitjumps(std::string test , label iftrue , label iffalse){
	if(iftrue != 0 && iffalse != 0){
		emit("if "+test+ "goto L"+ patch::to_string(iftrue));
		emit("goto L" + iffalse);
	}
	else if(iffalse != 0){
		emit("iffalse "+test + "goto L" + patch::to_string(iffalse));
	}
	else if(iftrue != 0){
		emit("if " + test + "goto L" +patch::to_string(iftrue));
	}
}

Expr* Op::reduce(){
	Expr *x = gen();	
	Temp *t = new Temp(type);
	emit(t->toString() + "=" + x->toString());
	return t;
}

Arith::Arith(Token* tok , Expr *x1 , Expr* x2) : Op(tok, nullptr){
	e1 = x1;
	e2 = x2;
	type = Type::max(e1->type,e2->type);
	if( type == nullptr)
		; //throw out exception
}

Unary::Unary(Token *tok, Expr *x) : Op(tok,nullptr) {
	e = x;
	type = Type::max(Type::Char , x->type);
	if(type == nullptr) 
		; //throw exception;
}

Rel::Rel (Token *t ,Expr *x1 , Expr *x2) : Expr(t,nullptr){
	e1 = x1;
	e2 = x2;
	if( !check(e1->type,e2->type))
		; // throw exception
	type = Type::Int; 
}

bool Rel::check(Type *p1 , Type *p2){
	if(!Type::numeric(p1) || !Type::numeric(p2))
		return false;
	else 
		return true;
}
