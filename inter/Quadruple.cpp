#include "../include/Quadruple.h"
#include "../include/PAF.h"
#include "../include/Expr.h"

std::string Quadruple::to_string(){
	std::string s = "";
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
