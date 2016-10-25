# https://sites.google.com/site/michaelsafyan/software-engineering/how-to-write-a-makefile


program_NAME := libtodo.a
program_SRC_SUBDIRS := $(filter-out tests/,$(wildcard */))
program_SRCS := $(wildcard *.c) $(wildcard $(addsuffix *.c,$(program_SRC_SUBDIRS)))
program_OBJS := ${program_SRCS:.c=.o}

test_NAME := todo-tests
test_OBJS := $(filter-out main.o,$(program_OBJS)) tests/main.o

CFLAGS  += -g

.PHONY: all clean distclean 

all: $(program_NAME)
$(program_NAME): $(program_OBJS)
	$(AR) rcs $@ $^

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(test_NAME)
	@- $(RM) $(program_OBJS)
	@- $(RM) $(test_OBJS)

distclean: clean

test: $(test_NAME)
	./$<

$(test_NAME) : $(test_OBJS)
	$(LINK.c) $(test_OBJS) -o $(test_NAME)


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
