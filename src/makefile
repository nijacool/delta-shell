TARGET = sudba
INCLUDE = include/
YACC = bison
LEX = flex
OBJS = susql-scanner.o susql-parser.o sudba-main.o

CFLAGS = -I$(INCLUDE) -Wall -pedantic -Wextra -Wcast-align -Wcast-qual \
	-Wdisabled-optimization -Wformat=2 -Winit-self \
	-Wmissing-include-dirs -Wredundant-decls -Wshadow \
	-Wstrict-overflow=5 -Wundef -Werror -Wno-unused \
	-g -Wno-unused-result -O3 -Wno-strict-overflow

all: $(TARGET)

# Uncomment the end of the rule for memory debugging
sudba: $(OBJS) 
	$(CC) $(OBJS) -o $(TARGET) #-lefence 

susql-scanner.c: susql.l susql-parser.h
	$(LEX) -o susql-scanner.c susql.l

susql-parser.c susql-parser.h: susql.y
	$(YACC) -Wempty-rule -d -o susql-parser.c susql.y

sudba-main.o: sudba-main.c susql-parser.h

susql-scanner.o: susql-scanner.c susql-parser.h

# Remove everything that can be rebuilt
clean:
	rm -f $(OBJS) $(TARGET) susql-scanner.c susql-parser.h susql-parser.c

test: $(TARGET) test.sql
	@echo "The program must fail the next three tests. If it does not, it is incorrect"
	@echo "Test 1 (must not \"hang up\"):"
	./$(TARGET) 1 2 3 || true
	@echo "Test 2 (must not \"hang up\"):"
	./$(TARGET) -95 || true
	@echo "Test 3 (must not \"hang up\"):"
	./$(TARGET) "hello, world" || true
	@echo "The program must pass the next test:"
	@cat test.sql | ./$(TARGET) 8888
