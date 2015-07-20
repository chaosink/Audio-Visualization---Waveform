#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in float z;

out vec3 fragmentColor;

uniform mat4 MVP;
uniform int object;
uniform float top;
uniform float sine_z;
uniform float waveform_length;
int color_factor = 4;
float size_factor = 0.01;
float size_factor1 = 0.1;
int wave_height = 5;

bool zhuangbi = true;
#define ZHUANGBI

void main() {
	mat4 scale = mat4(
		 size_factor, 0.0, 0.0, 0.0,
		 0.0, 1.0, 0.0, 0.0,
		 0.0, 0.0, size_factor, 0.0,
		 0.0, 0.0, 0.0, 1.0);
	mat4 scale1 = mat4(
		 size_factor1, 0.0, 0.0, 0.0,
		 0.0, 1.0, 0.0, 0.0,
		 0.0, 0.0, size_factor1, 0.0,
		 0.0, 0.0, 0.0, 1.0);
	switch(object) {
		#ifdef ZHUANGBI
		case 0: //left column
			gl_Position = MVP * scale * vec4(vertexPosition_modelspace, 1);
			break;
		case 1: //right column
			gl_Position = MVP * scale * vec4(vertexPosition_modelspace, 1);
			break;
		#endif
		case 2: //left wave
			gl_Position = MVP * (vec4(z, vertexPosition_modelspace.x / 32768 * wave_height, 0, 1) + vec4(0.0, 0.0,-waveform_length / 2, 0.0));
			break;
		case 3: //right wave
			gl_Position = MVP * vec4( 5.0, vertexPosition_modelspace.x / 32768 * wave_height, z, 1);
			break;
		case 4: //left top
			gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
			break;
		case 5: //right top
			gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
			break;
		case 6: //spectrum
			gl_Position = MVP * (vec4(0, vertexPosition_modelspace.x, z, 1) + vec4(-waveform_length / 2, 0.0, 0.0, 0.0));
			break;
		case 7: //sine
			gl_Position = MVP * vec4(z, vertexPosition_modelspace.x, sine_z, 1);
			break;
		default:
			gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
			break;
	}

	switch(object) {
		case 0: //left column
			fragmentColor = zhuangbi ? vec3(1.0, gl_Position.y / color_factor, gl_Position.y / color_factor) : vertexColor;
			break;
		case 1: //right column
			fragmentColor = zhuangbi ? vec3(gl_Position.y / color_factor, gl_Position.y / color_factor, 1.0) : vertexColor;
			break;
		case 2: //left wave
			fragmentColor = zhuangbi ? vec3(vertexPosition_modelspace.x / 32768 * 0.8 + 0.2, (z + waveform_length / 2) / waveform_length, 0.5) : vec3(1.0, 0.5, 0.5);
			break;
		case 3: //right wave
			fragmentColor = zhuangbi ? vec3(0.5, (z + waveform_length / 2) / waveform_length, vertexPosition_modelspace.x / 32768 * 0.8 + 0.2) : vec3(0.5, 0.5, 1.0);
			break;
		case 4: //left top (1.0 - pow(gl_Position.w, 2) / color_factor) * 0.7 + 0.3
			fragmentColor = vec3(1.0, top / 2, top / 2);
			break;
		case 5: //right top
			fragmentColor = vec3(top / 2, top / 2, 1.0);
			break;
		case 6: //spectrum log(vertexPosition_modelspace.x + 1) / 3 * 0.8 + 0.2
			fragmentColor = vec3(abs(z) / waveform_length * 2, (z + waveform_length / 2) / waveform_length, (log(vertexPosition_modelspace.x + 1) / 3 * 0.8) * (z + waveform_length / 2) / 5 + 0.2);
			break;
		case 7: //sine
			fragmentColor = zhuangbi ? vec3(vertexPosition_modelspace.x * 0.8 + 0.2, (z + 10.0) / 20.0, 0.5) : vec3(1.0, 0.5, 0.5);
			break;
		default:
			fragmentColor = vec3(1.0, 1.0, 1.0);
			break;
	}
}
