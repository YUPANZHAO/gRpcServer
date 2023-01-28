# 编译参数
CC = g++ -g
CFLAGS = -std=c++17
INCLUDE = -I./include
LIBS = -L./libs -lspdlog -lyaml-cpp
GRPCFLAG = `pkg-config --cflags protobuf grpc` \
	`pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl
ALLFLAGS = $(CFLAGS) $(INCLUDE) $(LIBS) $(GRPCFLAG)

# 文件位置
BUILD_DIR = build
BIN_DIR = bin

# 源文件
SOURCE = \
	src/main.cpp \
	src/apiServer.cpp \
	src/grpc/IPCService.cpp \
	src/grpc/grpcServer.cpp \
	src/grpc/IPC.grpc.pb.cc \
	src/grpc/IPC.pb.cc \
	src/logger/spdLogger.cpp \
	src/logger/loggerManager.cpp \
	src/config/yamlConfigDec.cpp \
	src/api/loginApiPlugin.cpp \
	src/utils/KeyGenerator.cpp \
	src/utils/DeviceManager.cpp \
	src/api/genKeyApiPlugin.cpp \
	src/api/getDeviceInfoApiPlugin.cpp \
	src/api/msgCBApiPlugin.cpp \
	src/api/talkCtrlApiPlugin.cpp \
	src/api/deviceQuitApiPlugin.cpp \
	src/api/heartBeatApiPlugin.cpp \
	
# 目标文件
OBJS = $(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(notdir $(SOURCE))))

server: $(OBJS)
	$(CC) -o $@ $^ $(ALLFLAGS)
	mkdir $(BUILD_DIR)
	mkdir $(BIN_DIR)
	mv *.o $(BUILD_DIR)
	mv $@ $(BIN_DIR)

$(OBJS) : $(SOURCE)
	$(CC) -c $^ $(ALLFLAGS)

.PHONY: clean
clean:
	-rm -r *.o
	-rm -r $(BUILD_DIR)
	-rm -r $(BIN_DIR)