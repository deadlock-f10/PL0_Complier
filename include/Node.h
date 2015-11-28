#include "Token.h"
#include "Type.h"
#include "Tag.h"
#include "Lexer.h"
#include <string>
#include "OP.h"
#include "Quadruple.h"
#ifndef NODE_H
#define NODE_H
typedef std::string label;
class Program ;
namespace helper
{
	//void emitlabel(label l,Program *p); 
	void emitlabel(std::string s, Program *p); 
	void emit(OP op,Arg1* arg1 , Arg2* arg2,Result * result,Program *p);
};
class Node{
	public :
	int lexline = 0;
	Node () {lexline = Lexer::line;}
	void error(std::string s);      // unimplemented;
	static int labels;
	label newlabel() {return "L"+patch::to_string(++labels);}
	//void emitlabel(label l) {std::cout << "L" + l + ":";}
	//void emitlabel(label l,Program *p) {helper::emitlabel(l,p);}
	void emitlabel(std::string s, Program *p) {helper::emitlabel(s,p);}
	//void emit(std::string s) {std::cout <<"\t"<< s << endl;}
	//void emit(std::string s) {;}
	virtual void emit(OP op,Arg1* arg1 , Arg2* arg2,Result * result,Program *p){helper::emit(op,arg1 , arg2, result,p);}
};
#endif
