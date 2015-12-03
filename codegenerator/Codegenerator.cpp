#include "../include/Codegen.h"
#include "../include/PAF.h"
#include "../include/Regdef.h"
#include "../include/Expr.h"
#include <fstream>
#include <string>


std::string regto_string[32] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
extern std::ofstream targetcode;
void Codegenerator::gen(Program *p){
	gen_block(p->block);
	for(unsigned int i = 1 ; i < p->blocklist.size(); i++){
		Bblockgenerator * bb = new Bblockgenerator(p->blocklist[i],p);
		bb->gen();
	}
}
void Codegenerator::gen_block(Block *b){
	if(b->seq_paf != Program::Null)
		gen_seqpaf((Seq_PAF*)(b->seq_paf));
}
void Codegenerator::gen_seqpaf(Seq_PAF *seq){
	if(seq->paf != Program::Null)
		gen(seq->paf);
	if(seq->pafs != Program::Null)
		gen_seqpaf((Seq_PAF*)(seq->pafs));
}

void Codegenerator::print(Program *p){
	if(p->level == 1){
		targetcode<<"li $sp 0x7fffeffc\n";
		targetcode<<"add $fp $zero $sp\n";
		targetcode<<"j "<<p->beginlabel<<endl;
	}
	print_block(p->block);
	targetcode<<p->beginlabel<<":"<<endl;
	if(p->level != 1)
		targetcode<<"sw $ra 4($fp)"<<endl;
	targetcode<<"sub $sp $sp "<<p->used<<endl;
	for(unsigned int i = 1 ; i < p->blocklist.size(); i++){
		Bblockgenerator * bb = new Bblockgenerator(p->blocklist[i],p);
		bb->print();
	}
}
void Codegenerator::print_block(Block *b){
	if(b->seq_paf != Program::Null)
		print_seqpaf((Seq_PAF*)(b->seq_paf));
}
void Codegenerator::print_seqpaf(Seq_PAF *seq){
	if(seq->paf != Program::Null)
		print(seq->paf);
	if(seq->pafs != Program::Null)
		print_seqpaf((Seq_PAF*)(seq->pafs));
}
