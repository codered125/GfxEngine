#include <Windows.h>
#include <gl\glew\glew-2.1.0\include\GL\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <ios>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "stdafx.h"


GLuint compileShader(); 
unsigned long getFileLength(std::ifstream& file);

int loadsahder(char* filename, GLchar** ShaderSource, unsigned long * RealLength);
