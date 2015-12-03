#include "../include/Codegen.h"
#include "Regdef.h"
#include <vector>

void Bblockgenerator::chooseInstr(Quadruple *q){
	// watch out for t8,t9,k0
	//static bool aweekcall = false;
	static int params = 0;
	static int paramoffset = 0;
	static Program * calledProg = nullptr;
	//static Arg_id * returnid = nullptr;
	AttachedInfo *x = q->info;
	switch(q->op){
		case I_GOTO:
			emit("j "+((Result_label*)(q->result)->label));
			break;
		case I_END:
			{
				if(p->level != 1){
					emit("lw $ra 4($fp)");
					emit("lw $sp 12($fp)");
					if(dynamic_cast<Func*>(p)){
						emit("lw $k1 ($fp)");
						emit("sw $k1 ($sp)");
					}
					emit("lw $fp 8($fp)");
					emit("jr $ra");
				}
			}
		case I_INVOKE:
			{
				if(Arg_func *f = dynamic_cast<Arg_func*>(q->arg1))
					calledProg = f->func;
				else if(Arg_proc *proc = dynamic_cast<Arg_proc*>(q->arg1))
					calledProg = proc->proc;
				emit("add $gp $sp $zero");
			}
		case I_PARAM:
			{
				params++;
				if(Arg_id * arg1 = dynamic_cast<Arg_id*>(q->arg1)){ // if it is var ,  pass address.
					Id * id1 = arg1->id;
					Addr_Descripter * addr1 = addr_des.find(id1);
					if(Arg_func *func = dynamic_cast<Arg_func*>(q->arg1)){
						Id * func_para = func->paralist[params-1];
						if(func_para.isRef == false ){
							//addr1.assignReg(Reg_Descripter::t8);
							loadvariable(addr1,R_T8);
							emit("sw $t8 ($gp)");
						}
						else{
							loadaddress(addr1);
							emit("sw $k1 ($gp)");
						}
					}
					else if(Arg_proc *proc = dynamic_cast<Arg_proc*>(q->arg1)){
						Id * proc_para = proc->paralist[params-1];
						if(proc_para.isRef == false ){
							//addr1.assignReg(Reg_Descripter::t8);
							loadvariable(addr1,R_T8);
							emit("sw $t8 ($gp)");
						}
						else{
							loadaddress(addr1);
							emit("sw $k1 ($gp)");
						}
					}
					/*Addr_Descripter * addr1 = addr_des.find(id1);
					Reg_Descripter * reg1 = addr1->getReg();*
					emit("sw "+regto_string[reg1->r] + "($gp)");*/
					emit("sub $gp $gp 4");
				}
				else{
					Arg_int * in1 = dynamic_cast<Arg_int*>(q->arg1);
					emit("li $k1 "+patch::to_string(in1->value));
					emit("sw $k1 ($gp)");
					emit("sub $gp $gp 4");
				}
			/*	if(leftparam == 0){
					emit("jal "+calledProg->beginlabel);
					if(dynamic_cast<Func*>(calledProg)){
						Arg_id * result = returnid;
						Addr_Descripter * addr = addr_des.find(result->id);
						Reg_Descripter * reg = addr->getReg();
						emit("lw "+regto_string[reg->r]+" -12($sp)");
						if(reg ==Reg_Descripter::k0){   // it can't be a non-temp id
							storevariable(result,reg->r);
							addr.deleteReg();
							reg_des.availReg(reg);
						}
					}
					calledProg = nullptr;					
					returnid = nullptr;
				}*/
				break;
			}
		case I_CALLPROC:
		case I_CALLFUNC:
			{
				int displayoffset1 = -(p->para_offset);
				if(p->level != 1){
					for(int i = 1 ; i < fu->level ; i++){
						displayoffset1 -= 4;
						emit("lw $k1 "+ patch::to_string(displayoffset1) + "($fp)");
						emit("sw $k1 ($gp)");
						emit("sub $gp $gp 4");
					}
					if(calledProg->level == p->level + 1){
						emit("add $k1 $zero $fp");
						emit("sw $k1 ($gp)");
						emit("sub $gp $gp 4");
					}
				}
				else
				{
					emit("add $k1 $zero $fp");
					emit("sw $k1 ($gp)");
					emit("sub $gp $gp 4");
				}
				emit("sw $sp ($gp)");
				emit("sub $gp $gp 4");
				emit("sw $fp ($gp)");
				emit("add $fp $gp 8");
				emit("add $sp $fp $zero");
				//emit("add $sp $fp $zero");
				emit("jal "+calledProg->beginlabel);
				if(p->op == I_CALLFUNC){
					emit("lw $k1 $(sp)");
					Arg_id * result = dynamic_cast<Arg_id*>(p->result);
					storevariable(result,R_K1);
				}
				break;
			}
		case I_COPYIND:
			{
				Arg_id *result = dynamic_cast<Arg_id*>(q->result);
				Id *id = result->id;
				Addr_Descripter *addr = addr_des.find(id);
				Reg_Descripter * reg = addr.getReg();
				if(x->a2nextuse == -3){
					Arg_int * in = dynamic_cast<Arg_int*>(q->arg2);
					emit("add "+ regto_string[R_K1] +" "+regto_string[R_K1] + " " + patch::to_string(in->value));
				}
				else{
					Arg_id* arg = dynamic_cast<Arg_id*>(q->arg2); 
					Addr_Descripter *addr2 = addr_des.find(arg->id);
					Reg_Descripter * reg2 = addr2.getReg();
					emit("add "+ regto_string[R_K1] +" "+regto_string[R_K1] + " " + regto_string[reg2->r]);
					if(x->a2nextuse == -2){
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}
					else if(x->a2nextuse == -1){
						storevariable(arg,reg2->r);
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}
					else {
						if(reg2 == Reg_Descripter::t9)
							addr2.deleteReg();
					}
				}
				emit("lw "+ regto_string[reg->r]+ " " +"("+regto_string[R_K1]+")");
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					storevariable(i,reg->r);
					addr.deleteReg();
					reg_des.availReg(reg);
				}
			}
		case I_INDCOPY:
			{
				if(x->a2nextuse == -3){
					Arg_int * in2 = dynamic_cast<Arg_int*>(q->arg2);
					emit("add "+regto_string[R_K1]+" "+regto_string[R_K1] +" " + patch::to_string(in2->c));
					if(x->a1nextuse == -3){
						Arg_int * in1 = dynamic_cast<Arg_int*>(q->arg1);
						emit("li "+regto_string[R_T8]+" "+patch::to_string(in1->value));
						emit("sw "+regto_string[R_T8]+" "+"("+regto_string[R_K1]+")");
					}
					else{
						Arg_id* arg1 = dynamic_cast<Arg_id*>(q->arg1); 
						Addr_Descripter *addr1 = addr_des.find(arg1->id);
						Reg_Descripter * reg1 = addr1.getReg();
						emit("sw "+regto_string[reg1->r]+" "+"("+regto_string[R_K1]+")");
					if(x->a1nextuse == -2){
						addr1.deleteReg();
						reg_des.availReg(reg1);
					}
					else if(x->a1nextuse == -1){
						storevariable(arg,reg1->r);
						addr1.deleteReg();
						reg_des.availReg(reg1);
					}
					else {
						if(reg1 == Reg_Descripter::t8)
							addr1.deleteReg();
					}
					}
				}
				else{
					Arg_id* arg2 = dynamic_cast<Arg_id*>(q->arg2); 
					Addr_Descripter *addr2 = addr_des.find(arg1->id);
					Reg_Descripter * reg2 = addr2.getReg();
					emit("add "+regto_string[R_K1]+" "+regto_string[R_K1] +" " + regto_string[reg2->r]);
					if(x->a2nextuse == -2){
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}
					else if(x->a2nextuse == -1){
						storevariable(arg,reg2->r);
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}
					else {
						if(reg2 == Reg_Descripter::t9)
							addr2.deleteReg();
					}
					if(x->a1nextuse == -3){
						Arg_int * in1 = dynamic_cast<Arg_int*>(q->arg1);
						emit("li "+regto_string[R_T8]+" "+patch::to_string(in1->value));
						emit("sw "+regto_string[R_T8]+" "+"("+regto_string[R_K1]+")");

					}
					else{
						Arg_id* arg1 = dynamic_cast<Arg_id*>(q->arg1); 
						Addr_Descripter *addr1 = addr_des.find(arg1->id);
						Reg_Descripter * reg1 = addr1.getReg();
						emit("sw "+regto_string[reg1->r]+" "+"("+regto_string[R_K1]+")");
					if(x->a1nextuse == -2){
						addr1.deleteReg();
						reg_des.availReg(reg1);
					}
					else if(x->a1nextuse == -1){
						storevariable(arg,reg1->r);
						addr1.deleteReg();
						reg_des.availReg(reg1);
					}
					else {
						if(reg1 == Reg_Descripter::t8)
							addr1.deleteReg();
					}

					}
				}
			}
		case I_COPY:
			{
				Arg_id* i = dynamic_cast<Arg_id*>(q->result); 
				Addr_Descripter *addr = addr_des.find(i->id);
				Reg_Descripter * r = addr.getReg();
				if(x->a1nextuse == -3){
					Arg_int * in = dynamic_cast<Arg_int*>(q->arg1);
					emit("add "+ regto_string[r->r] +" "+regto_string[R_ZERO] + " " + patch::to_string(in->value));
				}
				else{
					Arg_id* arg = dynamic_cast<Arg_id*>(q->arg1); 
					Addr_Descripter *addr1 = addr_des.find(arg->id);
					Reg_Descripter * ad = addr1.getReg();
					emit("add "+ regto_string[r->r] +" "+regto_string[R_ZERO] + " " + regto_string[ad->r]);
					if(x->a1nextuse == -2){
						addr1.deleteReg();
						reg_des.availReg(r);
					}
					else if(x->a1nextuse == -1){
						storevariable(arg,ad->r);
						addr.deleteReg();
						reg_des.availReg(ad);
					}
				}
				if(x->resnextuse == -1 || r ==Reg_Descripter::k0){
					storevariable(i,r->r);
					addr.deleteReg();
					reg_des.availReg(r);
				}
			}
		case I_IF:
			{
				Rel * r = ((Arg_rel*)q->arg1)->relation;
				std:;string str =(Result_label*)(q->Result_label)->label
				std::string title = "";
				if(r->op->tag == T_EQ)
					title = "beq ";
				else if(r->op->tag == T_LE)
					title = "ble ";
				else if(r->op->tag == T_GE)
					title = "bge ";
				else if(r->op->tag == T_LT)
					title = "blt ";
				else if(r->op->tag == T_GT)
					title = "bgt ";
				else 
					title = "bne ";
				if(x->a1nextuse == -3 && x->a2nextuse == -3){
					Constant* c1 = (Constant *)(r->e1);
					Constant* c2 = (Constant *)(r->e2);
					if((r->op->tag == T_EQ && c1->c == c2->c) || (r->op->tag ==T_LE && c1->c <= c2->c) || (r->op->tag==T_GE && c1->c >= c2->c) || (r->op->tag == T_LT && c1->c < c2->c) || (r->op->tag == T_GT && c1->c > c2->c) || (r->op->tag == T_NE && c1->c != c2->c))
						emit("j "+(Result_label*)(q->Result_label)->label);
				}
				else if(x->a1nextuse != -3 && x->a2nextuse == -3){
					Id *id = (Id*)(r->e1);
					Addr_Descripter * addr = addr_des(id);
					Reg_Descripter * reg = addr.getReg();
					Constant* c2 = (Constant *)(r->e2);
					emit(title+ regto_string[reg->r] +" "+patch::to_string(c2->c)+" "+str)
					if(x->a1nextuse == -2){
						addr.deleteReg();
						reg_des.availReg(reg);
					}
				}
				else if(x->a2nextuse != -3 && x->a1nextuse == -3){
					Id *id = (Id*)(r->e2);
					Addr_Descripter * addr = addr_des(id);
					Reg_Descripter * reg = addr.getReg();
					Constant* c2 = (Constant *)(r->e1);
					emit(title+ regto_string[reg->r] +" "+patch::to_string(c2->c)+" "+str);
					if(x->a2nextuse == -2){
						addr.deleteReg();
						reg_des.availReg(reg);
					}
				}
				else{
					std::string title = "";
					Id *id1 = (Id*)(r->e1);
					Addr_Descripter * addr1 = addr_des(id1);
					Reg_Descripter * reg1 = addr1.getReg();
					Id *id2 = (Id*)(r->e2);
					Addr_Descripter * addr2 = addr_des(id2);
					Reg_Descripter * reg2 = addr2.getReg();
					emit(title+ regto_string[reg1->r] +" "+regto_string[reg2->r]+" "+str);
				}
					if(x->a1nextuse == -2){
						addr1.deleteReg();
						reg_des.availReg(reg);
					}
					if(x->a2nextuse == -2){
						addr2.deleteReg();
						reg_des.availReg(reg);
					}
			}
		case I_IFFALSE:
			{
				Rel * r = ((Arg_rel*)q->arg1)->relation;
				std:;string str =(Result_label*)(q->Result_label)->label
				std::string title = "";
				if(r->op->tag == T_EQ)
					title = "bne ";
				else if(r->op->tag == T_LE)
					title = "bgt ";
				else if(r->op->tag == T_GE)
					title = "blt ";
				else if(r->op->tag == T_LT)
					title = "bge ";
				else if(r->op->tag == T_GT)
					title = "ble ";
				else 
					title = "beq ";
				if(x->a1nextuse == -3 && x->a2nextuse == -3){
					Constant* c1 = (Constant *)(r->e1);
					Constant* c2 = (Constant *)(r->e2);
					if((r->op->tag == T_EQ && c1->c != c2->c) || (r->op->tag ==T_LE && c1->c > c2->c) || (r->op->tag==T_GE && c1->c < c2->c) || (r->op->tag == T_LT && c1->c >= c2->c) || (r->op->tag == T_GT && c1->c <= c2->c) || (r->op->tag == T_NE && c1->c == c2->c))
						emit("j "+(Result_label*)(q->Result_label)->label);
				}
				else if(x->a1nextuse != -3 && x->a2nextuse == -3){
					Id *id = (Id*)(r->e1);
					Addr_Descripter * addr = addr_des(id);
					Reg_Descripter * reg = addr.getReg();
					Constant* c2 = (Constant *)(r->e2);
					emit(title+ regto_string[reg->r] +" "+patch::to_string(c2->c)+" "+str)
					if(x->a1nextuse == -2){
						addr.deleteReg();
						reg_des.availReg(reg);
					}
				}
				else if(x->a2nextuse != -3 && x->a1nextuse == -3){
					Id *id = (Id*)(r->e2);
					Addr_Descripter * addr = addr_des(id);
					Reg_Descripter * reg = addr.getReg();
					Constant* c2 = (Constant *)(r->e1);
					emit(title+ regto_string[reg->r] +" "+patch::to_string(c2->c)+" "+str);
					if(x->a2nextuse == -2){
						addr.deleteReg();
						reg_des.availReg(reg);
					}
				}
				else{
					std::string title = "";
					Id *id1 = (Id*)(r->e1);
					Addr_Descripter * addr1 = addr_des(id1);
					Reg_Descripter * reg1 = addr1.getReg();
					Id *id2 = (Id*)(r->e2);
					Addr_Descripter * addr2 = addr_des(id2);
					Reg_Descripter * reg2 = addr2.getReg();
					emit(title+ regto_string[reg1->r] +" "+regto_string[reg2->r]+" "+str);
				}
					if(x->a1nextuse == -2){
						addr1.deleteReg();
						reg_des.availReg(reg);
					}
					if(x->a2nextuse == -2){
						addr2.deleteReg();
						reg_des.availReg(reg);
					}
					break;
			}
		case I_READ:
			{
				Arg_id * result = dynamic_cast<Arg_id*>(q->result);
				Id * id = result->id;
				if(id->type == Type::int)
					emit("lw "+regto_string[R_V0]+" "+patch::to_string(5));
				else if(id->type == Type::char)
					emit("lw "+regto_string[R_V0]+" "+patch::to_string(12));
				emit("syscall");
				Arg_id *result = dynamic_cast<Arg_id*>(q->result);
				Id *id = result->id;
				Addr_Descripter *addr = addr_des.find(id);
				Reg_Descripter * reg = addr.getReg();
				emit("add "+regto_string[R_V0]+" "+regto_string[reg->r]);
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					storevariable(result,reg->r);
					addr.deleteReg();
					reg_des.availReg(reg);
				}
			}
		case I_WRITE:
			{
				if(q->arg1 != nullptr){
					Arg_string * string = (Arg_string *)(q->arg1);
					emit(".data");
					emit(string->name+" : .asciiz \""+string->str+"\"");
					emit(".text");
					emit("lw $v0 4");
					emit("la $a0 "+string->name);
					emit("syscall");
				}
				if(q->arg2 != nullptr){
					if(Arg_id * arg2 = dynamic_cast<Arg_id*>(q->arg2)){
					Id * id2 = arg2->id2;
					Addr_Descripter * addr2 = addr_des(id2);
					Reg_Descripter * reg2 = addr2.getReg();
					if(id->type == Type::int)
						emit("lw "+regto_string[R_V0]+" "+patch::to_string(1));
					else if(id->type == Type::char)
						emit("lw "+regto_string[R_V0]+" "+patch::to_string(11));
					emit("add $a0 $zero"+regto_string[reg2->r]);
					emit("syscall");
					if(x->a2nextuse == -2){
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}
					else if(x->a2nextuse == -1){
						storevariable(arg,reg2->r);
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}
					else{
						if(reg2 == Reg_Descripter::t9)
							addr2.deleteReg();
					}
					}
					else{
						Arg_int * in = dynamic_cast<Arg_int*>(q->arg2);
						emit("add $a0 $zero "+patch::to_string(in->value));
					}
				}
				break;
		}
		case I_MULT:
		case I_DIV:
		case I_MINUS:
		case I_ADD:
			{
				Arg_id* result = dynamic_cast<Arg_id*>(q->result);
				Addr_Descripter *addr = addr_des.find(result->id);
				Reg_Descripter * reg = addr.getReg();
				std::string opera;
				switch(p->op){
					case I_MULT:
						opera="mulo ";
						break;
					case I_DIV:
						opera="div ";
						break;
					case I_MINUS:
						opera="sub ";
						break;
					case I_ADD:
						opera="add ";
						break;
					default:
						break;
				}
				if(x->a1nextuse == -3 && x->a2nextuse == -3){
					Arg_int arg1 = dynamic_cast<Arg_int*>(q->arg1);
					int c1 = arg1->value;
					Arg_int arg2 = dynamic_cast<Arg_int*>(q->arg2);
					int c2 = arg2->value;
					int result = 0 ;
					switch(p->op){
						case I_MULT:
							result = c1 * c2;
							break;
						case I_DIV:
							result = c1 / c2;
							break;
						case I_MINUS:
							result = c1 - c2;
							break;
						case I_ADD:
							result = c1 + c2;
							break;
						default:
							break;
					}
					emit("li "+regto_string[reg->r]+" "+patch::to_string(result));
				}
				else if(x->a1nextuse == -3 && x->a2nextuse != -3){
					Arg_int arg1 = dynamic_cast<Arg_int*>(q->arg1);
					int c1 = arg1->value;
					Arg_id *arg2 = dynamic_cast<Arg_id*>(q->arg2);
					Addr_Descripter *addr2 = addr_des.find(arg2->id);
					Reg_Descripter * reg2 = addr2.getReg();
					emit("la $t8 "+patch::to_string(c1));
					emit(opera+regto_string[reg->r]+" $t8 "+regto_string[reg2->r]);
					if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}
				}
				else if(x->a1nextuse != -3 && x->a2nextuse == -3){
					Arg_int arg2 = dynamic_cast<Arg_int*>(q->arg2);
					int c2 = arg2->value;
					Arg_id *arg1 = dynamic_cast<Arg_id*>(q->arg1);
					Addr_Descripter *addr1 = addr_des.find(arg1->id);
					Reg_Descripter * reg1 = addr2.getReg();
					emit(opera+regto_string[reg->r]+" "+regto_string[reg1->r]+" "+patch::to_string(c2));
					if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1.deleteReg();
						reg_des.availReg(reg1);
					}
				}
				else{
					Arg_id *arg1 = dynamic_cast<Arg_id*>(q->arg1);
					Addr_Descripter *addr1 = addr_des.find(arg1->id);
					Reg_Descripter * reg1 = addr2.getReg();
					Arg_id *arg2 = dynamic_cast<Arg_id*>(q->arg2);
					Addr_Descripter *addr2 = addr_des.find(arg2->id);
					Reg_Descripter * reg2 = addr2.getReg();
					emit(opera+regto_string[reg->r]+" "+regto_string[reg1->r]+" "+regto_string[reg2->r]);
					if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1.deleteReg();
						reg_des.availReg(reg1);
					}
					if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}
				}
				storevariable(result,reg->r);
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					addr.deleteReg();
					reg_des.availReg(reg);
				}
				break;
			}
	}
}
					/* if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1.deleteReg();
						reg_des.availReg(reg1);
					}*/

					/* if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2.deleteReg();
						reg_des.availReg(reg2);
					}*/
				/*	storevariable(result,reg->r);
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					addr.deleteReg();
					reg_des.availReg(reg);
				}*/
