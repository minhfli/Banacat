########################################################################
####################### Makefile Template ##############################
########################################################################

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

APP = bin/MyGame
SRC = $(call rwildcard,src,*.cpp )
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

COMPILER_FLAGS = -std=c++14 -Wall -pedantic -MMD -MP 

#? addtional flag for release version
R_COMPILER_FLAGS = $(COMPILER_FLAGS) -mwindows -w

INCLUDE_FLAGS = -Isrc -Ivendor/include
LIB_FLAGS = 
LINKER_FLAGs =

#* SDL flags
LIB_FLAGS += -Lvendor/lib/SDL2
LINKER_FLAGs  += -lmingw32 -lSDL2main -lSDL2 

#* GLAD flags
LIB_FLAGS += -Lvendor/lib/GLAD
LINKER_FLAGs +=  -lglad -lopengl32

#* STB_IMAGE flags
LIB_FLAGS += -Lvendor/lib/STB
LINKER_FLAGs +=  -lstb_image

#* GLM flags

.PHONY: all 
all: 
	g++ $(COMPILER_FLAGS) $(INCLUDE_FLAGS) $(LIB_FLAGS) $(SRC) -o $(APP).exe $(LINKER_FLAGs)

.PHONY: release
release: 
	g++ $(R_COMPILER_FLAGS) -O3 $(INCLUDE_FLAGS) $(LIB_FLAGS) $(SRC) -o $(APP).exe $(LINKER_FLAGs)  

.PHONY: run
run: 
	.\$(APP)
