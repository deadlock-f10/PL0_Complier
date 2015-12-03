#include "../include/Codegen.h"
#include "../include/PAF.h"
#include <fstream>

extern std::ofstream targetcode;
void Codegen::gen(Program *p){
	gen_block(p->block);	
	for(unsigned int i = 1 ; i < blocklist.size(); i++){
		Bblockgenerator * bb = new Bblockgenerator(blocklist[i],p);
		bb->gen();
	}
}
void Codegen::gen_block(Block *b){
	if(b->seq_paf != Program::Null)
		gen_seqpaf(b->seq_paf);
}
void Codegen::gen_seqpaf(Seq_PAF *seq){
	if(seq->paf != Program::Null)
		gen(seq->paf);
	if(seq->pafs != Program::Null)
		gen_seqpaf(seq->pafs);
}

void Codegen::print(Program *p){
	if(p->level == 1){
		targetcode<<"add $fp $zero 0x7ffff000\n";
		targetcode<<"add $sp $zero 0x7fffeffc\n";
		targetcode<<"j "<<p->beginlabel<<endl;
	}
	gen_block(p->block);	
	targetcode<<p->beginlabel<<":"<<endl;
	if(p->level != 1)
		targetcode<<"sw $ra 4($fp)"<<endl;
	targetcode<<"add $sp $sp "<<p->used<<endl;
	for(unsigned int i = 1 ; i < blocklist.size(); i++){
		Bblockgenerator * bb = new Bblockgenerator(blocklist[i],p);
		bb->print();
	}
}
void Codegen::print_block(Block *b){
	if(b->seq_paf != Program::Null)
		print_seqpaf(b->seq_paf);
}
void Codegen::print_seqpaf(Seq_PAF *seq){
	if(seq->paf != Program::Null)
		print(seq->paf);
	if(seq->pafs != Program::Null)
		print_seqpaf(seq->pafs);
}
