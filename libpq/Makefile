.SUFFIXES:.c .o

OBJS_DIR = obj

SRCS = main.c
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)
EXEC = demo

CC = gcc
CC_FLAGS = -g -Wall -O0
INCLUDE = 
LINK = -lpq

.PHONY:all default clean distclean

default:all $(OBJS)
	$(CC) $(CC_FLAGS) $(LINK) $(OBJS) -o $(EXEC)
	@echo "----- ^_^ build ok ^_^ -----"

all:
ifneq ($(OBJS_DIR), $(wildcard $(OBJS_DIR)))
	@mkdir $(OBJS_DIR)
endif

$(OBJS): $(OBJS_DIR)/%.o : %.c
	$(CC) $(CC_FLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)/*.o *.o
	@echo "----- ^_^ clean ok ^_^ -----"

distclean:clean
	rm -f $(EXEC)
	rm -rf $(OBJS_DIR)