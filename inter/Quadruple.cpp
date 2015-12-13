#include "../include/Quadruple.h"
#include "../include/PAF.h"
#include "../include/Expr.h"
std::string OptoString[I_MINUS + 1] = {"read","write","if", "iffalse","goto", "end","invoke","param", "callproc", "callfunc", "copy", "copyind","indcopy", "*", "/", "+", "-"};

int Arg_string::num=0;
std::string Quadruple::to_string(){
	std::string s = "";
	if(labellist != nullptr)
        for(unsigned int i = 0 ; i < labellist->size();i++)
            s += (labellist->at(i)+": ");
/*	s += "\t"+OptoString[op];
	if(arg1 != nullptr)
		s += "\t"+arg1->to_string();
	if(arg2 != nullptr)
		s += "\t"+arg2->to_string();
	if(result != nullptr)
		s += "\t"+result->to_string();
	return s;*/
	switch(op){
		case I_READ:
			s += "\t" + OptoString[op];
			s += " " + result->to_string();
			break;
		case I_WRITE:
			s += "\t" + OptoString[op];
			if(arg1 != nullptr)
				s += "\""+arg1->to_string()+"\"";
			if(arg2 != nullptr)
				s += " " + arg2->to_string();
			break;
		case I_IF:
		case I_IFFALSE:
			s += "\t" + OptoString[op];
			s += " " + arg1->to_string();
			s += " goto " + result->to_string();
			break;
		case I_PARAM:
			s += "\t" + OptoString[op];
			s += " " + arg1->to_string();
			break;
		case I_INVOKE:
			s += "\t" + OptoString[op];
			s += " " + arg1->to_string();
			s += " " + arg2->to_string();
			break;
		case I_CALLPROC:
			s += "\t" + OptoString[op];
			s += " " + arg1->to_string();
			s += " " + arg2->to_string();
			break;
		case I_CALLFUNC:
			s += "\t" + result->to_string();
			s += " = " + OptoString[op];
			s += " " + arg1->to_string();
			s += " " + arg2->to_string();
			break;
		case I_COPY:
			s += "\t" + result->to_string();
			s += " = " + arg1->to_string();
			break;
		case I_COPYIND:
			s += "\t" + result->to_string();
			s += " = " + arg1->to_string();
			s += "["+arg2->to_string()+"]";
			break;
		case I_INDCOPY:
			s += "\t" + result->to_string();
			s += "["+arg2->to_string()+"]";
			s += " = " + arg1->to_string();
			break;
		case I_MULT:
		case I_DIV:
		case I_ADD:
		case I_MINUS:
			s += "\t" + result->to_string();
			s += " = " + arg1->to_string();
			s += " " + OptoString[op];
			s += " " + arg2->to_string();
			break;
		case I_GOTO:
			s += "\t" + OptoString[op];
			s += " " + result->to_string();
			break;
		case I_END:
			s += "\t" + OptoString[op];
	}
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
std::string Arg_ace::to_string(){
	return ace->to_string();
}
