.SUFFIXES:.c .o

OBJS_DIR = obj
GLOBAL_DIR = global


SRCS = main.c \
        $(GLOBAL_DIR)/global_variable.c
OBJS = $(SRCS:%.c=%.o)
EXEC = demo

CC = gcc
CC_FLAGS = -g -Wall -O0
INCLUDE = 
LINK = 

.PHONY:all default clean distclean

default:all $(OBJS)
	$(CC) $(CC_FLAGS) $(LINK)  $(addprefix $(OBJS_DIR)/, $(notdir $(OBJS))) -o $(EXEC)
	@echo "----- ^_^ build ok ^_^ -----"

all:
ifneq ($(OBJS_DIR), $(wildcard $(OBJS_DIR)))
	@mkdir $(OBJS_DIR)
endif

$(OBJS): %.o : %.c
	$(CC) $(CC_FLAGS) $(INCLUDE) -c $< -o $(OBJS_DIR)/$(notdir $@)

clean:
	@rm -rf $(OBJS_DIR)/*.o *.o
	@echo "----- ^_^ clean ok ^_^ -----"

distclean:clean
	rm -f $(EXEC)
	rm -rf $(OBJS_DIR)
