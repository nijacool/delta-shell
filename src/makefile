TARGET = sudba
INCLUDE = include/
OBJS = sudba.o sudba_interface.o

CFLAGS = -I$(INCLUDE) -Wall -pedantic -Wextra -Wcast-align -Wcast-qual \
	-Wdisabled-optimization -Wformat=2 -Winit-self \
	-Wmissing-include-dirs -Wredundant-decls -Wshadow \
	-Wstrict-overflow=5 -Wundef -Werror -Wno-unused \
	-g -Wno-unused-result -O3 -Wno-strict-overflow

all: $(TARGET)

sudba: $(OBJS) # Uncomment the end of the next line for memory debugging
	$(CC) $(OBJS) -o $(TARGET) #-lefence 

sudba.o: sudba.c $(INCLUDE)/sudba.h

sudba_interface.o: sudba_interface.c $(INCLUDE)/sudba.h

# Remove everything that can be rebuilt
clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	@echo "CREATE TABLE FOO;" | ./$(TARGET)
	@echo "This is a long test command" | ./$(TARGET) 
	@echo "Check the system log file!"
