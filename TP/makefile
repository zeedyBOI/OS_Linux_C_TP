all: gestor cliente verif cleaner

gestor: gestor.o
	gcc gestor.o -o gestor

gestor.o: gestor.c gestor.h
	gcc gestor.c -c

cliente: cliente.o
	gcc cliente.o -o cliente

cliente.o: cliente.c cliente.h
	gcc cliente.c -c

verif: verif.o
	gcc verif.o -o verif

verif.o: verif.c
	gcc verif.c -c

cleaner: 
	rm -f *.o
