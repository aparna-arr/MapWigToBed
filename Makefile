CXX := g++
CXXFLAGS := -Wall -std=c++11 -g
LDFLAGS := -lssl -lcrypto

SRC_CACHE := ../Cache/Cache.cpp ../Cache/Debug.cpp ../Cache/MD5sum.cpp ../Cache/Serialize.cpp ../Cache/Utils.cpp 
SRC_PRG := Main.cpp MapInput.cpp Signal.cpp BedFile.cpp $(SRC_CACHE) 

OBJ := $(SRC_PRG:.cpp=.o)

EXEC := mapwigtobed

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) $(EXEC)
