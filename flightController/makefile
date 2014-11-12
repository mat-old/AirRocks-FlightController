
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
all: START $(HPP)  $(EXEC_C)
	@echo ' + linking'	
	@$(CXX) $(LI_FLAGS) $(CC_FLAGS) $(EXEC_C) $(HPP) -o $(EXEC)
	@echo ' Build Complete'

.object/%.o:Modes/%.cpp
	@echo ' +' $<
	@$(CXX) -c $(CC_FLAGS) $< -o $@
.object/%.o:Systems/%.cpp  
	@echo ' +' $<
	@$(CXX) -c $(CC_FLAGS) $< -o $@
.object/%.o:Cores/%.cpp
	@echo ' +' $<
	@$(CXX) -c $(CC_FLAGS) $< -o $@
.object/%.o:Types/%.cpp
	@echo ' +' $<
	@$(CXX) -c $(CC_FLAGS) $< -o $@
.object/Defines.o:Defines.cpp
	@echo ' +' $<
	@$(CXX) -c $(CC_FLAGS) $< -o $@
.object/$(Main).o:$(Main).cpp
	@echo ' +' $<
	@$(CXX) -c $(CC_FLAGS) $< -o $@

build: START $(HPP)  $(EXEC_C) END

new: clean PRE START all


START: PRE
	@echo 
	@echo ' Building' $(EXEC)	
END:
	@echo ' Build Complete'
PRE:
	@mkdir -p $(OUT)



clean:
	rm -f ./*.o $(EXEC) ./$(OUT)*.o
test: p-HPP p-TYPES
p-%  : 
	 @echo
	 @echo   $* = $($*)