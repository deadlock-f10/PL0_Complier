CC = g++
CFLAGS = -Wall -Werror --std=c++11
BUILDDIR = ../obj/lexer/

LEXER_C = Lexer.cpp Type.cpp 
LEXER_TEST = ../test/test_lexer.cpp
HEADER_PATH = ../include
LEXER_H = $(HEADER_PATH)/Lexer.h  $(HEADER_PATH)/Tag.h $(HEADER_PATH)/Token.h $(HEADER_PATH)/Type.h

OBJS = $(patsubst %.cpp,$(BUILDDIR)%.o,$(LEXER_C))

$(BUILDDIR)%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

test: $(OBJS)
	$(CC) $(CFLAGS) $(LEXER_TEST) $(OBJS) -o ../lexer_test
clean:
	rm -f $(OBJS) *~ *.o ../lexer_test
depend: $(LEXER_C)
	makedepend $(LEXER_H) $^
