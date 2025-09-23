# 编译器和选项
CXX = g++
CXXFLAGS = -std=c++17 -Wall  -g -O0

# Homebrew 路径
SDL2_DIR      = /opt/homebrew/opt/sdl2
PORTAUDIO_DIR = /opt/homebrew/opt/portaudio
FFTW_DIR      = /opt/homebrew/opt/fftw
LIBSNDFILE_DIR = /opt/homebrew/opt/libsndfile

# 头文件搜索路径
INCLUDES = -I$(SDL2_DIR)/include \
           -I$(PORTAUDIO_DIR)/include \
           -I$(FFTW_DIR)/include \
           -I$(LIBSNDFILE_DIR)/include

# 库搜索路径
LDFLAGS = -L$(SDL2_DIR)/lib \
          -L$(PORTAUDIO_DIR)/lib \
          -L$(FFTW_DIR)/lib \
          -L$(LIBSNDFILE_DIR)/lib

# 需要链接的库
LIBS = -lSDL2 -lportaudio -lfftw3 -lm -lsndfile

# 源文件和目标文件
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# 生成的可执行文件
TARGET = mvj

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
