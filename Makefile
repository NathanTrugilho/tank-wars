# Makefile Cross-Platform (Windows e Linux)

# Compilador padrão
CC = gcc
# Flags de compilação (comuns para ambos os sistemas)
CFLAGS = -Iinclude -Wall -g

# --- Detecção Automática do Sistema Operacional ---
ifeq ($(OS),Windows_NT)
    # Configurações para WINDOWS
    TARGET      = tankwars.exe
    LIBS        = -lfreeglut -lopengl32 -lglu32
    RM          = del /Q
    MKDIR_P     = if not exist $(1) mkdir $(1)
    SLASH       = \\
else
    # Configurações para LINUX (e outros Unix-like)
    TARGET      = tankwars
    # ADICIONADO -lm PARA A BIBLIOTECA MATEMÁTICA
    LIBS        = -lglut -lGL -lGLU -lm
    RM          = rm -f
    MKDIR_P     = mkdir -p
    SLASH       = /
endif
# --- Fim da Detecção ---

# Diretórios
OBJDIR = obj
SRCDIR = src

# Lista todos os arquivos .c em SRCDIR
SRCS = $(wildcard $(SRCDIR)/*.c)
# Converte a lista de fontes (.c) para uma lista de objetos (.o)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Regra padrão
all: $(OBJDIR) $(TARGET)

# Linka todos os arquivos objeto para criar o executável final
$(TARGET): $(OBJS)
	@echo "Ligando os arquivos objeto para criar [$(TARGET)]..."
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)
	@echo "Compilação finalizada com sucesso!"

# Regra para compilar cada arquivo .c em um arquivo .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para criar o diretório de objetos
$(OBJDIR):
	@echo "Verificando diretório de objetos [$(OBJDIR)]..."
	$(call MKDIR_P,$(OBJDIR))

# Regra para limpar o projeto
clean:
	@echo "Limpando arquivos de compilação..."
	$(RM) $(OBJDIR)$(SLASH)*.o
	$(RM) $(TARGET)

.PHONY: all clean