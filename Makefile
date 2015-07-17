GLFW = `pkg-config --cflags glfw3` `pkg-config --libs --static glfw3`
GLEW = `pkg-config --cflags glew` `pkg-config --libs glew`
common = ./common/shader.cpp ./common/text2D.cpp ./common/objloader.cpp ./common/controls.cpp ./common/texture.cpp
lib = audio_data.cpp
build = build/
target = visual

all: $(target)

$(target): $(target).cpp
	clang++ -I/usr/local/include/GLFW/ -I. $(target).cpp -o $(build)$(target) $(common) $(lib) -lasound $(GLFW) $(GLEW)