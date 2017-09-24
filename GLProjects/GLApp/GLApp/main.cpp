#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2.h>
#include "Shader.h"

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader ourShader("Shader/core.vs", "frag.frag");
	//Attachment process
	GLfloat verts[] =
	{
		//positon						//colour				//TexCood(Normalised)
		0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,				1.0f, 1.0f, //TopRight
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f,	0.0f,				1.0f, 0.0f,	//BottomRight
		-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,				0.0f, 1.0f, //TopLeft
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,				0.0f, 0.0f //BottomLeft


	};
	GLuint indice[] =
	{
		//Indexing shared vertexs
		0, 1, 3, //First Triangle
		1, 2, 3, //Second Triangle
	};



	GLuint vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	//Bind the vao first, then bind and set the buffers / attrib pointers
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);

	//Position Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Colour attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//TexCoord attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Unbind vao
	glBindVertexArray(0);

	//Textyre storage + width and height
	GLuint texture;
	int texWidth, texHeight;
	//Texture initialisation
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Setting texture parameters
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load image
	unsigned char * image = SOIL_load_image("Images/Spiral.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//Gen mipmaps (LODING)
	glGenerateMipmap(GL_TEXTURE_2D);

	//Free up memory
	SOIL_free_image_data(image);
	//Unbinding it now
	glBindTexture(GL_TEXTURE_2D, 0);

	while (!glfwWindowShouldClose(window))
	{
		//check for events/inputs
		glfwPollEvents();
		
		//handle game logic

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw
		ourShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.shaderProgram, "ourTexture"), 0);


		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	return EXIT_SUCCESS;
}