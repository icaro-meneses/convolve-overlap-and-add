#
# Arquivo de Makefile
#

# Nome do projeto
PROJ_NAME=test_convolve

# Arquivos de código fonte .c
C_SOURCE=$(wildcard *.c)

# Arquivos de cabeçalho .h
H_SOURCE=$(wildcard *.h)

OCTAVE_SOURCE=$(wildcard *.m)

# Arquivos objeto .o
OBJ=$(C_SOURCE:.c=.o)

# Compilador
CC=gcc

# Flags para o compilador
CC_FLAGS=-c \
	 -O \
	 -W \
	 -Wall \
	 -pedantic \
	 -std=c99 \
	 -Wpedantic

LIBS=-lm

#
# Compilação e Linkagem
#


rebuild-run: clean build run

# 1º Passo: Início do processo de build
build:	$(PROJ_NAME)

# 3º Passo: Linkagem
$(PROJ_NAME):	$(OBJ)
	@echo " "
	@echo "2 - Starting program linking..."
	$(CC) $^ -o $@ $(LIBS)
	@echo " "
	@echo "Program building completed."
	@echo " "

%.o:	%.c %.h
	@echo " "
	$(CC) $(CC_FLAGS) $< -o $@

# 2º Passo: Compilação
main.o:	main.c $(H_SOURCE)
	@echo " "
	@echo "## Starting program building:"
	@echo "1 - Starting program compiling..."
	$(CC) $(CC_FLAGS) $< -o $@
	@echo " "

# Processo de Limpeza
clean:
	@echo " "
	@echo "Removing object files and executable files..."
	rm *.o *.aux *.log *.tex *.pdf *.txt $(PROJ_NAME)
	@echo "Cleaning Completed."
	@echo " "

# Processo de execução do programa
run:
	@echo " "
	@./$(PROJ_NAME)
	@octave $(OCTAVE_SOURCE) 
	@echo " "

.PHONY:	build clean run

