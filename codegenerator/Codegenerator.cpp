#include "../include/Codegen.h"
#include "../include/PAF.h"

std::ofstream targetcode("/home/f10/pl0-compiler/my_compiler/targeg.asm");
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
