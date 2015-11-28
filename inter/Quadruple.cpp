#include "../include/Quadruple.h"
#include "../include/PAF.h"
#include "../include/Expr.h"
std::string OptoString[I_MINUS + 1] = {"read","write","if", "iffalse","goto", "end","param", "callproc", "callfunc", "copy", "copyind","indcopy", "*", "/", "+", "-"};

std::string Quadruple::to_string(){
	std::string s = "";
	if(labellist != nullptr)
        for(unsigned int i = 0 ; i < labellist->size();i++)
            s += (labellist->at(i)+": ");
	s += "\t"+OptoString[op];
	if(arg1 != nullptr)
		s += "\t"+arg1->to_string();
	if(arg2 != nullptr)
		s += "\t"+arg2->to_string();
	if(result != nullptr)
		s += "\t"+result->to_string();
	return s;
}
Quadruple::Quadruple(std::vector<std::string> *l,OP Op, Arg1 * a1, Arg2* a2, Result *r)
{
    if(l != nullptr)
        labellist = new std::vector<std::string>(*l);
    else
        labellist = nullptr;
    op = Op;arg1=a1;arg2=a2;result=r;
}

std::string Arg_id::to_string(){
	return id->to_string();
}
std::string Arg_string::to_string(){
	return str;
}
std::string Arg_int::to_string(){
	return patch::to_string(value);
}
std::string Arg_rel::to_string(){
	return relation->to_string();
}
std::string Arg_func::to_string(){
	return func->name->to_string();
}
std::string Arg_proc::to_string(){
	return proc->name->to_string();
}
std::string Result_label::to_string(){
	return label;
}