#include "../include/Lexer.h"
#include "../include/Type.h"
#include <iostream>

Word *  Word::assign = new Word(":=",T_ASSIGN);
Word	*Word::Plus = new Word("+",T_PLUS);
Word	*Word::Minus = new Word("-",T_MINUS);
Word	*Word::Mult = new Word("*",T_MULT);	
Word	*Word::Div = new Word("/",T_DIV);	
Word	*Word::eq = new Word("=",T_EQ);	
Word	*Word::gt = new Word(">",T_GT);	
Word	*Word::lt = new Word("<",T_LT);	
Word	*Word::le = new Word("<=",T_LE);	
Word	*Word::ge = new Word(">=",T_GE);	
Word	*Word::ne = new Word("<>",T_NE);	
Word	*Word::openparenthesis = new Word("(",T_OPENPARENTHESIS);	
Word	*Word::closeparenthesis = new Word(")",T_CLOSEPARENTHESIS);
Word	*Word::openbracket = new Word("[",T_OPENBRACKET);	
Word	*Word::closebracket = new Word("]",T_CLOSEBRACKET);	
Word	*Word::comma = new Word(",",T_COMMA);	
Word	*Word::colon = new Word(":",T_COLON);	
Word	*Word::semicolon = new Word(";",T_SEMICOLON);	
Word	*Word::dot = new Word(".",T_DOT);	
Word    *Word::temp = new Word("temp",T_TEMP);
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
			reserve(new Word("array" , T_ARRAY));
			
			reserve(Type::Char);
			reserve(Type::Int);
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
L:  peek = ' ';
	for( ; ; readch()){
		if(peek == ' ' || peek == '\t')
			continue;
		else if(peek == '\n') 
			line++;
		else
			break;
	}
	if(peek == '{'){
		for( ;peek != '}' ; readch())
			;
		goto L;
	}
	switch (peek){
		case ':':
			if(readch('='))
				return Word::assign;
			else
				return Word::colon;
		case '<':
			readch();
			if(peek == '>')
				return Word::ne;
			else if(peek == '=')
				return Word::le;
			else 
				return Word::lt;
		case '>':
			if(readch('='))
				return Word::ge;
			else
				return Word::gt;
		case '=':
			return Word::eq;
		case '+':
			return Word::Plus;
		case '-':
			return Word::Minus;
		case '*':
			return Word::Mult;
		case '/':
			return Word::Div;
		case ';':
			return Word::semicolon;
		case '.':
			return Word::dot;
		case '(':
			return Word::openparenthesis;
		case ')':
			return Word::closeparenthesis;
		case '[':
			return Word::openbracket;
		case ']':
			return Word::closebracket;
		case ',':
			return Word::comma;
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
		 fs->unget();
		 return  new Num(v);
	}
	else if(isalpha(peek)){
		char str[MAX_WORD_LEN];
		int i = 0;
		do{
			str[i++] = peek;
			readch();
		}while(isalnum(peek));
		fs->unget();
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
		std::cout << "WrongSymbol :"<< peek << endl;
	return nullptr;
}
