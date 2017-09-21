#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

const GLint width = 600, height = 800;

const GLchar  * VertShaderSourceCode =
"#version 330 core \n"
"layout (location = 0) in vec3 position; \n"
"void main() \n"
"{\n"
"gl_Position = vec4 (position.x , position.y, position.z, 1.0f);"
"}"
"\n";

const GLchar  * FragmentShaderSourceCode =
"#version 330 core \n"
"out vec4 color;\n"
"void main () \n"
"{"
"color = vec4 (1.0f, 0.5f, 0.2f, 1.0f);  \n"
"}";

int main()
{
	glfwInit();

	//Setting the version of our glfw window. Different versions have things like immediate mode removed ( 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	//Doesnt care for backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Cares for forwards
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Toggles Resizeability
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	//Creating window
	
	GLFWwindow *  window = glfwCreateWindow(width, height, "Moses App", nullptr, nullptr);

	//Going to get the real size and height of the screen and store it to our storage
	//helps with high density displays. Will use it when creating viewport
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr) 
	{
		//Safety check
		std::cout << "Failed to create Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	//Makes current windows
	glfwMakeContextCurrent(window);

	//Tells glew to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	//inline initiation and safety check
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initilaise Glew" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &VertShaderSourceCode, NULL);
	glCompileShader(vertShader);


	GLint Success;
	GLchar inforlog[512];

	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &Success);
	if(!Success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, inforlog);
		std::cout << "VertCompile Fail\n" << inforlog << std::endl;
	}

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &FragmentShaderSourceCode, NULL);
	glCompileShader(fragShader);


	

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, inforlog);
		std::cout << "frag  Fail\n" << inforlog << std::endl;
	}

	while (!glfwWindowShouldClose(window))
	{
		//check for events/inputs
		glfwPollEvents();
		
		//handle game logic

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw
		glfwSwapBuffers(window);

	}
	return EXIT_SUCCESS;
}