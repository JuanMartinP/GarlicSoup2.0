CC = gcc
CFLAGS = -Wall
EXE = ejercicio2 ejercicio4 ejercicio6a ejercicio6b ejercicio8 ejercicio9

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o -lm -pthread
	@echo "# Eliminando el objeto $<"
	rm $<
