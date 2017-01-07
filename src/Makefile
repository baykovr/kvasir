CPP=gcc
CSRCFLAGS=-pthread  -O3 

DBG=-Wall -Wextra -Wuninitialized -pedantic -g
OBJ= kvasir.o scan.o

kvasir: $(OBJ)
	$(CPP) -o $@ $^ $(CSRCFLAGS)

%.o: $.c
	$(CPP) $(CSRCFLAGS) -c $< -o $@

debug: $(OBJ)
	$(CPP) -o $@ $^ $(DBG)

clean:
	rm *.o 
