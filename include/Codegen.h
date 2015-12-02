#include "Quadruple.h"
#include <vector>
#include "Regdef.h"
#include <unordered_map>

#ifndef CODEGEN_H
#define CODEGEN_H
class Program;
class Reg_Des;
class Addr_Des;
class AttachedInfo{      // -3 mean correponding variable don't exit or do not need a next-use information(func,proc,const,array) , -2 represent no next use. -1 represent live on exit.
	int a1nextuse;
	int a2nextuse;
	int resnextuse;
};
class Codegenerator{
	public:
	Program *p;
	Codegen(Program *prog){p = prog;}
	void gen(Program *p);
	void gen_block(Block *b);
	void gen_seqpaf(Seq_PAF *seq);
	//void basicblockgen();
};
class Bblockgenerator{
	std::vector<std::string> instrlist;
	Reg_Des reg_des;
	Addr_Des addr_des;       // no array variable.
	BasicBlock *block;
	Program *prog;
	void getReg(Quadruple *q);           // automatically assign register for each variable in x(modify register and address decripter,emit load code if necessary).      next-use information contained inside quadruple.
	void loadvariable(Id *id , Register r); //load id to register r
	void gen();
	void backwardscan();              // first pass backward from the last instruction of this basicblock. collect next-use info and add variable to addr_des.
	Bblockgenerator(BasicBlock *b,Program *p){block = b; prog = p;}
	void emit(std::string s);
	void chooseInstr(Quadruple *q);       // consider  constant-constant rel.
	void findstore(Arg_id *argid);
	void findload(Arg_id *argid,Reg_Descripter* backup);
	void loadarrayaddr(Arg_id* argid);
	void loadvariable(Arg_id* argid, Register r);
	void storevariable(Arg_id* argid, Register r);
}
class Reg_Descripter{
	public:
	Register r;	
	Addr_Descripter* addr;
	Reg_Descripter(Register reg) {r = reg;}
	void assignId(Addr_Descripter *x) { addr = x;}
	void deleteId(){addr = nullptr;}
	static Reg_Descripter *t8,*t9,*k0;           //only used when no available register.
	//std::vector<Addr_Descripter*> list;
	//void clear();
//	static Reg_Descripter *zero;               // zero doesn't any register, it means variable's value is on stack.
};
class Addr_Descripter{
	public:
	Id * id;
	//std::vector<Reg_Descripter*> list;
	bool valueonstack = true;
	Reg_Descripter* reg;
	int temponstack;      // offset with regard to FP
	Addr_Descripter(Id *i){if(Temp * t = dynamic_cast<Temp*>(i)) valueonstack = false;id = i;}
	void assignReg(Reg_Descripter *r) { reg = r;}
	Reg_Descripter* getReg(){return reg;};
	void deleteReg(){reg.deleteId();reg = nullptr};
	void invalidatestack(){valueonstack == false;}
	//void clear();
};
class Reg_Des{
	public:
	std::vector<Reg_Descripter*> Available{ new Reg_Descripter(R_A1),new Reg_Descripter(R_A2),new Reg_Descripter(R_A3),new Reg_Descripter(R_T0),new Reg_Descripter(R_T1),new Reg_Descripter(R_T2),new Reg_Descripter(R_T3),new Reg_Descripter(R_T4),new Reg_Descripter(R_T5),new Reg_Descripter(R_T6),new Reg_Descripter(R_T7),new Reg_Descripter(R_S0),new Reg_Descripter(R_S1),new Reg_Descripter(R_S2),new Reg_Descripter(R_S3),new Reg_Descripter(R_S4),new Reg_Descripter(R_S5),new Reg_Descripter(R_S6),new Reg_Descripter(R_S7)};
	//std::vector<Reg_Descripter*> Istaken;
	Reg_Descripter* getAvail();       // if a register is now empty ,return it. else return nullptr
	void availReg(Reg_Descripter *r){r.deleteReg();if(r != Reg_Descripter::k0 && r != Reg_Descripter::t8 && r != Reg_Descripter::t9) Available.push_back(r);}
};
class Addr_Des{
	std::unordered_map<Id*,Addr_Descripter*> map;
	void addtomap(Id *i);            // temp 
	Addr_Descripter * find(Id *i);
};
#endif
