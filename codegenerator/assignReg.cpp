#include "../include/Codegen.h"
#include "../include/Regdef.h"
#include "../include/Expr.h"
#include <vector>
#include <string>
extern ofstream targetcode;
//extern std::string regto_string[32] = {};
Reg_Descripter* Reg_Descripter::t8 = new Reg_Descripter(R_T8);
Reg_Descripter* Reg_Descripter::t9 = new Reg_Descripter(R_T9);
Reg_Descripter* Reg_Descripter::k0 = new Reg_Descripter(R_K0);

void Bblockgenerator::emit(std::string s){
	//targetcode<<s.length()<<endl;
    block->list.push_back(s);
}

void Bblockgenerator::print(){
    //targetcode<<instrlist.size()<<endl;
 //   instrlist.push_back("mk");
	for(std::vector<std::string>::iterator it = instrlist.begin(); it != instrlist.end();it++)
		targetcode<<*it<<endl;
}

void Bblockgenerator::gen(){
	emit(block->label+":");
	backwardscan();
	for(std::vector<Quadruple*>::iterator it = block->instrlist.begin(); it != block->instrlist.end();it++){
		getReg(*it);
		chooseInstr(*it);
	}
}
void Bblockgenerator::getReg(Quadruple *q){
	switch(q->op){
		case I_CALLPROC:
		case I_INVOKE:
		case I_GOTO:
		case I_END:
		case I_PARAM:
			break;
		case I_READ:
			{
				Arg_id * result = dynamic_cast<Arg_id*>(q->result);
					findstore(result);
				break;
			}
		case I_WRITE:
			{
				if(Arg_id * id2 = dynamic_cast<Arg_id*>(q->arg2))
						findload(id2,Reg_Descripter::t9);
				break;
			}
		case I_IFFALSE:
		case I_IF:
			{
				Rel *r = ((Arg_rel*)q->arg1)->relation;
				if(Id * id1 = dynamic_cast<Id*>(r->e1))
					findload(new Arg_id(id1),Reg_Descripter::t8);
				if(Id * id2 = dynamic_cast<Id*>(r->e2))
					findload(new Arg_id(id2),Reg_Descripter::t9);
				break;
			}
		case I_CALLFUNC:
			{
	/*			Arg_id * result = dynamic_cast<Arg_id*>(q->result);
					findstore(result);*/
				break;
			}
		case I_COPY:
			{
				if(Arg_id * id1 = dynamic_cast<Arg_id*>(q->arg1))
					 findload(id1,Reg_Descripter::t8);
				Arg_id * result = dynamic_cast<Arg_id*>(q->result);
					findstore(result);
				break;
			}
		case I_COPYIND:
			{
				if(Arg_id * id2 = dynamic_cast<Arg_id*>(q->arg2)) //use k1 as intermediate
					 findload(id2,Reg_Descripter::t9);
				Arg_id* arg1 = dynamic_cast<Arg_id*>(q->arg1);
				loadarrayaddr(arg1);                                 //store addr in k1
				Arg_id * id1 = dynamic_cast<Arg_id*>(q->result);
				findstore(id1);
				break;
			}
		case I_INDCOPY:
			{
				if(Arg_id * id1 = dynamic_cast<Arg_id*>(q->arg1))
					 findload(id1,Reg_Descripter::t8);
				if(Arg_id * id2 = dynamic_cast<Arg_id*>(q->arg2))
					 findload(id2,Reg_Descripter::t9);
				Arg_id* arg1 = dynamic_cast<Arg_id*>(q->result);
				loadarrayaddr(arg1);  //store addr in k1
				break;
			}
		case I_ADD:
		case I_MULT:
		case I_DIV:
		case I_MINUS:
			{
		/*		Arg_id * result = dynamic_cast<Arg_id*>(q->result);                //  findstore for result first so that we can deal with i = i + 1
					findstore(result);*/
				if(Arg_id * id1 = dynamic_cast<Arg_id*>(q->arg1))
					 findload(id1,Reg_Descripter::t8);
				if(Arg_id * id2 = dynamic_cast<Arg_id*>(q->arg2))
					 findload(id2,Reg_Descripter::t9);
				Arg_id * result = dynamic_cast<Arg_id*>(q->result);                //  findstore for result first so that we can deal with i = i + 1
					findstore(result);
				break;
			}
	}
}
void Bblockgenerator::findload(Arg_id *argid,Reg_Descripter *backup){//use  k1
	// find register to load
	Id * id = argid->id;
	Addr_Descripter *ad = addr_des.find(id);
	Reg_Descripter *r = ad->getReg();
	if(r == nullptr){
		Reg_Descripter *x = reg_des.getAvail();
		if(x != nullptr){
			x->assignId(ad);
			ad->deleteReg();
			ad->assignReg(x);
			loadvariable(argid,x->r);
		}
		else{
			//ad.clear();
			ad->deleteReg();
			ad->assignReg(backup);
			loadvariable(argid,backup->r);
		}
	}
}
void Bblockgenerator::findstore(Arg_id * argid){    //find register to store
	Id * id = argid->id;
	Addr_Descripter *ad = addr_des.find(id);
	if(ad->getReg() == nullptr){
		Reg_Descripter *x = reg_des.getAvail();
		if(x != nullptr){
			x->assignId(ad);
			ad->deleteReg();
			ad->assignReg(x);
		}
		else{
			ad->deleteReg();
			ad->assignReg(Reg_Descripter::k0);
		}
	}
	//ad->invalidatestack();
}
void Bblockgenerator::loadarrayaddr(Arg_id * argid){//store addr in k1
	Id * id = argid->id;
		if(id->level == prog->level)
			emit("lw "+regto_string[R_K1]+" "+ patch::to_string(-(id->offset))+"($fp)");
		else{
			int m = -(4 * (prog->level + 3 - id->level));
			emit("lw "+regto_string[R_K1]+" "+patch::to_string(-m)+"("+regto_string[R_FP]+")");
			//emit("lw "+regto_string[R_K1]+" "+patch::to_string(-(id->offset))+"("+regto_string[R_K1]+")");
			emit("sub $k1 $k1 "+patch::to_string(-(id->offset)));
		}
}
void Bblockgenerator::loadvariable(Arg_id * argid,Register r){       // use k1
	// watch out for var
	Id * id = argid->id;
	Register reg = R_FP;
	if(id->level != prog->level){
			int m = -(4 * (prog->level + 3 - id->level));
			emit("lw "+regto_string[R_K1]+" "+patch::to_string(-m)+"("+regto_string[R_FP]+")");
			//emit("lw "+regto_string[r]+" "+patch::to_string(id->offset)+"("+regto_string[R_K1]+")");
			reg = R_K1;
	}
	if(id->offset >= 0)
		emit("lw "+regto_string[r]+" "+patch::to_string(-(id->offset))+"("+regto_string[reg]+")");
	else{
		if(Proc *proc = dynamic_cast<Proc*>(prog)){
			if(id->isRef == false)
				emit("lw "+regto_string[r]+" "+ patch::to_string(-(proc->para_offset+id->offset))+"("+regto_string[reg]+")");
			else{
				emit("lw $k1 "+patch::to_string(-(proc->para_offset+id->offset))+"("+regto_string[reg]+")");
				emit("lw "+regto_string[r]+" ($k1)");
			}
		}
		else if(Func *func = dynamic_cast<Func*>(prog)){
			if(id->isRef == false)
				emit("lw "+regto_string[r]+ " "+patch::to_string(-(func->para_offset+id->offset))+"("+regto_string[reg]+")");
			else{
				emit("lw $k1 "+patch::to_string(-(func->para_offset+id->offset))+"("+regto_string[reg]+")");
				emit("lw "+regto_string[r]+" ($k1)");
			}
		}
	}
}
void Bblockgenerator::loadaddress(Arg_id * argid){//store addr in k1
	// load variable address to $k1
	Id * id = argid->id;
	Register reg = R_FP;
	if(id->level != prog->level){
			int m = -(4 * (prog->level + 3 - id->level));
			emit("lw "+regto_string[R_K1]+" "+patch::to_string(-m)+"("+regto_string[R_FP]+")");
			//emit("lw "+regto_string[r]+" "+patch::to_string(id->offset)+"("+regto_string[R_K1]+")");
			reg = R_K1;
	}
	if(id->offset >= 0)
		emit("add "+regto_string[reg]+" "+regto_string[reg]+" "+patch::to_string(-(id->offset)));
	else{
		if(Proc *proc = dynamic_cast<Proc*>(prog)){ if(id->isRef == false) emit("lw $k1 "+patch::to_string(-(proc->para_offset+id->offset))+"("+regto_string[reg]+")");
			else
				emit("add $k1 "+regto_string[reg]+" "+patch::to_string(-(proc->para_offset+id->offset)));
		}
		else if(Func *func = dynamic_cast<Func*>(prog)){
			if(id->isRef == false)
				emit("lw $k1 "+patch::to_string(-(func->para_offset+id->offset))+"("+regto_string[reg]+")");
			else
				emit("add $k1 "+regto_string[reg]+" "+patch::to_string(-(func->para_offset+id->offset)));
		}
	}
}
void Bblockgenerator::storevariable(Arg_id * argid,Register r){//use  k1
	Id * id = argid->id;
	Register reg = R_FP;
	if(id->level != prog->level){
			int m = -(4 * (prog->level + 3 - id->level));
			emit("lw "+regto_string[R_K1]+" "+patch::to_string(-m)+"("+regto_string[R_FP]+")");
			//emit("lw "+regto_string[r]+" "+patch::to_string(id->offset)+"("+regto_string[R_K1]+")");
			reg = R_K1;
	}
	if(id->offset >= 0)
		emit("sw "+regto_string[r]+" " +patch::to_string(-(id->offset))+"("+regto_string[reg]+")");
	else{
		if(Proc *proc = dynamic_cast<Proc*>(prog)){
			if(id->isRef == false)
				emit("sw "+regto_string[r]+ " " +patch::to_string(-(proc->para_offset+id->offset))+"("+regto_string[reg]+")");
			else{
				emit("lw $k1 "+patch::to_string(-(proc->para_offset+id->offset))+"("+regto_string[reg]+")");
				emit("sw "+regto_string[r]+" ($k1)");
			}
		}
		else if(Func *func = dynamic_cast<Func*>(prog)){
			if(id->isRef == false)
				emit("sw "+regto_string[r]+ " "+patch::to_string(-(func->para_offset+id->offset))+"("+regto_string[reg]+")");
			else{
				emit("lw $k1 "+patch::to_string(-(func->para_offset+id->offset))+"("+regto_string[reg]+")");
				emit("sw "+regto_string[r]+" ($k1)");
			}
		}
	}
}

void Addr_Des::addtomap(Id *i)            // temp
{
/*	if(Temp *t  =  dynamic_cast<Temp*>(i)){
		Addr_Descripter ad = new Addr_Descripter(t);
		map.insert(std::make_pair(t,ad));
	}
	else{
		Addr_Descripter ad = new Addr_Descripter(i);
		//ad->addReg(R_ZERO);
		map.insert(std::make_pair(i,ad));
	*/
	Addr_Descripter* ad = new Addr_Descripter(i);
	map.insert(std::make_pair(i,ad));
}

Addr_Descripter * Addr_Des::find(Id *i){
	std::unordered_map<Id*,Addr_Descripter*>::iterator it = map.find(i);
	return it->second;
}
Reg_Descripter* Reg_Des::getAvail(){
	if(!Available.empty())
	{
		Reg_Descripter *x = Available.back();
		Available.pop_back();
		Istaken.push_back(x);
		return x;
	}
	else
		return nullptr;
}
void Reg_Des::availReg(Reg_Descripter *r){
	r->deleteId();
	if(r != Reg_Descripter::k0 && r != Reg_Descripter::t8 && r != Reg_Descripter::t9){
		for(std::vector<Reg_Descripter*>::iterator it = Istaken.begin(); it != Istaken.end();it++){
			if((*it) == r){
				Istaken.erase(it);
				break;
			}
		}
		Available.push_back(r);
	}
}
