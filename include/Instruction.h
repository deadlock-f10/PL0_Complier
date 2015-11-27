#include <string>
#ifndef INSTRUCTION_H
#define INSTRUCTION_H
enum class Instruction : std::string{
	I_READ = "read",
	I_WRITE = "write",
	I_IF = "if",
	I_IFFALSE = "iffalse",
	I_PARAM = "param",
	I_CALLPROC = "callproc",
	I_CALLFUNC = "callfunc",
	I_COPY = "copy",
	I_COPYIND = "copyind",
	I_INDCOPY = "indcopy",
	I_MULT = "*",
	I_DIV = "/",
	I_ADD = "+",
	I_MINUS = "-",
};
#endif
