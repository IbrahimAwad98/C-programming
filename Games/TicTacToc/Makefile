# Källkodens plats
SRCDIR = ./src
LIBS = ./include

# Kompilator
CC = gcc

# Inkluderingskataloger
CFLAGS = -g -I$(LIBS)

# Utdatafil (exekverbar fil)
TARGET = TicTacToc.exe

# Alla C-filer
SRCS = $(SRCDIR)/main.c $(SRCDIR)/game.c

# Bygg målet
all:
	@echo "Bygger $(TARGET)..."
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Ta bort exekverbara filen
clean:
	rm -f $(TARGET)

# Kör spelet
run: all
	./$(TARGET)
