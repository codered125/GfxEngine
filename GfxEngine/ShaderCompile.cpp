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

GLuint vertShader, fragShader, program;
GLuint VAO[2];
GLuint vertex_buffer[2];
void compileShader(Cube input)
{
	GLenum err = glewInit();

	

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




	glGenVertexArrays(2, VAO);
	glBindVertexArray(VAO[0]);

	//Initialise our first buffer object
	//Theres 96 flots in holder embed that we want to pass
	//4 is the ammount of floats per vertice we want to process
	// Enable attribute index 0 as being used 
	glGenBuffers(2, vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 96, input.Points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//glGenVertexArrays(1, &VAO[1]);
	//glBindVertexArray(VAO[1]);

	//Initialise our first buffer object
	//Theres 96 flots in holder embed that we want to pass
	//4 is the ammount of floats per vertice we want to process
	// Enable attribute index 0 as being used 
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 96, input.Colours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	


	glBindAttribLocation(program, 0, "in_Position");
	glBindAttribLocation(program, 1, "in_Colour");
	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	glLinkProgram(program);
	glUseProgram(program);
	glDrawArrays(GL_QUADS, 0, 24);

	GLint IsLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&IsLinked);
	if (IsLinked == GL_FALSE)std::cout << "Failed Link \n";
	//return program;
}

