PROG=unplot2
OBJECTS=main.o \
	frame.o \
	data-mark.o \
	ticks.o \
	label-rect.o \
	label-text.o \
	major.o \
	peak1i-match.o \
	peak2f-check.o \
	correlation.o \
	linear-lsq.o

CXX=clang++ -std=c++11
CXXFLAGS=-Wall -O3 `pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv` -lm -ltesseract

$(PROG) : $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

main.o : main.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

frame.o : frame.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c frame.cpp

data-mark.o : data-mark.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c data-mark.cpp

ticks.o : ticks.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c ticks.cpp

label-rect.o : label-rect.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c label-rect.cpp

label-text.o : label-text.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c label-text.cpp

major.o : major.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c major.cpp

peak1i-match.o : peak1i-match.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c peak1i-match.cpp

peak2f-check.o : peak2f-check.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c peak2f-check.cpp

correlation.o : correlation.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c correlation.cpp

linear-lsq.o : linear-lsq.cpp unplot2d.hpp
	$(CXX) $(CXXFLAGS) -c linear-lsq.cpp

clean :
	$(RM) $(PROG) *.o
