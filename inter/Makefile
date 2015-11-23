CC = g++
CFLAGS = -Wall --std=c++11
BUILDDIR = ../obj/inter/

LEXER_C = Expr.cpp Stmt.cpp 
HEADER_PATH = ../include
LEXER_H = $(HEADER_PATH)/Lexer.h  $(HEADER_PATH)/Tag.h $(HEADER_PATH)/Token.h $(HEADER_PATH)/Type.h

OBJS = $(patsubst %.cpp,$(BUILDDIR)%.o,$(LEXER_C))

$(BUILDDIR)%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@
all: Makefile $(OBJS)

clean:
	rm -f $(OBJS) *~ *.o parser_test
depend: $(LEXER_C)
	makedepend $(LEXER_H) $^


