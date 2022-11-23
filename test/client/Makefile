# 编译参数
CC = g++
CFLAGS = -std=c++17
INCLUDE = -I../../include
LIBS = -L./libs
GRPCFLAG = `pkg-config --cflags protobuf grpc` \
	`pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl

# 源文件
SOURCE = \
	client.cpp \
	../../src/grpc/IPC.grpc.pb.cc \
	../../src/grpc/IPC.pb.cc

# 目标文件
OBJS = $(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(notdir $(SOURCE))))

client: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LIBS) $(GRPCFLAG)

$(OBJS) : $(SOURCE)
	$(CC) -c $^ $(CFLAGS) $(INCLUDE) $(LIBS) $(GRPCFLAG)

.PHONY = clean
clean:
	-rm -r *.o