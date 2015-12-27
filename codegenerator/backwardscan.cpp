#include "../include/Codegen.h"
#include <unordered_map>
#include "../include/OP.h"
#include "../include/Quadruple.h"
#include "../include/Expr.h"

void Bblockgenerator::backwardscan(){
	std::unordered_map<Id *,int> nextuse;
	if(prog->toa1 != nullptr)          // it's is essential. because below will not check variable in I_param  while global variable might appear in param
	{
		addr_des.addtomap(prog->toa1);
		Addr_Descripter *ad = addr_des.find(prog->toa1);
		Reg_Descripter::a1->assignId(ad);
		ad->assignReg(Reg_Descripter::a1);
	}
	if(prog->toa2 != nullptr){
		addr_des.addtomap(prog->toa2);
		Addr_Descripter *ad = addr_des.find(prog->toa2);
		Reg_Descripter::a2->assignId(ad);
		ad->assignReg(Reg_Descripter::a2);
	}
	for(int i = block->instrlist.size() - 1; i >= 0 ; i--){
		Quadruple *q = block->instrlist[i];
		AttachedInfo *x = new AttachedInfo();
		x->a1nextuse = -3;
		x->a2nextuse = -3;
		x->resnextuse = -3;
		switch(q->op){
			case I_COPYIND:
				{
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
						if(Temp *t = dynamic_cast<Temp *>(result))
						{
							prog->put(t->op,t);
							t->offset = prog->used;
							prog->used += t->type->width;
						}
					std::unordered_map<Id*,int>::iterator it = nextuse.find(result);
					if( it == nextuse.end()){
						nextuse.insert(make_pair(result,-1));
						x->resnextuse = -1;
						addr_des.addtomap(result);
					}
					else{
						x->resnextuse = it->second;
						it->second = -1;
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
							addr_des.addtomap(id2);
						}
						else{
							x->a2nextuse = it->second;
							it->second = i;
						}
					}
					break;
				}
			case I_INDCOPY:
				{
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->a1nextuse = -1;
							addr_des.addtomap(id1);
						}
						else{
							x->a1nextuse = it->second;
							it->second = i;
						}
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
							addr_des.addtomap(id2);
						}
						else{
							x->a2nextuse = it->second;
							it->second = i;
						}
					}
					//x->resnextuse = -1;
					break;
				}
			case I_COPY:
				{
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
						if(Temp *t = dynamic_cast<Temp *>(result))
						{
							prog->put(t->op,t);
							t->offset = prog->used;
							prog->used += t->type->width;
						}
					std::unordered_map<Id*,int>::iterator it = nextuse.find(result);
					if( it == nextuse.end()){
						nextuse.insert(make_pair(result,-1));
						x->resnextuse = -1;
						addr_des.addtomap(result);
					}
					else{
						x->resnextuse = it->second;
						it->second = -1;
					}
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->a1nextuse = -1;
							addr_des.addtomap(id1);
						}
						else{
							x->a1nextuse = it->second;
							it->second = i;
						}
					}
					break;
				}
			case I_MULT:
			case I_ADD:
			case I_DIV:
			case I_MINUS:
				{
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
						if(Temp *t = dynamic_cast<Temp *>(result))
						{
							prog->put(t->op,t);
							t->offset = prog->used;
							prog->used += t->type->width;
						}
					std::unordered_map<Id*,int>::iterator it = nextuse.find(result);
					if( it == nextuse.end()){
						nextuse.insert(make_pair(result,-1));
						x->resnextuse = -1;
						addr_des.addtomap(result);
					}
					else{
						x->resnextuse = it->second;
						it->second = -1;
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
							addr_des.addtomap(id2);
						}
						else{
							x->a2nextuse = it->second;
							it->second = i;
						}
					}
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->a1nextuse = -1;
							addr_des.addtomap(id1);
						}
						else{
							x->a1nextuse = it->second;
							it->second = i;
						}
					}
					break;
				}
			case I_IF:
			case I_IFFALSE:
				{
					Rel * r = ((Arg_rel*)q->arg1)->relation;
					if(Id * id1 = dynamic_cast<Id*>(r->e1)){
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->a1nextuse = -1;
							addr_des.addtomap(id1);
						}
						else{
							x->a1nextuse = it->second;
							it->second = i;
						}
					}
					if(Id * id2 = dynamic_cast<Id*>(r->e2)){
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
							addr_des.addtomap(id2);
						}
						else{
							x->a2nextuse = it->second;
							it->second = i;
						}
					}
					break;
				}
			case I_WRITE:
				{
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( it == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
							addr_des.addtomap(id2);
						}
						else{
							x->a2nextuse = it->second;
							it->second = i;
						}
					}
					break;
				}
			case I_READ:
				{
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
						if(Temp *t = dynamic_cast<Temp *>(result))
						{
							prog->put(t->op,t);
							t->offset = prog->used;
							prog->used += t->type->width;
						}
					std::unordered_map<Id*,int>::iterator it = nextuse.find(result);
					if( it == nextuse.end()){
						nextuse.insert(make_pair(result,-1));
						x->resnextuse = -1;
						addr_des.addtomap(result);
					}
					else{
						x->resnextuse = it->second;
						it->second = -1;
					}
					break;
				}
			case I_CALLFUNC:
				{
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
						if(Temp *t = dynamic_cast<Temp *>(result))
						{
							prog->put(t->op,t);
							t->offset = prog->used;
							prog->used += t->type->width;
						}
					break;
				}
			case I_CALLPROC:
			case I_GOTO:
			case I_END:
			case I_INVOKE:
			case I_PARAM:
				;
		}
		q->info = x;
	}
}
					/*Id * result = dynamic_cast<Id*>((Arg_id*)(q->result)->id);
					std::unordered_map<Id*,int>::iterator it = nextuse.find(result);
					if( it == nextuse.end()){
						nextuse.insert(make_pair(result,-1));
						if(Temp *t = dynamic_cast<Temp *>(id))
						{
							prog->put(t->op,t);
							t->offset = prog->used;
							prog->used += t->type->width;
						}
						x->resnextuse = -1;
						addr_des.addtomap(result);
					}
					else{
						x->resnextuse = it->second;
						it->second = -1;
					}*/
					/*if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
						if( it == nextuse.end()){
						if(Temp *t = dynamic_cast<Temp *>(id1))
						{
								prog->put(t->op,t);
								t->offset = prog->used;
								prog->used += t->type->width;
						}
							nextuse.insert(make_pair(id1,i));
							x->a1nextuse = -1;
							addr_des.addtomap(id1);
						}
						else{
							x->a1nextuse = it->second;
							it->second = i;
						}
					}*/
					/*if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( it == nextuse.end()){
							if(Temp *t = dynamic_cast<Temp *>(id2))
							{
								prog->put(t->op,t);
								t->offset = prog->used;
								prog->used += t->type->width;
							}
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
							addr_des.addtomap(id2);
						}
						else{
							x->a2nextuse = it->second;
							it->second = i;
						}
					}*/
