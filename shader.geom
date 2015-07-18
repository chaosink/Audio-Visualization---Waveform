#version 430
#pragma optimize(off)
#pragma debug(on)

// Establish our primitive's input and output types, as well as the number of vertices that we will produce.
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {
	// GLSL provides us with the .length() method for arrays. It tells us the number of elements in the vector.
	for(int i = 0; i < gl_in.length(); ++i) {
		// gl_in is the standard input interface block
		gl_Position = gl_in[i].gl_Position;
		// The next function, will emit the computed vertex to thepipeline
		EmitVertex();
	}
	// The next function will close the primitive
	EndPrimitive();
}
