#include "Token.h"
#include "Type.h"
#include "Tag.h"
#include "Lexer.h"
#include <string>
#ifndef NODE_H
#define NODE_H
typedef int label;
class Node{
	public :
	int lexline = 0;
	Node () {lexline = Lexer::line;}
	void error(std::string s);      // unimplemented;
	static label labels;
	label newlabel() {return ++labels;}
	void emitlabel(label l) {std::cout << "L" + i + ":";}
	void emit(std::string s) {std::cout <<"\t"<< s << endl;}
};
#endif
