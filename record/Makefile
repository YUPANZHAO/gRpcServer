CC = g++ -g
CFLAGS = -std=c++17
LIBS = -L./libs -lyaml-cpp -lrtmp -ldl -lpthread -lz -lyaml-cpp
INCLUDE = -I./include

BIN_DIR = bin
BUILD_DIR = build/record
EXEC = recorder

SOURCE = $(shell find record -name *.cpp | xargs)
SOURCE += $(shell find src/logger src/config -name *.cpp | xargs)
OBJS = $(addprefix $(BUILD_DIR)/,  $(patsubst src/%.cpp, %.o, $(patsubst record/%.cpp, %.o, $(SOURCE))))

$(BIN_DIR)/$(EXEC): $(OBJS)
	-mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(INCLUDE) $(LIBS) $(CFLAGS)

$(BUILD_DIR)/%.o: record/%.cpp
	-mkdir -p $(@D);
	$(CC) -o $@ -c $^ $(INCLUDE) $(LIBS) $(CFLAGS)

$(BUILD_DIR)/%.o: src/%.cpp
	-mkdir -p $(@D);
	$(CC) -o $@ -c $^ $(INCLUDE) $(LIBS) $(CFLAGS)