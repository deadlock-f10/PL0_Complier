#include "../include/Codegen.h"
#include "../include/Regdef.h"
#include "../include/Expr.h"
#include <vector>

void Bblockgenerator::chooseInstr(Quadruple *q){
	// watch out for t8,t9,k0
	//static bool aweekcall = false;
	static int params = 0;
	//static int paramoffset = 0;
	static Program * calledProg = nullptr;
	//static Arg_id * returnid = nullptr;
	AttachedInfo *x = q->info;
	switch(q->op){                // watch for delay slot    // used to keep ra when call proc
		case I_GOTO:
			emit("j "+(((Result_label*)(q->result))->label));
			break;
		case I_END:
			{
				if(prog->level != 1){
					emit("lw $ra 4($fp)");
					emit("lw $sp 12($fp)");
					if(dynamic_cast<Func*>(prog)){
						emit("lw $k1 ($fp)");
						emit("sw $k1 ($sp)");
					}
					emit("lw $fp 8($fp)");
					emit("jr $ra");
				}
				break;
			}
		case I_INVOKE:
			{
				if(Arg_func *f = dynamic_cast<Arg_func*>(q->arg1))
					calledProg = f->func;
				else if(Arg_proc *proc = dynamic_cast<Arg_proc*>(q->arg1))
					calledProg = proc->proc;
				emit("add $gp $sp $zero");
				break;
			}
		case I_PARAM:
			{
				params++;
				if(Arg_id * arg1 = dynamic_cast<Arg_id*>(q->arg1)){ // if it is var ,  pass address.
					//Id * id1 = arg1->id;
					//Addr_Descripter * addr1 = addr_des.find(id1);
					if(Func *func = dynamic_cast<Func*>(calledProg)){
						Id * func_para = func->paralist[params-1];
						if(func_para->isRef == false ){
							//addr1->assignReg(Reg_Descripter::t8);
							loadvariable(arg1,R_T8);
							emit("sw $t8 ($gp)");
						}
						else{
							loadaddress(arg1);
							emit("sw $k1 ($gp)");
						}
					}
					else if(Proc *proc = dynamic_cast<Proc*>(calledProg)){
						Id * proc_para = proc->paralist[params-1];
						if(proc_para->isRef == false ){
							//addr1->assignReg(Reg_Descripter::t8);
							loadvariable(arg1,R_T8);
							emit("sw $t8 ($gp)");
						}
						else{
							loadaddress(arg1);
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
				break;
			}
		case I_CALLPROC:
		case I_CALLFUNC:
			{
				int displayoffset1=0;
				if(Func* m = dynamic_cast<Func*>(prog))
					displayoffset1 = -(m->para_offset);
				if(Proc* n = dynamic_cast<Proc*>(prog))
					displayoffset1 = -(n->para_offset);
				if(prog->level != 1){
					if(calledProg->level == prog->level + 1){
						for(int i = 1 ; i < prog->level ; i++){
							displayoffset1 -= 4;
							emit("lw $k1 "+ patch::to_string(displayoffset1) + "($fp)");
							emit("sw $k1 ($gp)");
							emit("sub $gp $gp 4");
						}
						emit("add $k1 $zero $fp");
						emit("sw $k1 ($gp)");
						emit("sub $gp $gp 4");
					}
					else{
						for(int i = 1 ; i < calledProg->level ; i++){
							displayoffset1 -= 4;
							emit("lw $k1 "+ patch::to_string(displayoffset1) + "($fp)");
							emit("sw $k1 ($gp)");
							emit("sub $gp $gp 4");
						}
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
				emit("sub $fp $gp 8");
				emit("add $sp $fp $zero");
				//emit("add $sp $fp $zero");
				emit("jal "+calledProg->beginlabel);
		//		emit("sw $ra 4($fp)");
				if(q->op == I_CALLFUNC){
					emit("lw $k1 ($sp)");
					Arg_id * result = dynamic_cast<Arg_id*>(q->result);
					storevariable(result,R_K1);
				}
				calledProg = nullptr;
				params = 0;
				break;
			}
		case I_COPYIND:
			{
				Arg_id *result = dynamic_cast<Arg_id*>(q->result);
				Id *id = result->id;
				Addr_Descripter *addr = addr_des.find(id);
				Reg_Descripter * reg = addr->getReg();
				if(x->a2nextuse == -3){
					Arg_int * in = dynamic_cast<Arg_int*>(q->arg2);
					emit("sub "+ regto_string[R_K1] +" "+regto_string[R_K1] + " " + patch::to_string(in->value));
				}
				else{
					Arg_id* arg2 = dynamic_cast<Arg_id*>(q->arg2);
					Addr_Descripter *addr2 = addr_des.find(arg2->id);
					Reg_Descripter * reg2 = addr2->getReg();
					emit("sub "+ regto_string[R_K1] +" "+regto_string[R_K1] + " " + regto_string[reg2->r]);
					if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2->deleteReg();
						reg_des.availReg(reg2);
					}
				}
				emit("lw "+ regto_string[reg->r]+ " " +"("+regto_string[R_K1]+")");
				storevariable(result,reg->r);
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					addr->deleteReg();
					reg_des.availReg(reg);
				}
				break;
			}
		case I_INDCOPY:
			{
				if(x->a2nextuse == -3){
					Arg_int * in2 = dynamic_cast<Arg_int*>(q->arg2);
					emit("sub "+regto_string[R_K1]+" "+regto_string[R_K1] +" " + patch::to_string(in2->value));
				}
				else{
					Arg_id* arg2 = dynamic_cast<Arg_id*>(q->arg2);
					Addr_Descripter *addr2 = addr_des.find(arg2->id);
					Reg_Descripter * reg2 = addr2->getReg();
					emit("sub "+regto_string[R_K1]+" "+regto_string[R_K1] +" " + regto_string[reg2->r]);
					if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2->deleteReg();
						reg_des.availReg(reg2);
					}
				}
				if(x->a1nextuse == -3){
					Arg_int * in1 = dynamic_cast<Arg_int*>(q->arg1);
					emit("li "+regto_string[R_T8]+" "+patch::to_string(in1->value));
					emit("sw "+regto_string[R_T8]+" "+"("+regto_string[R_K1]+")");
				}
				else{
					Arg_id* arg1 = dynamic_cast<Arg_id*>(q->arg1);
					Addr_Descripter *addr1 = addr_des.find(arg1->id);
					Reg_Descripter * reg1 = addr1->getReg();
					emit("sw "+regto_string[reg1->r]+" "+"("+regto_string[R_K1]+")");
					if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1->deleteReg();
						reg_des.availReg(reg1);
					}
				}
				break;
			}
		case I_COPY:
			{
				Arg_id* result = dynamic_cast<Arg_id*>(q->result);
				Addr_Descripter *addr = addr_des.find(result->id);
				Reg_Descripter * reg = addr->getReg();
				if(x->a1nextuse == -3){
					Arg_int * in = dynamic_cast<Arg_int*>(q->arg1);
					emit("li "+ regto_string[reg->r]  + " " + patch::to_string(in->value));
				}
				else{
					Arg_id* arg1 = dynamic_cast<Arg_id*>(q->arg1);
					Addr_Descripter *addr1 = addr_des.find(arg1->id);
					Reg_Descripter * reg1 = addr1->getReg();
					emit("add "+ regto_string[reg->r] +" "+regto_string[R_ZERO] + " " + regto_string[reg1->r]);
					if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1->deleteReg();
						reg_des.availReg(reg1);
					}
				}
				storevariable(result,reg->r);
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					addr->deleteReg();
					reg_des.availReg(reg);
				}
				break;
			}
		case I_IF:
		case I_IFFALSE:
			{
				Rel * r = ((Arg_rel*)(q->arg1))->relation;
				std::string str =((Result_label*)(q->result))->label;
				std::string title1 = "";
				std::string title11 = "";
				std::string title2= "";
				std::string title22= "";
				switch(r->op->tag){
					case T_EQ:
						title1 = "beq ";
						title11 = "beq ";
						title2 = "bne ";
						title22 = "bne ";
						break;
					case T_LE:
						title1 = "ble ";
						title11 = "bge ";
						title2 = "bgt ";
						title22 = "blt ";
						break;
					case T_GE:
						title1 = "bge ";
						title11 = "ble ";
						title2 = "blt ";
						title22 = "bgt ";
						break;
					case T_LT:
						title1 = "blt ";
						title11 = "bgt ";
						title2 = "bge ";
						title22 = "ble ";
					case T_GT:
						title1 = "bgt ";
						title11 = "blt ";
						title2 = "ble ";
						title22 = "bge ";
					default:
						title1 = "bne ";
						title11 = "bne ";
						title2 = "beq ";
						title22 = "beq ";
				}
					if(x->a1nextuse == -3 && x->a2nextuse == -3){
						Constant* c1 = (Constant *)(r->e1);
						Constant* c2 = (Constant *)(r->e2);
						if(q->op == I_IF){
							if((r->op->tag == T_EQ && c1->c == c2->c) || (r->op->tag ==T_LE && c1->c <= c2->c) || (r->op->tag==T_GE && c1->c >= c2->c) || (r->op->tag == T_LT && c1->c < c2->c) || (r->op->tag == T_GT && c1->c > c2->c) || (r->op->tag == T_NE && c1->c != c2->c))
							emit("j "+((Result_label*)(q->result))->label);
						}
						else{
							if((r->op->tag == T_EQ && c1->c != c2->c) || (r->op->tag ==T_LE && c1->c > c2->c) || (r->op->tag==T_GE && c1->c < c2->c) || (r->op->tag == T_LT && c1->c >= c2->c) || (r->op->tag == T_GT && c1->c <= c2->c) || (r->op->tag == T_NE && c1->c == c2->c))
							emit("j "+((Result_label*)(q->result))->label);
						}
					}
				else if(x->a1nextuse != -3 && x->a2nextuse == -3){
					Id *id1 = (Id*)(r->e1);
					Addr_Descripter * addr1 = addr_des.find(id1);
					Reg_Descripter * reg1 = addr1->getReg();
					Constant* c2 = (Constant *)(r->e2);
					if(q->op == I_IF)
						emit(title1+ regto_string[reg1->r] +" "+patch::to_string(c2->c)+" "+str);
					else
						emit(title2+ regto_string[reg1->r] +" "+patch::to_string(c2->c)+" "+str);
					if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1->deleteReg();
						reg_des.availReg(reg1);
					}
				}
				else if(x->a2nextuse != -3 && x->a1nextuse == -3){
					Id *id2 = (Id*)(r->e2);
					Addr_Descripter * addr2 = addr_des.find(id2);
					Reg_Descripter * reg2 = addr2->getReg();
					Constant* c1 = (Constant *)(r->e1);
					if(q->op == I_IF)
						emit(title11+ regto_string[reg2->r] +" "+patch::to_string(c1->c)+" "+str);
					else
						emit(title22+ regto_string[reg2->r] +" "+patch::to_string(c1->c)+" "+str);
					if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2->deleteReg();
						reg_des.availReg(reg2);
					}
				}
				else{
					Id *id1 = (Id*)(r->e1);
					Addr_Descripter * addr1 = addr_des.find(id1);
					Reg_Descripter * reg1 = addr1->getReg();
					Id *id2 = (Id*)(r->e2);
					Addr_Descripter * addr2 = addr_des.find(id2);
					Reg_Descripter * reg2 = addr2->getReg();
					if(q->op == I_IF)
						emit(title1+ regto_string[reg1->r] +" "+regto_string[reg2->r]+" "+str);
					else
						emit(title2+ regto_string[reg1->r] +" "+regto_string[reg2->r]+" "+str);
					if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1->deleteReg();
						reg_des.availReg(reg1);
					}
					if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2->deleteReg();
						reg_des.availReg(reg2);
					}
				}
				break;
			}
		case I_READ:
			{
				Arg_id * result = dynamic_cast<Arg_id*>(q->result);
				Id * id = result->id;
				if(id->type == Type::Int)
					emit("li $v0 "+patch::to_string(5));
				else if(id->type == Type::Char)
					emit("li $v0 "+patch::to_string(12));
				emit("syscall");
				//Arg_id *result = dynamic_cast<Arg_id*>(q->result);
				Addr_Descripter *addr = addr_des.find(id);
				Reg_Descripter * reg = addr->getReg();
				emit("add "+regto_string[reg->r]+" $v0 $zero");
				storevariable(result,reg->r);
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					addr->deleteReg();
					reg_des.availReg(reg);
				}
				break;
			}
		case I_WRITE:
			{
				if(q->arg1 != nullptr){
					Arg_string * string = (Arg_string *)(q->arg1);
					emit(".data");
					emit(string->name+" : .asciiz \""+string->str+"\"");
					emit(".text");
					emit("li $v0 4");
					emit("la $a0 "+string->name);
					emit("syscall");
				}
				if(q->arg2 != nullptr){
					if(Arg_id * arg2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id * id2 = arg2->id;
						Addr_Descripter * addr2 = addr_des.find(id2);
						Reg_Descripter * reg2 = addr2->getReg();
						if(id2->type == Type::Int)
							emit("li $v0 "+patch::to_string(1));
						else if(id2->type == Type::Char)
							emit("li $v0 "+patch::to_string(11));
						emit("add $a0 $zero "+regto_string[reg2->r]);
						emit("syscall");
						if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
							addr2->deleteReg();
							reg_des.availReg(reg2);
						}
					}
					else{
						Arg_int * in = dynamic_cast<Arg_int*>(q->arg2);
						emit("li $a0 "+patch::to_string(in->value));
						emit("syscall");
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
				Reg_Descripter * reg = addr->getReg();
				std::string opera;
				switch(q->op){
					case I_MULT:
						opera="mul ";
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
					Arg_int *arg1 = dynamic_cast<Arg_int*>(q->arg1);
					int c1 = arg1->value;
					Arg_int *arg2 = dynamic_cast<Arg_int*>(q->arg2);
					int c2 = arg2->value;
					int res = 0 ;
					switch(q->op){
						case I_MULT:
							res = c1 * c2;
							break;
						case I_DIV:
							res = c1 / c2;
							break;
						case I_MINUS:
							res = c1 - c2;
							break;
						case I_ADD:
							res = c1 + c2;
							break;
						default:
							break;
					}
					emit("li "+regto_string[reg->r]+" "+patch::to_string(res));
				}
				else if(x->a1nextuse == -3 && x->a2nextuse != -3){
					Arg_int *arg1 = dynamic_cast<Arg_int*>(q->arg1);
					int c1 = arg1->value;
					Arg_id *arg2 = dynamic_cast<Arg_id*>(q->arg2);
					Addr_Descripter *addr2 = addr_des.find(arg2->id);
					Reg_Descripter * reg2 = addr2->getReg();
					emit("la $t8 "+patch::to_string(c1));
					emit(opera+regto_string[reg->r]+" $t8 "+regto_string[reg2->r]);
					if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2->deleteReg();
						reg_des.availReg(reg2);
					}
				}
				else if(x->a1nextuse != -3 && x->a2nextuse == -3){
					Arg_int *arg2 = dynamic_cast<Arg_int*>(q->arg2);
					int c2 = arg2->value;
					Arg_id *arg1 = dynamic_cast<Arg_id*>(q->arg1);
					Addr_Descripter *addr1 = addr_des.find(arg1->id);
					Reg_Descripter * reg1 = addr1->getReg();
					emit(opera+regto_string[reg->r]+" "+regto_string[reg1->r]+" "+patch::to_string(c2));
					if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1->deleteReg();
						reg_des.availReg(reg1);
					}
				}
				else{
					Arg_id *arg1 = dynamic_cast<Arg_id*>(q->arg1);
					Addr_Descripter *addr1 = addr_des.find(arg1->id);
					Reg_Descripter * reg1 = addr1->getReg();
					Arg_id *arg2 = dynamic_cast<Arg_id*>(q->arg2);
					Addr_Descripter *addr2 = addr_des.find(arg2->id);
					Reg_Descripter * reg2 = addr2->getReg();
					emit(opera+regto_string[reg->r]+" "+regto_string[reg1->r]+" "+regto_string[reg2->r]);
					if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1->deleteReg();
						reg_des.availReg(reg1);
					}
					if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2->deleteReg();
						reg_des.availReg(reg2);
					}
				}
				storevariable(result,reg->r);
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					addr->deleteReg();
					reg_des.availReg(reg);
				}
				break;
		}
	}
}
					/* if(x->a1nextuse == -1 || reg1 == Reg_Descripter::t8){
						addr1->deleteReg();
						reg_des.availReg(reg1);
					}*/

					/* if(x->a2nextuse == -1 || reg2 == Reg_Descripter::t9){
						addr2->deleteReg();
						reg_des.availReg(reg2);
					}*/


				/*	storevariable(result,reg->r);
				if(x->resnextuse == -1 || reg ==Reg_Descripter::k0){
					addr->deleteReg();
					reg_des.availReg(reg);
				}*/
