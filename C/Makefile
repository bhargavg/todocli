# https://sites.google.com/site/michaelsafyan/software-engineering/how-to-write-a-makefile


program_NAME := todo
program_SRC_SUBDIRS := $(filter-out tests/,$(wildcard */))
program_SRCS := $(wildcard *.c) $(wildcard $(addsuffix *.c,$(program_SRC_SUBDIRS)))
program_OBJS := ${program_SRCS:.c=.o}

program_INCLUDE_DIRS := ./libs/libtodo/
program_LIBRARY_DIRS := ./libs/libtodo/
program_LIBRARIES := todo

test_NAME := todo-tests
test_OBJS := $(filter-out main.o,$(program_OBJS)) tests/main.o

CFLAGS  += -g
CFLAGS  += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

INSTALL_DIR = /usr/local/bin/

.PHONY: all clean distclean uninstall test

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	cd libs/libtodo; make
	$(LINK.c) $(program_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(test_NAME)
	@- $(RM) $(program_OBJS)
	@- $(RM) $(test_OBJS)
	@- cd libs/libtodo; make clean

install: all
	cp -f $(program_NAME) $(INSTALL_DIR)

uninstall:
	rm $(INSTALL_DIR)$(program_NAME)

distclean: clean

test: $(test_NAME)
	./$<

$(test_NAME) : $(test_OBJS)
	cd libs/libtodo; make
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
