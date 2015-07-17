GLFW = `pkg-config --cflags glfw3` `pkg-config --libs --static glfw3`
GLEW = `pkg-config --cflags glew` `pkg-config --libs glew`
common = ./common/shader.cpp ./common/text2D.cpp ./common/objloader.cpp ./common/controls.cpp ./common/texture.cpp
other = audio_data.cpp play_wav.cpp
build_path = build/
lib = -lasound -lpthread
inc = -I.
macro = -D_REENTRANT
target = visual

all: $(target)

$(target): $(target).cpp
	clang++ $(macro) -I/usr/local/include/GLFW/ $(target).cpp $(common) $(other) -o $(build_path)$(target) $(GLFW) $(GLEW) $(lib) $(inc)
