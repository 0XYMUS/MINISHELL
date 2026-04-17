# ============================================================================ #
#                                 VARIABLES                                    #
# ============================================================================ #

NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
# Auto-deps: recompila objetos cuando cambian headers (.h)
DEPFLAGS	= -MMD -MP
# -g3 se usa para usar valgrind/gdb
# sirve para generar mas info en caso de errores al compilar

# Directorios
SRC_DIR		= srcs
OBJ_DIR		= objects
INC_DIR		= include

# Archivos Fuente (Añade aquí tus nuevos archivos .c)
# Archivos Fuente
SRC_FILES	=	main.c \
				utils/utils.c \
				utils/utils_1.c \
				utils/token_utils.c \
				utils/parse_utils.c \
				utils/export_utils.c \
				utils/unset_utils.c \
				errors/error.c \
				errors/error_print.c \
				core/env_manage.c \
				core/initialize.c \
				core/shell.c \
				token/tokenization.c \
				token/tokenization_aux.c \
				token/token_manage.c \
				parse/parse.c \
				parse/validate_syntax.c \
				parse/qmask.c \
				parse/pipeline_utils.c \
				parse/free_pipeline.c \
				builtins/echo.c \
				builtins/cd.c \
				builtins/pwd.c \
				builtins/export.c \
				builtins/unset.c \
				builtins/env.c \
				builtins/exit.c \
				exec/exec_utils.c \
				exec/child_process.c \
				exec/exec.c \
				expand/expand.c \
				expand/expand_env.c \
				expand/expand_special.c \
							expand/expand_qmask.c \
							expand/expand_cleanup.c \
							expand/expand_utils.c \
				expand/expand_auxiliars.c \
				

# Generación automática de rutas
SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
DEPS        = $(OBJS:.o=.d)

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
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Limpieza de objetos
clean:
	@echo "$(RED)Limpiando archivos objeto...$(DEF_COLOR)"
	@rm -rf $(OBJ_DIR)

# Incluye dependencias generadas (si existen)
-include $(DEPS)

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