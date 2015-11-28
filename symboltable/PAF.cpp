#include "../include/PAF.h"
#include "../include/Parser.h"

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
std::fstream intercode("inter.txt");
//typedef std::unordered_set<Tag> Tag_Set;
Program::Program(){
	level = 1;
	name = new Word("main",T_IDENT);
	beginlabel = Parser::getlabel(name);
}
void Program::gen(){
	block->gen(this);
}
void Block::gen(Program *p){
	if(seq_paf != Program::Null)
		seq_paf->gen();
	seq_stmt->gen(p);
}
void Seq_PAF::gen(){
	if(paf != Program::Null)
		paf->print();
	if(pafs != Program::Null)
		pafs->print();
}
Func::Func(Program *p,Word *w, int l) {
	prev = p;
	name = w;
	level = l;
	beginlabel = Parser::getlabel(name);
}
Proc::Proc(Program  *p,Word *w , int l) {
	prev = p;
	name = w;
	level = l;
	beginlabel = Parser::getlabel(name);
}
void Program::print(){
	block->print();
	for(unsigned int i=0; i < Instrlist.size() ; i++)
		intercode<<Instrlist[i]->to_string()<<endl;
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
