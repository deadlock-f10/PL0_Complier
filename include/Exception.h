#include <string>
#include "Node.h"
#include "Token.h"
#ifndef EXCEPTION_H
#define EXCEPTION_H

class Exception{
	public:
	virtual std::string print(){;}
};

class TypeMatchExcetion : public Exception{
	public:
		int line;
	Node * exitedtype;
	Node * expectedtype;
	TypeMatchException(Node *a , Node *b , int l){exitedtype = a ; expectedtype = b;line = l;}
	std::string print();
};

class TokenMatchException : public Exception{
	public:
		int line;
	Token * exitedtoken;
	Tag * expectedtoken;
	TokenMatchException(Token *a , Tag b, int l){exitedtoken = a ; expectedtoken = b;line = l;}
	std::string print();
};

class InappropriateException : public Exception{
	public:
	int line;
	Token * exitedtoken;
	InappropriateException(Token *a, int l){exitedtoken = a ;line = l;}
	std::string print();
};

class MultipleDefinitionException : public Exception{
	public :
		;
};

class ToomucherrorException{
	public :
};

#endif
