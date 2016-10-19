# https://sites.google.com/site/michaelsafyan/software-engineering/how-to-write-a-makefile

CFLAGS += -g

program_NAME := todo
program_SRC_SUBDIRS := $(wildcard */)
program_SRCS := $(wildcard *.c) $(wildcard $(addsuffix *.c,$(program_SRC_SUBDIRS)))
program_OBJS := ${program_SRCS:.c=.o}

.PHONY: all clean distclean 

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(LINK.c) $(program_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean: clean


#print a variable
print-%:
	@echo $* = $($*)

# Don't know why the following is showing erro
#
# define OBJECT_DEPENDS_ON_CORRESPONDING_HEADER
# 	$(1) : ${1:.o=.h}
# endef
# 
# $(foreach object_file,$(program_OBJS),$(eval $(call OBJECT_DEPENDS_ON_CORRESPONDING_HEADER,$(object_file))))
