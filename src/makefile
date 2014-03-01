CPP=gcc
CSRCFLAGS=-pthread  -O3 
C_DBG=-Wall -Wextra -Wuninitialized -pedantic -g
OBJ= kvasir.o scan.o

k-debug: $(OBJ)
	$(CPP) -o $@ $^ $(C_DBG)

kvasir: $(OBJ)
	$(CPP) -o $@ $^ $(CSRCFLAGS)

%.o: $.c
	$(CPP) $(CSRCFLAGS) -c $< -o $@
	
clean:
	rm *.o 