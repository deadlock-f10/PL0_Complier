#ifndef Tags_H
#define Tags_H
enum Tag{
	T_NULL = 256,

	T_INT, // integer
	T_CHAR, // char

	T_IDENT,
	T_NUMBER,
	T_CHARACTER, 
	T_STRING,
	T_OF, // of
	T_ARRAY , // array

	T_CONST, // const
	T_VAR, // var
    T_PROCEDURE, // procedure
	T_FUNCTION, // function

    T_BEGIN, // begin
    T_END, // end
    T_IF, // if
    T_THEN, // then
	T_ELSE, // else
    T_WHILE, // while
    T_DO,  // do
	T_FOR, // for
	T_DOWNTO, // downto
	T_TO, // to
	
	T_READ, // read
	T_WRITE, // write

	T_ASSIGN, // :=

	T_PLUS, // + 
	T_MINUS, // -
	T_MULT, // *
	T_DIV, // / 

	T_EQ, // =
	T_GT, // >
	T_LT, // <
	T_LE, // <=
	T_GE, // >=
	T_NE, // <>

	T_OPENPARENTHESIS, // (
	T_CLOSEPARENTHESIS,  // )
	T_OPENBRACKET, // [
	T_CLOSEBRACKET, // ]

	T_COMMA, // :
	T_SEMICOLON, // ;
	T_DOT, // .
//	T_DQUOTE, // "
//	T_SQUOTE // '
};

#endif
