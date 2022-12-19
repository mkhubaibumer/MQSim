CC        := g++
LD        := g++
CC_FLAGS := -std=c++14 -O0 -g3 -ggdb3 -fPIC

MODULES   := exec host nvm_chip nvm_chip/flash_memory sim ssd utils lib
SRC_DIR   := $(addprefix src/,$(MODULES)) src
BUILD_DIR := $(addprefix build/,$(MODULES)) build

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) -DBUILD_LIB $(CC_FLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: clean checkdirs libmqsim.so

libmqsim.so: $(OBJ)
	$(CC) -DBUILD_LIB $(CC_FLAGS) -shared $^ -o $@

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f libmqsim.so

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
