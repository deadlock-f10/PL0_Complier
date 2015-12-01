#include "Quadruple.h"
#include <vector>
#include "Regdef.h"
#include <unordered_map>

#ifndef CODEGEN_H
#define CODEGEN_H
class Program;
class Reg_Des;
class Addr_Des;
class Codegen{
	public:
	Program *p;
	Reg_Des reg_des;
	Addr_Des addr_des;
	Codegen(Program *prog){p = prog;}
	void gen();
	void getReg(Quadruple *q);           // automatically assign register for each variable in x(modify register and address decripter,emit load code if necessary).      next-use information contained inside quadruple.
	void emit(quadruple *q);       // consider  constant-constant rel.
};
class Reg_Descripter{
	public:
	Register r;	
	std::vector<Addr_Descripter*> list;
	Reg_Descripter(Register reg) {r = reg;}
	void addId(Addr_Descripter *x) { list.push_back(x);}
	void deleteId(Addr_Descripter *x);
	static Reg_Descripter zero;               // zero doesn't any register, it means variable's value is on stack.
};
class Addr_Descripter{
	public:
	Id * id;
	std::vector<Reg_Descripter> list;
	int temponstack;      // offset with regard to FP
	Addr_Descripter(Id *i){id = i;}
	void addReg(Reg_Descripter *r) { list.push_back(r);}
	void deleteReg(Reg_Descripter *r);
};
class Reg_Des{
	public:
	std::vector<Reg_Descripter*> Available{new Reg_Descripter(R_A0), new Reg_Descripter(R_A1),new Reg_Descripter(R_A2),new Reg_Descripter(R_A3),new Reg_Descripter(R_T0),new Reg_Descripter(R_T1),new Reg_Descripter(R_T2),new Reg_Descripter(R_T3),new Reg_Descripter(R_T4),new Reg_Descripter(R_T5),new Reg_Descripter(R_T6),new Reg_Descripter(R_T7),new Reg_Descripter(R_S0),new Reg_Descripter(R_S1),new Reg_Descripter(R_S2),new Reg_Descripter(R_S3),new Reg_Descripter(R_S4),new Reg_Descripter(R_S5),new Reg_Descripter(R_S6),new Reg_Descripter(R_S7)};
	std::vector<Reg_Descripter*> Istaken;
	Reg_Descripter* getAvail();
};
class Addr_Des{
	std::unordered_map<Id*,Addr_Descripter*> map;
	void addtomap(Id *i);            // temp 
};
#endif
