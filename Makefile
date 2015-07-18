GLFW = `pkg-config --cflags glfw3` `pkg-config --libs --static glfw3`
GLEW = `pkg-config --cflags glew` `pkg-config --libs glew`
common = ./common/shader.cpp ./common/objloader.cpp ./common/controls.cpp
other = audio_data.cpp play_wav.cpp
build_path = build/
lib = -lasound -lpthread
inc = -I. -I/usr/local/include/GLFW/
macro = -D_REENTRANT
target = visual

all: $(target)

$(target): $(target).cpp
	clang++ $(macro) $(target).cpp $(common) $(other) -o $(build_path)$(target) $(GLFW) $(GLEW) $(lib) $(inc)
