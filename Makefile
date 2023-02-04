# 编译参数
CC = g++ -g
CFLAGS = -std=c++17
INCLUDE = -I./include
LIBS = -L./libs -lspdlog -lyaml-cpp
GRPCFLAG = `pkg-config --cflags protobuf grpc` \
	`pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl
ALLFLAGS = $(INCLUDE) $(LIBS) $(GRPCFLAG) $(CFLAGS)

# 文件位置
BUILD_DIR = build
BIN_DIR = bin
EXEC = server
RECORDER = recorder

# 目标对象
OBJS = $(addprefix $(BUILD_DIR)/, $(patsubst src/%.cpp, %.o, $(shell find src -name *.cpp | xargs)) $(patsubst src/%.cc, %.o, $(shell find src -name *.cc | xargs)))

all: $(BIN_DIR)/$(EXEC) $(RECORDER)

$(BIN_DIR)/$(EXEC): $(OBJS)
	-mkdir $(BIN_DIR)
	$(CC) -o $@ $^ $(ALLFLAGS)

$(BUILD_DIR)/%.o: src/%.cpp
	-mkdir -p $(@D)
	$(CC) -o $@ -c $^ $(ALLFLAGS)

$(BUILD_DIR)/%.o: src/%.cc
	-mkdir -p $(@D)
	$(CC) -o $@ -c $^ $(ALLFLAGS)

$(RECORDER):
	$(MAKE) -f record/Makefile

.PHONY: clean
clean:
	-rm -r $(BUILD_DIR)
	-rm -r $(BIN_DIR)