#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

const GLint width = 600, height = 800;



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
		std::cout << "Failed to initializes Glew" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	
	//Attachment process
	GLfloat verts[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0, 0.0f,//bottom left
		0.5f, -0.5f, 0.0f, //Bottom Right
		0.0f, 0.5f, 0.0f//Top middle
	};

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		//check for events/inputs
		glfwPollEvents();
		
		//handle game logic

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	return EXIT_SUCCESS;
}