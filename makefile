#gcc sniffer.c -o sniffer -lpcap

all: myping

myping: myping.c 
	gcc  -Wall  -o myping myping.c

myping.o: myping.c 
	gcc -Wall -c $*.c

	
.PHONY: clean all  

clean:
	rm -f *.a *.so *.o sniffer myping
		
	


