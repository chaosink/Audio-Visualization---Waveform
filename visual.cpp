// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/controls.hpp>
#include "audio_data.hpp"
#include "play_wav.hpp"

#define max(a, b) ((a)>(b)?(a):(b))

const int fps = 60;
const int cube_height = 4;

void *play_wav_d(void *file) {
	play_wav((char *)file);
	return NULL;
}

int main(int argc, char **argv)
{
	if(argc != 2) exit(0);
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//	glfwSetInputMode(window, GLFW_CURSOR_DISABLED, GL_TRUE);

	// Dark blue background
	glClearColor(0.f / 255.f, 15.f / 255.f, 0.f / 255.f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vert", "ColorFragmentShader.frag" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint objectID = glGetUniformLocation(programID, "object");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(10,15,20), // Camera is at (4,3,-3), in World Space
								glm::vec3(0,5,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
	glm::mat4 PV         = Projection * View;

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data1[] = { 
		-1.0f-1.5f,-1.0f+1.0f,-1.0f, //left
		-1.0f-1.5f,-1.0f+1.0f, 1.0f,
		-1.0f-1.5f, 1.0f+1.0f, 1.0f,
		-1.0f-1.5f,-1.0f+1.0f,-1.0f,
		-1.0f-1.5f, 1.0f+1.0f,-1.0f,
		-1.0f-1.5f, 1.0f+1.0f, 1.0f,
		 1.0f-1.5f,-1.0f+1.0f,-1.0f, //right
		 1.0f-1.5f,-1.0f+1.0f, 1.0f,
		 1.0f-1.5f, 1.0f+1.0f, 1.0f,
		 1.0f-1.5f,-1.0f+1.0f,-1.0f,
		 1.0f-1.5f, 1.0f+1.0f,-1.0f,
		 1.0f-1.5f, 1.0f+1.0f, 1.0f,
		-1.0f-1.5f,-1.0f+1.0f, 1.0f, //front
		 1.0f-1.5f,-1.0f+1.0f, 1.0f,
		 1.0f-1.5f, 1.0f+1.0f, 1.0f,
		-1.0f-1.5f,-1.0f+1.0f, 1.0f,
		-1.0f-1.5f, 1.0f+1.0f, 1.0f,
		 1.0f-1.5f, 1.0f+1.0f, 1.0f,
		-1.0f-1.5f, 1.0f+1.0f,-1.0f, //top
		 1.0f-1.5f, 1.0f+1.0f,-1.0f,
		 1.0f-1.5f, 1.0f+1.0f, 1.0f,
		-1.0f-1.5f, 1.0f+1.0f,-1.0f,
		-1.0f-1.5f, 1.0f+1.0f, 1.0f,
		 1.0f-1.5f, 1.0f+1.0f, 1.0f,
		-1.0f-1.5f,-1.0f+1.0f,-1.0f, //back
		 1.0f-1.5f,-1.0f+1.0f,-1.0f,
		 1.0f-1.5f, 1.0f+1.0f,-1.0f,
		-1.0f-1.5f,-1.0f+1.0f,-1.0f,
		-1.0f-1.5f, 1.0f+1.0f,-1.0f,
		 1.0f-1.5f, 1.0f+1.0f,-1.0f,
		-1.0f-1.5f,-1.0f+1.0f,-1.0f, //bottom
		 1.0f-1.5f,-1.0f+1.0f,-1.0f,
		 1.0f-1.5f,-1.0f+1.0f, 1.0f,
		-1.0f-1.5f,-1.0f+1.0f,-1.0f,
		-1.0f-1.5f,-1.0f+1.0f, 1.0f,
		 1.0f-1.5f,-1.0f+1.0f, 1.0f
	};

	static const GLfloat g_vertex_buffer_data2[] = { 
		-1.0f+1.5f,-1.0f+1.0f,-1.0f, //left
		-1.0f+1.5f,-1.0f+1.0f, 1.0f,
		-1.0f+1.5f, 1.0f+1.0f, 1.0f,
		-1.0f+1.5f,-1.0f+1.0f,-1.0f,
		-1.0f+1.5f, 1.0f+1.0f,-1.0f,
		-1.0f+1.5f, 1.0f+1.0f, 1.0f,
		 1.0f+1.5f,-1.0f+1.0f,-1.0f, //right
		 1.0f+1.5f,-1.0f+1.0f, 1.0f,
		 1.0f+1.5f, 1.0f+1.0f, 1.0f,
		 1.0f+1.5f,-1.0f+1.0f,-1.0f,
		 1.0f+1.5f, 1.0f+1.0f,-1.0f,
		 1.0f+1.5f, 1.0f+1.0f, 1.0f,
		-1.0f+1.5f,-1.0f+1.0f, 1.0f, //front
		 1.0f+1.5f,-1.0f+1.0f, 1.0f,
		 1.0f+1.5f, 1.0f+1.0f, 1.0f,
		-1.0f+1.5f,-1.0f+1.0f, 1.0f,
		-1.0f+1.5f, 1.0f+1.0f, 1.0f,
		 1.0f+1.5f, 1.0f+1.0f, 1.0f,
		-1.0f+1.5f, 1.0f+1.0f,-1.0f, //top
		 1.0f+1.5f, 1.0f+1.0f,-1.0f,
		 1.0f+1.5f, 1.0f+1.0f, 1.0f,
		-1.0f+1.5f, 1.0f+1.0f,-1.0f,
		-1.0f+1.5f, 1.0f+1.0f, 1.0f,
		 1.0f+1.5f, 1.0f+1.0f, 1.0f,
		-1.0f+1.5f,-1.0f+1.0f,-1.0f, //back
		 1.0f+1.5f,-1.0f+1.0f,-1.0f,
		 1.0f+1.5f, 1.0f+1.0f,-1.0f,
		-1.0f+1.5f,-1.0f+1.0f,-1.0f,
		-1.0f+1.5f, 1.0f+1.0f,-1.0f,
		 1.0f+1.5f, 1.0f+1.0f,-1.0f,
		-1.0f+1.5f,-1.0f+1.0f,-1.0f, //bottom
		 1.0f+1.5f,-1.0f+1.0f,-1.0f,
		 1.0f+1.5f,-1.0f+1.0f, 1.0f,
		-1.0f+1.5f,-1.0f+1.0f,-1.0f,
		-1.0f+1.5f,-1.0f+1.0f, 1.0f,
		 1.0f+1.5f,-1.0f+1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data1[] = { 
		1.0f, 0.0f, 0.0f, //left
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, //right
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 0.5f, //front
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 0.5f, //top
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, //back
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.0f, 0.0f, //bottom
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.5f, 0.5f
	};

	static const GLfloat g_color_buffer_data2[] = { 
		0.0f, 0.0f, 1.0f, //left
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, //right
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 1.0f, //front
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 1.0f, //top
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, //back
		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, //bottom
		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 1.0f
	};

	GLuint vertexbuffer1;
	glGenBuffers(1, &vertexbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data1), g_vertex_buffer_data1, GL_STATIC_DRAW);

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);

	GLuint vertexbuffer3;
	glGenBuffers(1, &vertexbuffer3);

	GLuint vertexbuffer4;
	glGenBuffers(1, &vertexbuffer4);

	GLuint colorbuffer1;
	glGenBuffers(1, &colorbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data1), g_color_buffer_data1, GL_STATIC_DRAW);

	GLuint colorbuffer2;
	glGenBuffers(1, &colorbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data2), g_color_buffer_data2, GL_STATIC_DRAW);

	audio_data data = get_audio_data(argv[1]);
	int bpf = data.sampling_rate / fps;
	int data_index = 0;
	double current_time;
	double last_time;

	int res;
	pthread_t a_thread;
	void *thread_result;
	res = pthread_create(&a_thread, NULL, play_wav_d, argv[1]);
	if(res != 0) {
		perror("Thread creation failed!");
		exit(EXIT_FAILURE);
	}

	glfwSetTime(0);
	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		PV = ProjectionMatrix * ViewMatrix;
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = PV * ModelMatrix;

//		MVP = PV;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		float x[bpf];
		for(int i = 0; i < bpf; i++) x[i] = 10.0 / bpf * i - 5;

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
		glBufferData(GL_ARRAY_BUFFER, bpf * 4, x, GL_STATIC_DRAW);
		glVertexAttribPointer(
			2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			1,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glUniform1i(objectID, 1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
		glBufferData(GL_ARRAY_BUFFER, bpf * 4, (short *)data.data + data_index, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			1,                  // size
			GL_SHORT,           // type
			GL_FALSE,           // normalized?
			3,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_LINE_STRIP, 0, bpf); // 12*3 indices starting at 0 -> 12 triangles

		glUniform1i(objectID, 2);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
		glBufferData(GL_ARRAY_BUFFER, bpf * 4, (short *)data.data + data_index, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			1,                  // size
			GL_SHORT,           // type
			GL_FALSE,           // normalized?
			3,                  // stride
			(void*)2            // array buffer offset
		);
		glDrawArrays(GL_LINE_STRIP, 0, bpf); // 12*3 indices starting at 0 -> 12 triangles
		usleep(2000);

		glUniform1i(objectID, 0);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		double sum_l = 0, sum_r = 0;
		for(int i = 0; i < bpf; i++) {
			//sum_l = max(sum_l, abs(((short*)data.data)[data_index++]));
			//sum_r = max(sum_r, abs(((short*)data.data)[data_index++]));
			sum_l += abs(((short*)data.data)[data_index++]);
			sum_r += abs(((short*)data.data)[data_index++]);
		}
		Model[1][1] = sum_l / bpf / 32768 * cube_height;
		//Model[1][1] = sum_l / 32768 * cube_height;
		MVP = PV * Model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
//		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
//		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer1);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		Model[1][1] = sum_r / bpf / 32768 * cube_height;
		//Model[1][1] = sum_r / 32768 * cube_height;
		MVP = PV * Model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
//		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
//		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		if(data_index >= data.size / 2) break;
		current_time = glfwGetTime();
		double accurate_time = data_index / 2.0 / bpf / fps;
		double delta = accurate_time - current_time;
		printf("%lf %lf %lf %lf\n", accurate_time, current_time, current_time - last_time, delta);
		delta = delta > 0 ? delta : 0;
		last_time = current_time;
		//if(current_time > 0.1) usleep(delta * 1000000);
		usleep(delta * 1000000);
		//printf("%d\n", data.size);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	res = pthread_join(a_thread, &thread_result);
	if(res != 0) {
		perror("Thread join failed!");
		exit(EXIT_FAILURE);
	}

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer1);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteBuffers(1, &vertexbuffer3);
	glDeleteBuffers(1, &vertexbuffer4);
	glDeleteBuffers(1, &colorbuffer1);
	glDeleteBuffers(1, &colorbuffer2);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

