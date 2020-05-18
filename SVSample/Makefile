CPP=g++

CFLAGS=-g -O2  -std=c++11 -Wall 
BINDIR =.

LDFLAGS= -lpng -lsvgensdk -L../../bins/SVCam_Kit_v2.5.0_Linux_AMD64/Genicam_x64/runtime/bin/Linux64_x64 -lGCBase_gcc421_v3_0 -lGenApi_gcc421_v3_0 -llog4cpp_gcc421_v3_0 -lLog_gcc421_v3_0 -lMathParser_gcc421_v3_0 -lNodeMapData_gcc421_v3_0 -lXmlParser_gcc421_v3_0


EXECUTABLE=$(BINDIR)/sample

all: $(BINDIR) $(EXECUTABLE)

$(BINDIR):
	mkdir $(BINDIR)


$(EXECUTABLE): sample.o
	$(CPP) $(CFLAGS) -o $(EXECUTABLE) sample.o $(LDFLAGS)

sample.o: sample.cpp 
	$(CPP) $(CFLAGS)  $(INCPATH) -c sample.cpp -o $@ 

clean:
	rm -f *.o *~ $(EXECUTABLE) sample $@
	rm *.png
