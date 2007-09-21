LEX=flex
LFLAGS=-8

YACC=bison
YFLAGS=-d

LIBOBJ=miString.o miDate.o miClock.o miTime.o miCommandLine.o miDirtools.o ttycols.o

LIBSRC=$(LIBOBJ:.o=.cc)

VPATH=$(LIBDIR)

.SUFFIXES: .cc .o

.cc.o:
	$(CXX) $(CCFLAGS) -c $< $(INCLUDE)

all: $(LIBDIR)/libpuTools.a

depends:
	rm -f $(DEPENDSFILE)
	echo '# Automatically-generated dependencies list:' > $(DEPENDSFILE)
	$(CXX) $(CCFLAGS) $(INCLUDE) -M $(LIBSRC) >> $(DEPENDSFILE)

$(LIBDIR)/libpuTools.a: $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $(LIBOBJ)

include $(DEPENDSFILE)
