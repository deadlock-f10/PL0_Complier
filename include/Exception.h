#include <string>
#include "Node.h"
#include "Token.h"
#ifndef EXCEPTION_H
#define EXCEPTION_H

class Type;
class Exception{
	public:
	int line;
	virtual std::string print(){return "";}
};

class TypeMatchException : public Exception{
	public:
	Type* exitedtype;
	Type * expectedtype;
	TypeMatchException(Type*a , Type*b , int l){exitedtype = a ; expectedtype = b;line = l;}
	std::string print();
};

class TokenMatchException : public Exception{
	public:
	Token * exitedtoken;
	Tag expectedtoken;
	TokenMatchException(Token *a , Tag b, int l){exitedtoken = a ; expectedtoken = b;line = l;}
	std::string print();
};

class InappropriateException : public Exception{
	public:
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

class NoSuchIdentException : public Exception{
	public :
		Word * w;
		NoSuchIdentException(Word * word,int l){w = word;line = l;}
		std::string print();
};

class OutOfBoundException : public Exception{
	public:
		Word * w;
		OutOfBoundException(Word * word,int l){w = word;line = l;}
		std::string print();
};

class MiscellaneousException : public Exception{
	public:
		std::string s;
		MiscellaneousException(std::string str,int l){line = l;s=str;}
		std::string print();
};

#endif
