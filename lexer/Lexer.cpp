#include "Lexer.h"
#include <iostream>

Word *  assign = new Word(":=",T_ASSIGN);
Word	*Plus = new Word("+",T_PLUS);
Word	*Minus = new Word("-",T_MINUS);
Word	*Mult = new Word("*",T_MULT);	
Word	*Div = new Word("/",T_DIV);	
Word	*eq = new Word("=",T_EQ);	
Word	*gt = new Word(">",T_GT);	
Word	*lt = new Word("<",T_LT);	
Word	*le = new Word("<=",T_LE);	
Word	*ge = new Word(">=",T_GE);	
Word	*ne = new Word("<>",T_NE);	
Word	*openparenthesis = new Word("(",T_OPENPARENTHESIS);	
Word	*closeparenthesis = new Word(")",T_CLOSEPARENTHESIS);
Word	*openbracket = new Word("[",T_OPENBRACKET);	
Word	*closebracket = new Word("]",T_CLOSEBRACKET);	
Word	*comma = new Word(":",T_COMMA);	
Word	*semicolon = new Word(";",T_SEMICOLON);	
Word	*dot = new Word(".",T_DOT);	
int Lexer::line = 1;
int Lexer::column = 0;
Lexer::Lexer(std::string str) {
	line = 1;
	column = 0;
			fs = new std::fstream(str,std::fstream::in);
			//std::cout << "open"<<endl;
			reserve(new Word("of" , T_OF));
			reserve(new Word("const" , T_CONST));
			reserve(new Word("var" , T_VAR));
			reserve(new Word("procedure" , T_PROCEDURE));
			reserve(new Word("function" , T_FUNCTION));
			reserve(new Word("begin" , T_BEGIN));
			reserve(new Word("end" , T_END));
			reserve(new Word("if" , T_IF));
			reserve(new Word("then" , T_THEN));
			reserve(new Word("else" , T_ELSE));
			reserve(new Word("while" , T_WHILE));
			reserve(new Word("do" , T_DO));
			reserve(new Word("for" , T_FOR));
			reserve(new Word("downto" , T_DOWNTO));
			reserve(new Word("to" , T_TO));
			reserve(new Word("read" , T_READ));
			reserve(new Word("write" , T_WRITE));
			
			//reserve( Type(T_CHAR));
			//reserve( Type(T_INT));
			//reserve( Array(T_ARRAY));
}
inline void Lexer::readch(){
	peek = fs->get();
	//std::cout << "kmk"<<endl;
	/*if(peek == '\n'){
		line++;
		column = 0;
	}
	else
		column++;*/
}
bool Lexer::readch(char c){
	readch();
	if(peek != c)
		return false;
	return true;
}

Token* Lexer::scan(){
	peek = ' ';
	for( ; ; readch()){
		if(peek == ' ' || peek == '\t')
			continue;
		else if(peek == '\n') 
			line++;
		else
			break;
	}
	switch (peek){
		case ':':
			if(readch('='))
				return assign;
			else
				return comma;
		case '<':
			readch();
			if(peek == '>')
				return ne;
			else if(peek == '=')
				return le;
			else 
				return lt;
		case '>':
			if(readch('='))
				return ge;
			else
				return gt;
		case '=':
			return eq;
		case '+':
			return Plus;
		case '-':
			return Minus;
		case '*':
			return Mult;
		case '/':
			return Div;
		case ';':
			return semicolon;
		case '.':
			return dot;
		case '(':
			return openparenthesis;
		case ')':
			return closeparenthesis;
		case '[':
			return openbracket;
		case ']':
			return closebracket;
		case '\'':
			readch();		
			char c = peek;
			if(readch('\''))
				return new Character(c);
			else
//				throw  SingleQuoteException();
		std::cout << "SingleQuote"<<endl
				;
		/*default:
			;*/
	}
	if(peek == '\"'){
		char str[MAX_STR_LEN];
		int i = 0;
		readch();
		while( peek == ' ' || peek == '!' || (peek >= '#' && peek <= '~')){
			str[i++] = peek;
			readch();
		}
		if(peek == '\"')
			return  new STring( std::string(str , i));
		else
			//throw  DoubleQuoteException();
		std::cout << "DoubleQuote"<<endl
			;
	}
	else if(isdigit(peek)){
		int v = 0;
         do {
            v = 10*v + peek-'0'; 
			readch();
         } while(isdigit(peek));
		 return  new Num(v);
	}
	else if(isalpha(peek)){
		char str[MAX_WORD_LEN];
		int i = 0;
		do{
			str[i++] = peek;
			readch();
		}while(isalnum(peek));
		std::string s =  std::string(str,i);
		hashtable::const_iterator it = words.find(s);
		if(it != words.end())
			return it->second;
		Word *w = new Word(s,T_IDENT);
		words.insert(std::make_pair(w->lexeme,w));
		return w;
	}
	else 
//		throw  WrongSymbolException();
		std::cout << "WrongSymbol"<<endl;
}
