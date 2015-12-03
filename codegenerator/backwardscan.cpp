#include "../include/Codegen.h"
#include <unordered_map>
#include "../include/OP.h"
Bblockgenerator::backwardscan(){
	std::unordered_map<Id *i,int> nextuse;
	for(int i = block->instrlist.size() - 1; i >= 0 ; i--){
		Quadruple *q = block->instrlist[i];
		AttachedInfo *x = new AttachedInfo();
		x->a1nextuse = -3;
		x->a2nextuse = -3;
		x->resnextuse = -3;
		switch(q->op){
			case I_COPYIND:
				{
					Id * id = dynamic_cast<Id*>((Arg_id*)(q->result)->id);
					std::unordered_map<Id*,int>::iterator it = nextuse.find(id);
					if( It == nextuse.end()){
						nextuse.insert(make_pair(id,-1));
						if(Temp *t = dynamic_cast<Temp *>(id))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
						x->resnextuse = -1;
						addtomap(Id *id);
					}
					else{
						x->resnextuse = It->second;
						It->second = -1;
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( It == nextuse.end()){
							if(Temp *t = dynamic_cast<Temp *>(id2))
							{
								p->put(t->op,t);
								t->offset = p->used;
								p->used += t->width;
							}
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
							addtomap(Id *id1);
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
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
						if(Temp *t = dynamic_cast<Temp *>(id1))
						{
								p->put(t->op,t);
								t->offset = p->used;
								p->used += t->width;
						}
							x->a1nextuse = -1;
							addtomap(Id *id1);
						}
						else{
							x->a1nextuse = It->second;
							It->second = i;
						}
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
						if(Temp *t = dynamic_cast<Temp *>(id2))
						{
								p->put(t->op,t);
								t->offset = p->used;
								p->used += t->width;
						}
							x->a2nextuse = -1;
							addtomap(Id *id2);
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
					Id * id = dynamic_cast<Id*>((Arg_id*)(q->result)->id);
					std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
					if( It == nextuse.end()){
						nextuse.insert(make_pair(id,-1));
						if(Temp *t = dynamic_cast<Temp *>(id))
						{
								p->put(t->op,t);
								t->offset = p->used;
								p->used += t->width;
						}
						x->resnextuse = -1;
						addtomap(Id *id);
					}
					else{
						x->resnextuse = It->second;
						It->second = -1;
					}
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
						if(Temp *t = dynamic_cast<Temp *>(id1))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
							x->a1nextuse = -1;
							addtomap(Id *id1);
						}
						else{
							x->a1nextuse = It->second;
							It->second = i;
						}
					}
					break;
				}
			case I_MULT:
			case I_ADD:
			case I_DIV:
			case I_MINUS:
				{
					Id * id = dynamic_cast<Id*>((Arg_id*)(q->result)->id);
					std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
					if( It == nextuse.end()){
						nextuse.insert(make_pair(id,-1));
						if(Temp *t = dynamic_cast<Temp *>(id))
						{
								p->put(t->op,t);
								t->offset = p->used;
								p->used += t->width;
						}
						x->resnextuse = -1;
						addtomap(Id *id);
					}
					else{
						x->resnextuse = It->second;
						It->second = -1;
					}
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id1);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
						if(Temp *t = dynamic_cast<Temp *>(id1))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
							x->a1nextuse = -1;
							addtomap(Id *id1);
						}
						else{
							x->a1nextuse = it->second;
							it->second = i;
						}
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->argid2)){
						Id *id2 = argid2->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id2);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
						if(Temp *t = dynamic_cast<Temp *>(id2))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
							x->a2nextuse = -1;
							addtomap(Id *id2);
						}
						else{
							x->a2nextuse = it->second;
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
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
						if(Temp *t = dynamic_cast<Temp *>(id1))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
							addtomap(Id *id1);
						}
						else{
							x->a1nextuse = it->second;
							it->second = i;
						}
					}
					if(Id * id2 = dynamic_cast<Id*>(r->e2)){
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
						if(Temp *t = dynamic_cast<Temp *>(id2))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
							x->a2nextuse = -1;
							addtomap(Id *id2);
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
					if(Arg_id * argid = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id = argid->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id,i));
						if(Temp *t = dynamic_cast<Temp *>(id))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
							x->a2nextuse = -1;
							addtomap(Id *id);
						}
						else{
							x->a2nextuse = it->second;
							it->second = i;
						}
					}
					break;
				}
			case I_PARAM:
				{
/*					if(Argid * argid = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id = argid->id;
						std::unordered_map<Id*,int>::iterator it = nextuse.find(id);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id,i));
						if(Temp *t = dynamic_cast<Temp *>(id))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
							x->a1nextuse = -1;
							addtomap(Id *id);
						}
						else{
							x->a1nextuse = it->second;
							it->second = i;
						}
					}*/
					break;
				}
			case I_READ:
			case I_CALLFUNC:
				{
					Id * id = dynamic_cast<Id*>((Arg_id*)(q->result)->id);
					std::unordered_map<Id*,int>::iterator it = nextuse.find(id);
					if( It == nextuse.end()){
						if(Temp *t = dynamic_cast<Temp *>(id))
						{
							p->put(t->op,t);
							t->offset = p->used;
							p->used += t->width;
						}
						x->resnextuse = -1;
						nextuse.insert(make_pair(id,-1));
						addtomap(Id *id);
					}
					else{
						x->resnextuse = it->second;
						it->second = -1;
					}
					break;
				}
			case I_CALLPROC:
			case I_GOTO:
			case I_END:
		}
		q->info = x;
	}
}
