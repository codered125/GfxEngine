#include "ShaderCompile.h"


GLuint compileShader()
{
	GLenum err = glewInit();


	GLuint vertShader;
	GLuint fragShader;
	GLuint program;

	static const GLchar * vertexShaderSource[] = {
		"#version 400 core								 \n"
		"												 \n"
		"layout (location = 0) in float [24]  p1;		 \n"
		"layout (location = 1) in float [24]  p2;		 \n"
		"layout (location = 2) in float [24]  p3;		 \n"
		"vec4 currentPoint;								 \n"
		"out float Ratio;								 \n"
		"												 \n"
		"void main(void)								 \n"
		"{												 \n"
		"gl_Position.xyz = vec3(p1[gl_VertexID], p2[gl_VertexID], p3[gl_VertexID]);			 \n"
		"gl_Position.z = 1.0f;							 \n"
		"Ratio = gl_VertexID;							 \n"
		"}												 \n"
	};


	static const GLchar * fragmentShaderSource[] = {
		"#version 400 core								 \n"
		"												 \n"
		"in float Ratio;								 \n"
		"vec4 FragColor;							 \n"
		"												 \n"
		"void main(void)								 \n"
		"{												 \n"
//		"FragColor = vec4(0.75f / Ratio, 0.15f / Ratio, 0.80f / Ratio, 0.60f);	 \n"
		"FragColor = vec4(0.93f, 0.25f, 1.0f, 1.0f);		 \n"
		"gl_FragColor = FragColor / Ratio;				 \n"
		"}												 \n"
	};

	
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

	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return program;
}

//unsigned long getFileLength(std::ifstream & file)
//{
//	if (!file.good()) return 0;
//
//	unsigned long pos = file.tellg();
//	file.seekg(0, ios::end);
//	
//	unsigned long len = file.tellg();
//	file.seekg(ios::beg);
//
//	return len;
//}
//
//int loadsahder(char * filename, GLchar ** ShaderSource, unsigned long * RealLength)
//{
//	ifstream file;
//	file.open(filename, ios::in);
//	if (!file) return -1;
//
//	*RealLength = getFileLength(file);
//
//	if (RealLength == 0) return -2;
//
//	//	*ShaderSource = (GLubyte*) new char[RealLength + 1];
//
//	return 0;
//}