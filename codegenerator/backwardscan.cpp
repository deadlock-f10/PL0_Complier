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
					Id * id = dynamic_cast<Id*>(q->result);
					std::unordered_map<Id*,int>::Iterator it = nextuse.find(id1);
					if( It == nextuse.end()){
						nextuse.insert(make_pair(id1,-2));
						x->resnextuse = -1;
					}
					else{
						x->resnextuse = It->second;
						It->second = -2;
					}
					if(Id *id1 = dynamic_cast<Id*>(q->arg2)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id1);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->a2nextuse = -1;
						}
						else{
							x->a2nextuse = It->second;
							It->second = i;
						}
					}
					break;
				}
			case I_INDCOPY:
				{
					if(Id *id1 = dynamic_cast<Id*>(q->arg1)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id1);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->a1nextuse = -1;
						}
						else{
							x->a1nextuse = It->second;
							It->second = i;
						}
					}
					if(Id *id2 = dynamic_cast<Id*>(q->arg2)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id2);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
						}
						else{
							x->a2nextuse = It->second;
							It->second = i;
						}
					}
					//x->resnextuse = -1;
					break;
				}
			case I_COPY:
				{
					Id * id = dynamic_cast<Id*>(q->result);
					std::unordered_map<Id*,int>::Iterator it = nextuse.find(id1);
					if( It == nextuse.end()){
						nextuse.insert(make_pair(id1,-2));
						x->resnextuse = -1;
					}
					else{
						x->resnextuse = It->second;
						It->second = -2;
					}
					if(Id *id1 = dynamic_cast<Id*>(q->arg1)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id1);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->a1nextuse = -1;
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
					Id * id = dynamic_cast<Id*>(q->result);
					std::unordered_map<Id*,int>::Iterator it = nextuse.find(id1);
					if( It == nextuse.end()){
						nextuse.insert(make_pair(id1,-2));
						x->resnextuse = -1;
					}
					else{
						x->resnextuse = It->second;
						It->second = -2;
					}
					if(Id *id1 = dynamic_cast<Id*>(q->arg1)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id1);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->a1nextuse = -1;
						}
						else{
							x->a1nextuse = It->second;
							It->second = i;
						}
					}
					if(Id *id2 = dynamic_cast<Id*>(q->arg2)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id2);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
							x->a2nextuse = -1;
						}
						else{
							x->a2nextuse = It->second;
							It->second = i;
						}
					}
					break;
				}
			case I_IF:
			case I_IFFALSE:
				{
					Rel * r = (Rel *)q->arg1;
					if(Id * id1 = dynamic_cast<Id*>(r->e1)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->resnextuse = -1;
						}
						else{
							x->resnextuse = It->second;
							It->second = i;
						}
					}
					if(Id * id2 = dynamic_cast<Id*>(r->e2)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id2,i));
							x->resnextuse = -1;
						}
						else{
							x->resnextuse = It->second;
							It->second = i;
						}
					}
					break;
				}
			case I_WRITE:
				{
					if(Id * id = dynamic_cast<Id*>(q->arg2)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->resnextuse = -1;
						}
						else{
							x->resnextuse = It->second;
							It->second = i;
						}
					}
					break;

				}
			case I_PARAM:
				{
					if(Id * id = dynamic_cast<Id*>(q->arg1)){
						std::unordered_map<Id*,int>::Iterator it = nextuse.find(id);
						if( It == nextuse.end()){
							nextuse.insert(make_pair(id1,i));
							x->resnextuse = -1;
						}
						else{
							x->resnextuse = It->second;
							It->second = i;
						}
					}
					break;
				}
			case I_READ:
			case I_CALLFUNC:
				{
					Id * id = dynamic_cast<Id*>(q->result);
					std::unordered_map<Id*i,int>::Iterator it = nextuse.find(id);
					if( It == nextuse.end()){
						nextuse.insert(make_pair(id1,-2));
						x->resnextuse = -1;
					}
					else{
						x->resnextuse = It->second;
						It->second = -2;
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
