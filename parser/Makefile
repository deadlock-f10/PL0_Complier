CC = g++
CFLAGS = -Wall --std=c++11
BUILDDIR = ../obj/parser/

LEXER_C = parsedeclconst.cpp parsedeclpaf.cpp parseexpr.cpp parsestmt.cpp 
LEXER_TEST = ../test/test_parser.cpp
HEADER_PATH = ../include
LEXER_H = $(HEADER_PATH)/Lexer.h  $(HEADER_PATH)/Tag.h $(HEADER_PATH)/Token.h $(HEADER_PATH)/Type.h
TEST_O = ../obj/inter/Expr.o ../obj/inter/Stmt.o ../obj/symboltable/PAF.o ../obj/lexer/Lexer.o ../obj/lexer/Type.o

OBJS = $(patsubst %.cpp,$(BUILDDIR)%.o,$(LEXER_C))

$(BUILDDIR)%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

test: $(OBJS)
	$(CC) $(CFLAGS) $(LEXER_TEST) $(OBJS) $(TEST_O)  -o ../parser_test
clean:
	rm -f $(OBJS) *~ *.o ../parser_test
depend: $(LEXER_C)
	makedepend $(LEXER_H) $^

