
OBJS    = crc lrc frame
SUBDIRS = 
NAMEDIR = $(shell dirname `pwd`)
 
 
#-------------------------------------------------------------------------------------------------------
#																									   #
#										  Make rules 									   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------


.PHONY: all clean $(SUBDIRS)

all:
	for i in $(OBJS);													   						 \
	do															    	   						 \
		$(CXX) $(CXXFLAGS) "$$i".c -L$(NAMEDIR) -lmodbcd -Wl,-rpath=$(NAMEDIR) -o "$$i".out; 	 \
	done

.PHONY:clean
clean:
	rm -rf *.out


