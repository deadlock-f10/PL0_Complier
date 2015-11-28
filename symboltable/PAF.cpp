#include "../include/PAF.h"

std::string Quadruple::to_string(){
	std::string s = "";
	for(int i = 0 ; i < labellist.size();i++)
		s += (labellist.at(i)+": ");
	s += "\t"+OptoString[op];
	if(arg1 != nullptr)
		s += "\t"+arg1->to_string();
	if(arg2 != nullptr)
		s += "\t"+arg2->to_string();
	if(result != nullptr)
		s += "\t"+result->to_string();
	return s;
}
Program * Program::Null = nullptr;

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
	for(int i=0; i < Instrlist->size() ; i++)
		intercode<<Instrlist->at(i)->to_string()<<endl;
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
