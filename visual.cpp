#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <GL/glew.h>
#include <glfw3.h>
GLFWwindow* window;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

//#include <common/load_shaders.hpp>
#include <common/shader.hpp>
#include <common/controls.hpp>
#include "audio_data.hpp"
#include "play_wav.hpp"

#define max(a, b) ((a) > (b) ? (a) : (b))

const int fps = 30;
const int cube_height = 4;
const int waveform_interval = 1;
const float waveform_length = 20.0;
const float top_height = 0.02;
const float top_speed = 0.02;

void *play_wav_d(void *file) {
	play_wav((char *)file);
	return NULL;
}

int main(int argc, char **argv) {
	if(argc != 2) exit(0);

	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1920, 1080, "Audio Visualization", glfwGetPrimaryMonitor(), NULL);
	if(window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; //Needed for core profile
	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR_DISABLED, GL_TRUE);



//	glClearColor(0.f / 255.f, 63.f / 255.f, 0.f / 255.f, 1.0f);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

/*	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "VertexShader.vert"},
		{GL_FRAGMENT_SHADER, "FragmentShader.frag"},
		{GL_GEOMETRY_SHADER, "GeometryShader.geom"},
		{GL_NONE, NULL}};
	GLuint programID = LoadShaders(shaders);*/
	GLuint programID = LoadShaders("VertexShader.vert", "FragmentShader.frag", NULL);//"GeometryShader.geom");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint objectID = glGetUniformLocation(programID, "object");

	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 View       = glm::lookAt(
		glm::vec3(10,15,20),	//Camera is at (4,3,-3), in World Space
		glm::vec3(0,0,0),		//and looks at the origin
		glm::vec3(0,1,0)		//Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 Model      = glm::mat4(1.0f);
	glm::mat4 PV         = Projection * View;
	glm::mat4 MVP        = PV * Model;



	static const GLfloat g_vertex_buffer_data1[] = { 
		-1.0f,-1.0f,-1.0f, //left
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f, //right
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f, //front
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, //top
		 1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f, //back
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f, //bottom
		 1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	static const GLfloat g_vertex_buffer_data2[] = { 
		-1.0f,-1.0f,-1.0f, //left
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f, //right
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f, //front
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, //top
		 1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f, //back
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f, //bottom
		 1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	static const GLfloat g_color_buffer_data1[] = { 
		1.0f, 0.2f, 0.2f, //left
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.2f, 0.2f,
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.2f, 0.2f, //right
		1.0f, 0.5f, 0.5f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.2f, 0.2f,
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
		1.0f, 0.2f, 0.2f, //back
		1.0f, 0.2f, 0.2f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.2f, 0.2f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.2f, 0.2f, //bottom
		1.0f, 0.2f, 0.2f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.2f, 0.2f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.5f, 0.5f
	};

	static const GLfloat g_color_buffer_data2[] = { 
		0.2f, 0.2f, 1.0f, //left
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.2f, 0.2f, 1.0f,
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.2f, 0.2f, 1.0f, //right
		0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.2f, 0.2f, 1.0f,
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
		0.2f, 0.2f, 1.0f, //back
		0.2f, 0.2f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.2f, 0.2f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.2f, 0.2f, 1.0f, //bottom
		0.2f, 0.2f, 1.0f,
		0.5f, 0.5f, 1.0f,
		0.2f, 0.2f, 1.0f,
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
	double total_time = data.size / data.sampling_rate / 4;

	int res;
	pthread_t a_thread;
	void *thread_result;
	res = pthread_create(&a_thread, NULL, play_wav_d, argv[1]);
	if(res != 0) {
		perror("Thread creation failed!");
		exit(EXIT_FAILURE);
	}



	float max_l = 0, max_r = 0;
	glfwSetTime(0);
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		computeMatricesFromInputs();
		glm::mat4 Projection  = getProjectionMatrix();
		glm::mat4 View        = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		PV                    = Projection * View;
		MVP                   = PV * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);



		float z[bpf];
		for(int i = 0; i < bpf; i++) z[i] = waveform_length / bpf * i - waveform_length / 2;
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
		glBufferData(GL_ARRAY_BUFFER, bpf * 4, z, GL_STATIC_DRAW);
		glVertexAttribPointer(
			2,						//attribute. No particular reason for 0, but must match the layout in the shader.
			1,						//size
			GL_FLOAT,				//type
			GL_FALSE,				//normalized?
			waveform_interval * 4,	//stride
			(void *)0				//array buffer offset
		);



		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
		glBufferData(GL_ARRAY_BUFFER, bpf * 4, (short *)data.data + data_index, GL_DYNAMIC_DRAW);

		glUniform1i(objectID, 1);
		glVertexAttribPointer(
			0,						//attribute. No particular reason for 0, but must match the layout in the shader.
			1,						//size
			GL_SHORT,				//type
			GL_FALSE,				//normalized?
			waveform_interval * 2,	//stride
			(void *)0				//array buffer offset
		);
		glDrawArrays(GL_LINE_STRIP, 0, bpf / waveform_interval);

		glUniform1i(objectID, 2);
		glVertexAttribPointer(
			0,						//attribute. No particular reason for 0, but must match the layout in the shader.
			1,						//size
			GL_SHORT,				//type
			GL_FALSE,				//normalized?
			waveform_interval * 2,	//stride
			(void *)2				//array buffer offset
		);
		glDrawArrays(GL_LINE_STRIP, 0, bpf / waveform_interval);


		
		float sum_l = 0, sum_r = 0;
		for(int i = 0; i < bpf; i++) {
			//sum_l = max(sum_l, abs(((short*)data.data)[data_index++])); //sum
			//sum_r = max(sum_r, abs(((short*)data.data)[data_index++]));
			sum_l += abs(((short*)data.data)[data_index++]); //avg
			sum_r += abs(((short*)data.data)[data_index++]);
		}
		sum_l /= bpf; //avg
		sum_r /= bpf;



		float scale_l = sum_l / 32768 * cube_height;
		glUniform1i(objectID, 0);
		Model = glm::mat4(
			 1.0, 0.0, 0.0, 0.0,
			 0.0, 1.0, 0.0, 0.0,
			 0.0, 0.0, 1.0, 0.0,
			-2.0, 1.0, 0.0, 1.0);
		glm::mat4 scale1 = glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, scale_l, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
		MVP = PV * scale1 * Model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
		glVertexAttribPointer(
			0,			// attribute. No particular reason for 0, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void *)0	// array buffer offset
		);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer1);
		glVertexAttribPointer(
			1,			// attribute. No particular reason for 1, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void *)0	// array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 12*3);
		if(scale_l + top_height > max_l) max_l = scale_l + top_height;
		else max_l -= top_speed;


		glUniform1i(objectID, 3);
		glm::mat4 translate1 = glm::mat4(
			 1.0, 0.0, 0.0, 0.0,
			 0.0, 1.0, 0.0, 0.0,
			 0.0, 0.0, 1.0, 0.0,
			-2.0, max_l * 2, 0.0, 1.0);
		scale1 = glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, top_height, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
		MVP = PV * translate1 * scale1;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 12*3);



		float scale_r = sum_r / 32768 * cube_height;
		glUniform1i(objectID, 5);
		Model = glm::mat4(
			 1.0, 0.0, 0.0, 0.0,
			 0.0, 1.0, 0.0, 0.0,
			 0.0, 0.0, 1.0, 0.0,
			 2.0, 1.0, 0.0, 1.0);
		glm::mat4 scale2 = glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, scale_r, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
		MVP = PV * scale2 * Model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,			// attribute. No particular reason for 0, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
		);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glVertexAttribPointer(
			1,			// attribute. No particular reason for 1, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 12*3);
		if(scale_r + top_height > max_r) max_r = scale_r + top_height;
		else max_r -= top_speed;

		glUniform1i(objectID, 4);
		glm::mat4 translate2 = glm::mat4(
			 1.0, 0.0, 0.0, 0.0,
			 0.0, 1.0, 0.0, 0.0,
			 0.0, 0.0, 1.0, 0.0,
			 2.0, max_r * 2, 0.0, 1.0);
		scale2 = glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, top_height, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
		MVP = PV * translate2 * scale2;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 12*3);



		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		if(data_index >= data.size / 2) break;
		current_time = glfwGetTime();
		double accurate_time = data_index / 2.0 / bpf / fps;
		double delta = accurate_time - current_time;
		printf("%lf %lf %lf %lf %lf\n", current_time - last_time, accurate_time, current_time, delta, total_time - current_time);
		delta = delta > 0 ? delta : 0;
		last_time = current_time;
		usleep(delta * 1000000);
		glfwPollEvents();
	} while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

	pthread_cancel(a_thread);
	res = pthread_join(a_thread, &thread_result);
	if(res != 0) {
		perror("Thread join failed!");
		exit(EXIT_FAILURE);
	}

	glDeleteBuffers(1, &vertexbuffer1);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteBuffers(1, &vertexbuffer3);
	glDeleteBuffers(1, &vertexbuffer4);
	glDeleteBuffers(1, &colorbuffer1);
	glDeleteBuffers(1, &colorbuffer2);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();

	return 0;
}
