#-------------------------------------------------------------------------------------------------------
#																									   #
#								Makefile for libModbus source file 									   #
#																									   #
#-------------------------------------------------------------------------------------------------------


PROJECT			=  	ModbusCS

CC				=	gcc

CFLAGS			=	-Werror -std=c99
CFLAGS         += 	-Wall
CFLAGS	   	   +=   -g

SUBDIRS 		=   src/frame

export CC CXXFLAGS


#-------------------------------------------------------------------------------------------------------
#																									   #
#											Make rules  								   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------


.PHONY: all clean $(SUBDIRS)

all:$(SUBDIRS)
	ar -rcs $(PROJECT).a $(shell find ./ -name "*.o")

$(SUBDIRS):
	$(MAKE) -C $@	

tags:
	@rm -rf ./tags
	ctags --exclude="docs"			\
		  --exclude="*.o"			\
		  --exclude="*.a"			\
		  --exclude="*.so"			\
		  --exclude="*.elf"			\
		  --exclude="*.map"			\
		  --exclude="*.lss" -R .

clean:
	for dir in $(SUBDIRS); do 		\
		$(MAKE) -C $$dir clean;		\
	done
	@rm -rf $(PROJECT).*

