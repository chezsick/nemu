##### global settings #####

.PHONY: nemu entry all_testcase kernel run gdb test submit clean

CC := arm-linux-gcc
LD := ld
CFLAGS := -MMD -Wall -Werror -c

LIB_COMMON_DIR := lib-common
NEWLIBC_DIR := $(LIB_COMMON_DIR)/newlib
NEWLIBC := $(NEWLIBC_DIR)/libc.a
#FLOAT := obj/$(LIB_COMMON_DIR)/FLOAT.a

include config/Makefile.git
include config/Makefile.build

all: nemu


testcase_CFLAGS_EXTRA := -m32 -fno-builtin -fno-stack-protector \
						-I$(LIB_COMMON_DIR) -I$(NEWLIBC_DIR)/include

$(eval $(call make_common_rules,testcase,$(testcase_CFLAGS_EXTRA)))

# redefine testcase_BIN
testcase_BIN := $(testcase_COBJS:.o=)
testcase_START_OBJ := $(testcase_OBJ_DIR)/start.o
testcase_LDFLAGS := -m elf_i386 -e start -Ttext=0x00100000 

$(testcase_BIN): % : $(testcase_START_OBJ) %.o $(FLOAT) $(NEWLIBC)
	$(call make_command, $(LD), $(testcase_LDFLAGS), ld $@, $^)
	@objdump -d $@ > $@.txt

$(testcase_OBJ_DIR)/mov: % : %.o
	$(call make_command, $(LD), $(testcase_LDFLAGS), ld $@, $^)
	@objdump -d $@ > $@.txt
##### rules for building the project #####

include nemu/Makefile.part
include testcase/Makefile.part
include lib-common/Makefile.part
include kernel/Makefile.part
include game/Makefile.part

nemu: $(nemu_BIN)
all_testcase: $(testcase_BIN)
kernel: $(kernel_BIN)
game: $(game_BIN)

##### rules for cleaning the project #####

clean-nemu:
	-rm -rf obj/nemu 2> /dev/null

clean-testcase:
	-rm -rf obj/testcase 2> /dev/null

clean-kernel:
	-rm -rf obj/kernel 2> /dev/null

clean-game:
	-rm -rf obj/game 2> /dev/null

clean: clean-cpp
	-rm -rf obj 2> /dev/null
	-rm -f *log.txt entry $(FLOAT) 2> /dev/null


##### some convinient rules #####

USERPROG := obj/testcase/mov-c
ENTRY := $(USERPROG)

entry: $(ENTRY)
	objcopy -S -O binary $(ENTRY) entry

run: $(nemu_BIN) $(USERPROG) entry
	$(call git_commit, "run")
	$(nemu_BIN) $(USERPROG)

gdb: $(nemu_BIN) $(USERPROG) entry
	gdb -s $(nemu_BIN) --args $(nemu_BIN) $(USERPROG)

test: $(nemu_BIN) $(testcase_BIN) entry
	bash test.sh $(testcase_BIN)

submit: clean
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2
count: $(nemu_BIN) $(USERPROG) entry
	git checkout b718
	cd nemu && find . -name '*.[ch]'|xargs cat|wc -l
	git checkout master
	cd nemu && find . -name '*.[ch]'|xargs cat|wc -l

