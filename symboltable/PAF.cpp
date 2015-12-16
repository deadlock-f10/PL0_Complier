#include "../include/PAF.h"
#include "../include/Parser.h"
#include <stdio.h>
Program * Program::Null = nullptr;

void Program::addinstr(OP op,Arg1* arg1 , Arg2* arg2,Result * result){
	Instrlist.push_back(new Quadruple(labellist,op,arg1 ,arg2,result));
	labellist = nullptr;
}

Node* Program::get(Token *w) {
	for(Program *e = this ; e != nullptr ; e = e->prev){
		Hashtable::const_iterator found = e->symboltable.find(w);
		if ( found != e->symboltable.end())
			return found->second;
	}
	return nullptr;
}
//FILE * intercode=fopen("/home/f10/Doc/a/inter.txt","w");
//typedef std::unordered_set<Tag> Tag_Set;
std::ofstream intercode("inter.txt");
std::ofstream optcode("opt.txt");
std::ofstream dagcode("dag.txt");
Program::Program(){
	level = 1;
	name = new Word("main",T_IDENT);
	beginlabel = Parser::getlabel(name);
}
void Program::gen(){
    //emitlabel(beginlabel,this);
	block->gen(this);
	emit(I_END,nullptr,nullptr,nullptr,this);
}
void Block::gen(Program *p){
	if(seq_paf != Program::Null)
		seq_paf->gen();
	seq_stmt->gen(p);
}
void Seq_PAF::gen(){
	if(paf != Program::Null)
		paf->gen();
	if(pafs != Program::Null)
		pafs->gen();
}
Func::Func(Program *p,Word *w, int l) {
	prev = p;
	name = w;
	level = l;
	beginlabel = Parser::getlabel(name);
	para_offset = -4*(3+level-1);
	display_offset = -4*(3+level);
}
Proc::Proc(Program  *p,Word *w , int l) {
	prev = p;
	name = w;
	level = l;
	beginlabel = Parser::getlabel(name);
	para_offset = -4*(3+level-1);
	display_offset = -4*(3+level);
}
void Program::print(){
	block->print();
	intercode<<beginlabel<<":"<<endl;
	for(unsigned int i=0; i < Instrlist.size() ; i++){
        intercode<<Instrlist[i]->to_string()<<endl;
        intercode.flush();
	}

		/*{
            fprintf(intercode,"%s\n",Instrlist[i]->to_string().c_str());
            fflush(intercode);
		}*/
}
void Block::print(){
	if(seq_paf != Program::Null)
		seq_paf->print();
}
void Seq_PAF::print(){
	if(paf != Program::Null)
		paf->print();
	if(pafs != Program::Null)
		pafs->print();
}

void Program::print_block(){
	block->print_block();
	optcode<<beginlabel<<":"<<endl;
	dagcode<<beginlabel<<":"<<endl;
	for(unsigned i = 1; i < blocklist.size(); i++){
		BasicBlock* b = blocklist[i];
		optcode<<b->label+":"<<endl;
		dagcode<<b->label+":"<<endl;
		for(unsigned j = 0 ; j < b->instrlist.size(); j++)
			optcode<<b->instrlist[j]->to_string()<<endl;
		for(unsigned j = 0 ; j < b->daglized.size(); j++)
			dagcode<<b->daglized[j]->to_string()<<endl;
	}
}
void Block::print_block(){
	if(seq_paf != Program::Null)
		seq_paf->print_block();
}
void Seq_PAF::print_block(){
	if(paf != Program::Null)
		paf->print_block();
	if(pafs != Program::Null)
		pafs->print_block();
}
