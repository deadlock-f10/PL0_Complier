#include "../include/Exception.h"
#include "../include/Token.h"
#include "../include/Expr.h"
#include "../include/Tag.h"
#include <string>

extern std::string tagto_string[T_DOT - T_NULL ] = {"T_NULL","T_TEMP","T_INT","T_CHAR","T_INDEX","T_IDENT","T_NUMBER","T_CHARACTER","T_STRING","T_OF","T_ARRAY","T_CONST","T_VAR","T_PROCEDURE","T_FUNCTION","T_BEGIN","T_END","T_IF","T_THEN","T_ELSE","T_WHILE","T_DO","T_FOR","T_DOWNTO","T_TO","T_READ","T_WRITE","T_ASSIGN","T_PLUS","T_MINUS","T_MULT","T_DIV","T_EQ","T_GT","T_LT","T_LE","T_GE","T_NE","(",")","[","]",",",":",";","."};


std::string typetostring(Node * n){

}

//std::string 


std::string TypeMatchExcetion::print(){

}

std::string TokenMatchExcetion::print(){
	return "in line "+patch::to_string(line)+"meet token "+exitedtoken.to_string()+", expected token :"+tagto_string[b];
}

std::string InappropriateException::print(){
	return "in line "+patch::to_string(line)+"meet token "+exitedtoken.to_string()+", can't match any appropriate token";
}
