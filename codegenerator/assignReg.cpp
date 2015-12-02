#include "../include/Codegen.h"
#include "Regdef.h"
#include <vector>

std::string regto_string[32] = {};
inline void Bblockgenerator::emit(std::string s){
	instrlist.push_back(s);
}

void Bblockgenerator::gen(){
	for(std::vector<Quadruple*>::iterator it = block->instrlist.begin(); it != block->instrlist.end();it++){
		getReg(*it);
		chooseInstr(*it);
	}
}
void Bblockgenerator::getReg(Quadruple *q){
	switch(q->op){
		case I_CALLPROC:
		case I_GOTO:
		case I_END:
			break;
		case I_READ:
			{
				Arg_id * argid = dynamic_cast<Arg_id*>(q->result);
				findstore(id);
				break;
			}
		case I_WRITE:
			{
				if(Id * id = dynamic_cast<Id*>(q->arg2))
					findload(id,Reg_Descripter::t9);
				break;
			}
		case I_IFFALSE:
		case I_IF:
			{
				Rel *r = (Rel *)q->arg1;
				if(Id * id1 = dynamic_cast<Id*>(r->e1))
					findload(id1,Reg_Descripter::t8);				
				if(Id * id2 = dynamic_cast<Id*>(r->e2))
					findload(id2,Reg_Descripter::t9);
				break;
			}
		case I_PARAM:
			{
				if(Id * id = dynamic_cast<Id*>(q->arg1))
					findload(id,Reg_Descripter::t8);
				break;
			}
		case I_CALLFUNC:
			{
				if(Id * id = dynamic_cast<Id*>(q->result))
					findstore(id);
				break;
			}
		case I_COPY:
			{
				if(Id * id = dynamic_cast<Id*>(q->arg1)){
					findload(id,Reg_Descripter::t8);
				}
				Id * id1 = dynamic_cast<Id*>(q->result);
				findstore(id1);
				break;
			}
		case I_COPYIND:
			{
				Id * id = dynamic_cast<Id*>(q->arg1);
				loadarrayaddr(id);
				if(Id * id2 = dynamic_cast<Id*>(q->arg2))
					findload(id2,Reg_Descripter::t9);
				Id * id1 = dynamic_cast<Id*>(q->result);
				findstore(id1);
				break;
			}
		case I_INDCOPY:
			{
				Id * id = dynamic_cast<Id*>(q->result);
				loadarrayaddr(id);
				if(Id * id1 = dynamic_cast<Id*>(q->arg1))
					findload(id1,Reg_Descripter::t8);
				if(Id * id2 = dynamic_cast<Id*>(q->arg2))
					findload(id2,Reg_Descripter::t9);
				break;
			}
		case I_ADD:
		case I_MULT:
		case I_DIV:
		case I_MINUS:
			{
				Id * id = dynamic_cast<Id*>(q->result);
				findstore(id);
				if(Id * id1 = dynamic_cast<Id*>(q->arg1))
					findload(id1,Reg_Descripter::t8);
				if(Id * id2 = dynamic_cast<Id*>(q->arg2))
					findload(id2,Reg_Descripter::t9);
				break;
			}
	}
}
void Bblockgenerator::findstore(Arg_id * argid){    //find register to store
	Id * id = argid->id;
	Addr_Descripter *ad = addr_des.find(id);
	Reg_Descripter *x = reg_des.getAvail();
	if(x != nullptr){
		x.assignId(ad);
		ad.deleteReg();
		ad.assignReg(x);
	}
	else{
		ad.deleteReg();
		ad.addReg(Reg_Descripter::k0);
	}
	ad.invalidatestack();
}
void Bblockgenerator::loadarrayaddr(Arg_id * argid){
	Id * id = argid->id;
		if(id->level == p->level)
			emit("lw "+regto_string[R_K1]+patch::to_string(id->offset)+"("+regto_string[R_FP]+")");
		else{
			if(Proc *proc = dynamic_cast<Proc*>(p)){
				int m = 4 * (id->level + 3) + proc->para_used;
			emit("lw "+regto_string[R_K1]+" "+patch::string(-m)+"("+regto_string[R_FP]+")");
			}
			else if(Func *func = dynamic_cast<func*>(p)){
				int m = 4 * (id->level + 3) + func->para_used;
			emit("lw "+regto_string[R_K1]+" "+patch::string(-m)+"("+regto_string[R_FP]+")");
			}
			emit("lw "+regto_string[R_K1]+patch::to_string(id->offset)+"("+regto_string[R_K1]+")");
		}
}
void Bblockgenerator::loadvariable(Arg_id * argid,Register r){
	// be cautious when id is a temp , althought it's very rare. 
	// watch out for var
	Id * id = argid->id;
	if(Temp *t = dynamic_cast<Id*>(id)){
		Addr_Descripter *adt = map.find(t);
		emit("lw "+regto_string[r]+patch::to_string(adt->temponstack)+"("+regto_string[R_FP]+")");
	}
	else{
		if(id->level == p->level)
			emit("lw "+regto_string[r]+patch::to_string(id->offset)+"("+regto_string[R_FP]+")");
		else{
			if(Proc *proc = dynamic_cast<Proc*>(p)){
				int m = 4 * (id->level + 3) + proc->para_used;
			emit("lw "+regto_string[R_K1]+" "+patch::string(-m)+"("+regto_string[R_FP]+")");
			}
			else if(Func *func = dynamic_cast<func*>(p)){
				int m = 4 * (id->level + 3) + func->para_used;
			emit("lw "+regto_string[R_K1]+" "+patch::string(-m)+"("+regto_string[R_FP]+")");
			}
			emit("lw "+regto_string[r]+" "+patch::to_string(id->offset)+"("+regto_string[R_K1]+")");
		}
	}
}
void Bblockgenerator::storevariable(Arg_id * argid,Register r){

}
void Bblockgenerator::findload(Arg_id *argid,Reg_Descripter *backup){
	// find register to load
	Id * id = argid->id;
	Addr_Descripter *ad = addr_des.find(id);
	Reg_Descripter *r = ad.getReg();
	if(r == nullptr){
		Reg_Descripter *x = reg_des.getAvail();
		if(x != nullptr){
			x.assignId(ad);
			ad.deleteReg();
			ad.assignReg(x);
			loadvariable(id,x->r);
		}
		else{
			//ad.clear();					
			ad.deleteReg();
			ad.assignReg(backup);
			loadvariable(id,backup->r);
		}
	}
}

void Addr_Des::addtomap(Id *i)            // temp 
{
	if(Temp *t  =  dynamic_cast<Temp*>(i)){
		Addr_Descripter ad = new Addr_Descripter(t);
		map.insert(std::make_pair(t,ad));
	}
	else{
		Addr_Descripter ad = new Addr_Descripter(i);
		ad->addReg(R_ZERO);
		map.insert(std::make_pair(i,ad));
	}
}

void Addr_Descripter * Addr_Descripter::find(Id *i){
	std::unordered_map<Id*,Addr_Descripter*>::iterator it = map.find(i);
	return it->second;
}
Reg_Descripter* Reg_Des::getAvail(){
	if(!Available.empty())
	{
		Reg_Descripter *x = Available.back();
		Available.pop_back();
		return x;
	}
	else 
		return nullptr;
}
