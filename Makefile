# Compilador e flags
CC = gcc
CFLAGS = -Iinclude -Wall -g
LIBS = -lfreeglut -lopengl32 -lglu32

# Diretórios
OBJDIR = obj
SRCDIR = src

# Executável
TARGET = tankwars.exe

# Lista todos os .c em src
SRCS = $(wildcard $(SRCDIR)/*.c)

# Converte src/arquivo.c -> obj/arquivo.o
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Regra padrão
all: $(OBJDIR) $(TARGET)

# Cria a pasta obj se não existir
$(OBJDIR):
	if not exist $(OBJDIR) mkdir $(OBJDIR)

# Linka os objetos para gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Regra para criar cada objeto em obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa objetos e executável
clean:
	del /Q $(OBJDIR)\*.o $(TARGET)
