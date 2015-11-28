#include "OP.h"
#include "Tag.h"
#include <string>
#include <vector>
#ifndef  QUADRUPLE_H
#define QUADRUPLE_H
class Arg1{
	virtual std::string to_string(){;}	
};
class Arg2{
	virtual std::string to_string(){;}
};
class Result{
	virtual std::string to_string(){;}
};
class Func;
class Proc;
class Id;
class Rel;
class Arg_id : public Arg1 ,Arg2,Result{             // maybe temp ?
	public :
		Id * id;
		Arg_id(Id *x){id = x;}
		std::string to_string(){return id->to_string();}
};
class Arg_string : public Arg1{
	public :
		std::string str;
		Arg_string(std::string s){str = s;}
		std::string to_string(){return str;}
};
class Arg_int : public Arg1 , Arg2{
	public :
		int value;
		Arg_int(int x) { value = x;}
		std::string to_string(){return patch::to_string(value);}
};
class Arg_rel : public Arg1{
	public :
		Rel * relation;
		Arg_rel(Rel * r) {relation = r;}
		std::string to_string(){return relation->toString();}
};
class Arg_func : public Arg1{
	public :
		Func * func;
		Arg_func(Func *f){func = f;}
		std::string to_string(){return func->name->toString();}
};
class Arg_proc : public Arg1{
	public :
		Proc * proc;
		Arg_proc(Proc *p){proc = p;}
		std::string to_string(){return proc->name->toString();}
};
class Result_label : public Result{
	public :
		std::string label;
		Result_label(std::string s){label = s;}
		std::string to_string(){return label;}
};
class Quadruple{
	public :
	std::vector<std::string> *labellist;
	OP	 op;
	Arg1 * arg1;
	Arg2 * arg2;
	Result * result;
	Quadruple(std::vector<std::string> *l,OP Op, Arg1 * a1, Arg2* a2, Result *r){labellist = new std::vector<std::string>(*l);op = Op;arg1=a1;arg2=a2;result=r;}
	std::string to_string();
};
#endif
