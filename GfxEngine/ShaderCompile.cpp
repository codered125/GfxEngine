#include "ShaderCompile.h"
static const GLchar * vertexShaderSource[] = {
		"#version 400 core								 \n"
		"												 \n"
		"layout (location = 0) in vec4 in_Position;		 \n"
		"layout (location = 1) in vec4 in_Colour;		 \n"
		"out vec4 ex_Colour;							 \n"
		"void main(void)								 \n"
		"{												 \n"
		"ex_Colour = in_Colour;							 \n"
		"gl_Position.xyz = vec3(in_Position[0], in_Position[1], in_Position[2]);	 \n"
		"gl_Position.w = 1.0f;							 \n"
		"}												 \n"
};

static const GLchar * fragmentShaderSource[] = {
	"#version 400 core								 \n"
	"in vec4 ex_Colour;								 \n"
	"out vec4 gl_FragColor;							 \n"
	"												 \n"
	"void main(void)								 \n"
	"{												 \n"
	"gl_FragColor = ex_Colour;						 \n"
	"												 \n"
	"}												 \n"
};


GLuint compileShader()
{
	GLenum err = glewInit();
	GLuint vertShader, fragShader, program;

	vertShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertShader);
	GLint isCompiled = 0;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)std::cout << "Failed Vert \n";

	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragShader);

	GLint fragisCompiled = 0;
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragisCompiled);
	if (fragisCompiled == GL_FALSE)std::cout << "Failed Frag \n";


	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	GLint IsLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&IsLinked);
	if (IsLinked == GL_FALSE)std::cout << "Failed Link \n";


	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	//glDeleteShader(vertShader);
	//glDeleteShader(fragShader);
	return program;
}

