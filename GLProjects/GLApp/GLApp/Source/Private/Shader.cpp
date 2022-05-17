#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* GeometryPath, const GLchar* ComputePath, const GLchar* TesselationPath )
{

	GLuint VertexShader;
	GLuint FragmentShader;
	GLuint ComputeShader;
	GLuint GeometryShader;
	GLuint TEvaluationShader;
	GLuint TControlShader;

	if (vertexPath)
	{
		VertexShader = MakeInternalShader(vertexPath, GL_VERTEX_SHADER, "Vertex Compile Fail");
	}

	if (fragmentPath)
	{
		FragmentShader = MakeInternalShader(fragmentPath, GL_FRAGMENT_SHADER, "Fragment Compile Fail");
	}

	if (ComputePath)
	{
		ComputeShader = MakeInternalShader(ComputePath, GL_COMPUTE_SHADER, "Compute Compile Fail");
	}
	
	if (GeometryPath)
	{
		GeometryShader = MakeInternalShader(GeometryPath, GL_GEOMETRY_SHADER, "Geometry Compile Fail");
	}

	if (TesselationPath && Tesselation)
	{ 
		auto ControlPath = std::string(TesselationPath) + ".tesc";
		TControlShader = MakeInternalShader(ComputePath, GL_TESS_CONTROL_SHADER, "Tessellation control Compile Fail");
	
		auto EvaluationPath = std::string(TesselationPath) + ".tese";
		TEvaluationShader = MakeInternalShader(ComputePath, GL_TESS_EVALUATION_SHADER, "Tessellation Evaluate Fail");	
	}

	//2. Create program and Attach Shaders
	GLint Success;
	GLchar infolog[512];
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, VertexShader);

	if (ComputePath)
	{
		glAttachShader(this->shaderProgram, ComputeShader);
	}
	
	if (GeometryPath)
	{
		glAttachShader(this->shaderProgram, GeometryShader);
	}

	if (TesselationPath && Tesselation)
	{
		glAttachShader(this->shaderProgram, TControlShader);
		glAttachShader(this->shaderProgram, TEvaluationShader);
	}
	glAttachShader(this->shaderProgram, FragmentShader);



	//Link Shaders
	glLinkProgram(this->shaderProgram);
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(this->shaderProgram, 512, NULL, infolog);
		std::cout << "program  Fail\n" << infolog << std::endl;
		std::cout << vertexPath << std::endl;

		if (ComputePath)
		{
			std::cout << ComputePath << std::endl;
		}

		if (GeometryPath)
		{
			std::cout << GeometryPath << std::endl;
		}

		if (TesselationPath && Tesselation)
		{
			std::string ControlPath = std::string(TesselationPath) + ".cs";
			auto EvaluationPath = std::string(TesselationPath) + ".es";

			std::cout << ControlPath << std::endl;
			std::cout << EvaluationPath << std::endl;
		}
		std::cout << fragmentPath << std::endl;
	}


	//3. Delete Shaders
	glDeleteShader(VertexShader);

	if (ComputePath)
	{
		glDeleteShader(ComputeShader);
	}
	
	if (GeometryPath)
	{
		glDeleteShader(GeometryShader);
	}
	
	if (TesselationPath && Tesselation)
	{
		glDeleteShader(TControlShader);
		glDeleteShader(TEvaluationShader);
	}
	glDeleteShader(FragmentShader);

}

//-------------------------------------------------------------------

std::string Shader::ParseShaderForIncludes(const GLchar* CurrentShaderPath)
{
	std::string OutputShader;
	std::ifstream CurrentShaderFile;
	CurrentShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		//Open files
		CurrentShaderFile.open(CurrentShaderPath);
		std::string CurrentLine;
		while (std::getline(CurrentShaderFile, CurrentLine))
		{
			if (CurrentLine.substr(0, 8) == "#include")
			{
				auto CurrentIncludePath = CurrentLine.substr(9);
				std::ifstream IncludedFile(CurrentIncludePath);
				if (IncludedFile.is_open())
				{
					IncludedFile.close();
					OutputShader += ParseShaderForIncludes(CurrentIncludePath.c_str());
				}
			}
			else
			{
				OutputShader += CurrentLine + "\n";
			}
		}
		CurrentShaderFile.close();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "shader include failed to read" << CurrentShaderPath << std::endl;
	}
	return OutputShader;
}

//-------------------------------------------------------------------

GLuint Shader::MakeInternalShader(const GLchar* InPath, const GLenum ShaderType, const std::string FailureMessage)
{
	//Recursively get shader source code from path
	const std::string CompleteShader = ParseShaderForIncludes(InPath);
	const GLchar* ShaderCode = CompleteShader.c_str();


	//2. Compile Shaders
	GLint Success;
	GLchar infolog[512];

	//Create vertex shader
	GLuint Shader = glCreateShader(ShaderType);
	glShaderSource(Shader, 1, &ShaderCode, NULL);
	glCompileShader(Shader);

	//Vertex Safety check
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(Shader, 512, NULL, infolog);
		std::cout << FailureMessage << "\n" << infolog << std::endl;
	}
	return Shader;
}

//-------------------------------------------------------------------

void Shader::use()
{
	glUseProgram(this->shaderProgram);
}

//-------------------------------------------------------------------

void Shader::setFloat(const std::string& Accessor, float value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, Accessor.c_str()), value);
}

//-------------------------------------------------------------------

void Shader::setVec2(const std::string& Accessor, glm::vec2 value)
{
	glUniform2fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, &value[0]);
}

//-------------------------------------------------------------------

void Shader::setVec3(const std::string& Accessor, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, &value[0]);
}

//-------------------------------------------------------------------

void Shader::setVec4(const std::string &Accessor, glm::vec4  &value)
{
	glUniform4fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, &value[0]);
}

//-------------------------------------------------------------------

void Shader::setMat4(const std::string &Accessor, const glm::mat4 &value)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, GL_FALSE, &value[0][0]);
}

//-------------------------------------------------------------------

void Shader::setBool(const std::string &Accessor, bool value)
{
	glUniform1i(glGetUniformLocation(shaderProgram, Accessor.c_str()), (int)value);
}

//-------------------------------------------------------------------

void Shader::setInt(const std::string &Accessor, int value)
{
	glUniform1i(glGetUniformLocation(shaderProgram, Accessor.c_str()), value);
}

//-------------------------------------------------------------------

void Shader::SetSampler(const std::string & Accessor, GLuint* value, GLenum TextureType)
{
	const auto SamplerLocation = glGetUniformLocation(shaderProgram, Accessor.c_str());
	if (SamplerLocation != -1)
	{
		glEnable(TextureType);
		glActiveTexture(GL_TEXTURE0 + SamplerLocation);
		glBindTexture(TextureType, *value);
		glUniform1i(glGetUniformLocation(shaderProgram, Accessor.c_str()), SamplerLocation);
	}
}

//-------------------------------------------------------------------

GLint Shader::GetUniformLocation(const std::string & Accessor)
{
	return glGetUniformLocation(shaderProgram, Accessor.c_str());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------



