
CC_FLAGS = -std=c++0x -pthread
LI_FLAGS = 
EXEC     = UAVapp.o
OUT      = .object/

#Types
Types = $(wildcard Types/*.cpp)
Cores = $(wildcard Cores/*.cpp)
Sys   = $(wildcard Systems/*.cpp)
Modes = $(wildcard Modes/*.cpp)
Def   = Defines
Main  = UAVmain

#Extensions
HPP   = $(OUT)$(DEF).o
HPP  =  $(Types:Types/%.cpp=$(OUT)%.o)
HPP  += $(Cores:Cores/%.cpp=$(OUT)%.o)
HPP  += $(Sys:Systems/%.cpp=$(OUT)%.o)
HPP  += $(Modes:Modes/%.cpp=$(OUT)%.o) 

EXEC_C= $(OUT)$(Main).o

# Main target
all: $(HPP)  $(EXEC_C) 
	$(CXX) $(LI_FLAGS) $(CC_FLAGS) $(EXEC_C) $(HPP) -o $(EXEC)

.object/%.o:Modes/%.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@
.object/%.o:Systems/%.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@
.object/%.o:Cores/%.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@
.object/%.o:Types/%.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@
.object/Defines.o:Defines.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@


.object/$(Main).o:$(Main).cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@

# Do work before compilation
PRE:
	@mkdir -p $(OUT)
clean:
	rm -f ./*.o $(EXEC) ./$(OUT)*
test: p-HPP p-TYPES
p-%  : 
	 @echo
	 @echo   $* = $($*)