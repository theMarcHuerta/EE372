#MACROS
TARGET    = fir_filter_tb 
OBJECTS   = src/csvparser.o src/fir_filter.o src/fir_filter_tb.o
INCLUDES  = -I"$(MGC_HOME)/shared/include" 
CXX       =  $(MGC_HOME)/bin/g++
CXXFLAGS  = $(INCLUDES)

%.o: %.cpp 
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean
clean: 
	rm -f $(TARGET) $(OBJECTS) 

