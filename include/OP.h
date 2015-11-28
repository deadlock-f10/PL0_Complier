#include <string>
#include <vector>
#ifndef OP_H
#define OP_H
enum OP {
	I_READ = 0 ,
	I_WRITE  ,
	I_IF  ,
	I_IFFALSE ,
	I_GOTO,
	I_PARAM ,
	I_CALLPROC ,
	I_CALLFUNC ,
	I_COPY ,
	I_COPYIND ,
	I_INDCOPY  ,
	I_MULT ,
	I_DIV ,
	I_ADD ,
	I_MINUS ,
};
extern std::string OptoString[15];
#endif
