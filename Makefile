GLFW = `pkg-config --cflags glfw3` `pkg-config --libs --static glfw3`
GLEW = `pkg-config --cflags glew` `pkg-config --libs glew`
LIBRARY = -lasound -lpthread
INCLUDE = -I$(SOURCE_PATH) -I/usr/local/include/GLFW/
MACRO = -D_REENTRANT

BINARY = waveform
SOURCE_PATH = source/
HPP = \
	$(SOURCE_PATH)audio_data/audio_data.hpp \
	$(SOURCE_PATH)play_wav/play_wav.hpp \
	$(SOURCE_PATH)common/shader.hpp \
	$(SOURCE_PATH)common/objloader.hpp \
	$(SOURCE_PATH)common/controls.hpp
CPP = \
	$(SOURCE_PATH)audio_data/audio_data.cpp \
	$(SOURCE_PATH)play_wav/play_wav.cpp \
	$(SOURCE_PATH)common/shader.cpp \
	$(SOURCE_PATH)common/objloader.cpp \
	$(SOURCE_PATH)common/controls.cpp $(SOURCE_PATH)waveform.cpp
BUILD_PATH = build/
TARGET = $(BUILD_PATH)$(BINARY)

all: $(TARGET)

$(TARGET): $(CPP) $(HPP)
	clang++ $(CPP) -o $(TARGET) $(GLFW) $(GLEW) $(LIBRARY) $(INCLUDE) $(MACRO)
