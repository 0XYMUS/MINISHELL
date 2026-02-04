# ============================================================================ #
#                                 VARIABLES                                    #
# ============================================================================ #

NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3 
# -g3 se usa para usar valgrind/gdb
# sirve para generar mas info en caso de errores al compilar

# Directorios
SRC_DIR		= srcs
OBJ_DIR		= objects
INC_DIR		= include

# Archivos Fuente (Añade aquí tus nuevos archivos .c)
# Archivos Fuente
SRC_FILES	=	main.c \
				utils/token_utils.c \
				utils/errors.c \
				utils/utils.c \
				utils/parse_utils.c \
				token/tokenization.c \
				token/token_manage.c \
				parse/parse.c \
				parse/parse_error.c \
				parse/validate_syntax.c \
				parse/pipeline_utils.c \
				parse/free_pipeline.c \
				builtins/echo.c \
				builtins/pwd.c \
				builtins/env.c \
				exec/exec.c \

# Generación automática de rutas
SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Librerías (Readline)
LIBS        = -lreadline -lhistory -lncurses
INCLUDES    = -I$(INC_DIR)

# ============================================================================ #
#                                  COLORES                                     #
# ============================================================================ #

DEF_COLOR   = \033[0;39m
GRAY        = \033[0;90m
RED         = \033[0;91m
GREEN       = \033[0;92m
YELLOW      = \033[0;93m
BLUE        = \033[0;94m
MAGENTA     = \033[0;95m
CYAN        = \033[0;96m
WHITE       = \033[0;97m

# ============================================================================ #
#                                   REGLAS                                     #
# ============================================================================ #

all: $(NAME)

# Regla principal: Linkeo
$(NAME): $(OBJS)
	@echo "$(YELLOW)Enlazando objetos...$(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@
	@echo "$(GREEN)✨ Minishell compilada con éxito! ✨$(DEF_COLOR)"
	@$(call PRINT_ASCII_ART)

# Regla de compilación de objetos (Crea la carpeta obj si no existe)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compilando: $(GRAY)$<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpieza de objetos
clean:
	@echo "$(RED)Limpiando archivos objeto...$(DEF_COLOR)"
	@rm -rf $(OBJ_DIR)

# Limpieza total
fclean: clean
	@echo "$(RED)Eliminando ejecutable...$(DEF_COLOR)"
	@rm -f $(NAME)

# Re-compilación
re: fclean all

# Evita conflictos con archivos que se llamen igual que las reglas
.PHONY: all clean fclean re

# ============================================================================ #
#                                 ASCII ART                                    #
# ============================================================================ #

define PRINT_ASCII_ART
echo "$(MAGENTA)"
echo "███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     "
echo "████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     "
echo "██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     "
echo "██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     "
echo "██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
echo "╚═╝     ╚═╝╚═╝╚═╝  ╚╦══╝╚═╝╚══════╝╚═╝  ╚═╝╚═╦════╝╚══════╝╚══════╝"
echo "                    ╠═by jojeda-p & julepere═╣                     "
echo "                    ╚════════════════════════╝                     "
																   
echo "$(DEF_COLOR)"
endef