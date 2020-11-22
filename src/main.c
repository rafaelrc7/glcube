#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

#define SCR_WID 800
#define SCR_HGT 600

#define VBO_QNT 1
#define VAO_QNT 1
#define EBO_QNT 1

#define _DEBUG

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(	GLFWwindow *window, int key, int scancode, int action,
					int mods);
void GLAPIENTRY GLErrorCallback(GLenum source, GLenum type, GLuint id,
								GLenum severity, GLsizei length,
								const GLchar *message, const void *userParam);
void pollInput(GLFWwindow *window, void (**keys) (GLFWwindow *win, int key));
void rotateCube(GLFWwindow *window, int key);
void scaleCube(GLFWwindow *window, int key);
void setPolygonMode(GLFWwindow *window, int key);
void toggleProjection(GLFWwindow *window, int key);

// Global Variables
float deltaTime = 0.0f, lastFrame = 0.0f;
float xRotation = 35.0f, yRotation = 45.0f, zRotation = 0.0f;
float cube_scale = 1.0f;
char projectionType = 0;

int main(int argc, char *argv[])
{
	int width, height;
	float currentFrame, ar;
	unsigned int shaderProgram, u_model, u_view, u_projection;
	unsigned int vertexBuffers[VBO_QNT], vertexArrays[VAO_QNT],
				indexBuffers[EBO_QNT];
	GLFWwindow *window;
	GLenum err;
	mat4 model, view, projection;

	// Cube vertices and indices
	float vertices[] = {
		// Positions			// Colours
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 1.0f,

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};


	// Program keybindings
	void (*keys[512]) (GLFWwindow *win, int key) = {0};

	keys[GLFW_KEY_ESCAPE] = glfwSetWindowShouldClose;
	keys[GLFW_KEY_W] = rotateCube;
	keys[GLFW_KEY_A] = rotateCube;
	keys[GLFW_KEY_S] = rotateCube;
	keys[GLFW_KEY_D] = rotateCube;
	keys[GLFW_KEY_Q] = rotateCube;
	keys[GLFW_KEY_E] = rotateCube;
	keys[GLFW_KEY_R] = scaleCube;
	keys[GLFW_KEY_F] = scaleCube;
	keys[GLFW_KEY_1] = setPolygonMode;
	keys[GLFW_KEY_2] = setPolygonMode;
	keys[GLFW_KEY_3] = setPolygonMode;
	keys[GLFW_KEY_TAB] = toggleProjection;


	// GLFW and GLEW init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WID, SCR_HGT, "Shader Demo", NULL, NULL);
		if (!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if( (err=glewInit()) != GLEW_OK) {
		fprintf(stderr, "Failed to initialise GLEW:\n%s\n",
				glewGetErrorString(err));
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLErrorCallback, 0);
#endif

	// Shader compiling and linkage

	shaderProgram = createShader("./res/shader/vertex.shader",
								"./res/shader/fragment.shader");

	u_model = glGetUniformLocation(shaderProgram, "u_model");
	u_view = glGetUniformLocation(shaderProgram, "u_view");
	u_projection = glGetUniformLocation(shaderProgram, "u_projection");


	// Create and set Buffers

	glGenVertexArrays(VAO_QNT, vertexArrays);
	glBindVertexArray(vertexArrays[0]);

	glGenBuffers(VBO_QNT, vertexBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(	1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
							(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(EBO_QNT, indexBuffers);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[0]);

	glBufferData(	GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
					GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		pollInput(window, keys);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vertexArrays[0]);

		glUseProgram(shaderProgram);

		glm_mat4_identity(model);
		glm_mat4_identity(view);
		glm_mat4_identity(projection);

		// Model Matrix
		glm_scale_uni(model, cube_scale);
		glm_rotate(model, glm_rad(xRotation), (vec3){1.0f, 0.0f, 0.0f});
		glm_rotate(model, glm_rad(yRotation), (vec3){0.0f, 1.0f, 0.0f});
		glm_rotate(model, glm_rad(zRotation), (vec3){0.0f, 0.0f, 1.0f});

		// View Matrix
		glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

		// Aspect Ratio Calculation
		glfwGetWindowSize(window, &width, &height);
		ar = (float)width/(float)height;

		// Projection Matrix
		if (!projectionType)
			glm_perspective(glm_rad(45.0f), ar, 0.1f, 100.0f, projection);
		else {
			if (ar > 1)
				glm_ortho(	-1.0f*ar, 1.0f*ar, -1.0f, 1.0f, 0.1f, 100.0f,
							projection );
			else
				glm_ortho(	-1.0f, 1.0f, -1.0f/ar, 1.0f/ar, 0.1f, 100.0f,
							projection );
		}

		glUniformMatrix4fv(u_model, 1, GL_FALSE, (float *)model);
		glUniformMatrix4fv(u_view, 1, GL_FALSE, (float *)view);
		glUniformMatrix4fv(u_projection, 1, GL_FALSE, (float *)projection);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);

	glDeleteBuffers(VBO_QNT, vertexBuffers);
	glDeleteBuffers(VAO_QNT, vertexArrays);
	glDeleteBuffers(EBO_QNT, indexBuffers);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void pollInput(GLFWwindow *window, void (**keys) (GLFWwindow *win, int key))
{
	int i = 0;

	for (i = 0; i < 512; i++) {
		if (!keys[i])
			continue;

		if (glfwGetKey(window, i))
			(*keys[i]) (window, i);
	}
}

void rotateCube(GLFWwindow *win, int key)
{
	float step = 100.0f*deltaTime;
	switch (key) {
		case GLFW_KEY_D:
			if ( (yRotation+=step) > 360.0f )
				yRotation = 0.0f;
			break;
		case GLFW_KEY_A:
			if ( (yRotation-=step) < 0.0f )
				yRotation = 360.0f;
			break;
		case GLFW_KEY_W:
			if ( (xRotation-=step) < 0.0f )
				xRotation = 360.0f;
			break;
		case GLFW_KEY_S:
			if ( (xRotation+=step) < 0.0f )
				xRotation = 360.0f;
			break;
		case GLFW_KEY_Q:
			if ( (zRotation-=step) < 0.0f )
				zRotation = 360.0f;
			break;
		case GLFW_KEY_E:
			if ( (zRotation+=step) < 0.0f )
				zRotation = 360.0f;
			break;
	}
}

void scaleCube(GLFWwindow *win, int key)
{
	float step = 0.7f*deltaTime;

	switch (key) {
		case GLFW_KEY_R:
			if ( (cube_scale+=step) < 0 )
				cube_scale = 0;
			break;
		case GLFW_KEY_F:
			if ( (cube_scale-=step) < 0 )
				cube_scale = 0;
			break;
	}

}

void setPolygonMode(GLFWwindow *win, int key)
{
	switch (key) {
		case GLFW_KEY_1:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case GLFW_KEY_2:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case GLFW_KEY_3:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
	}
}

void toggleProjection(GLFWwindow *window, int key)
{
	if (key == GLFW_KEY_TAB && glfwGetKey(window, key) == GLFW_PRESS) {
		projectionType = !projectionType;
		while(glfwGetKey(window, key) == GLFW_PRESS)
			glfwPollEvents();
	}
}

void GLAPIENTRY GLErrorCallback(GLenum source, GLenum type, GLuint id,
								GLenum severity, GLsizei length,
								const GLchar *message, const void *userParam)
{
	fprintf(stderr, "%s CODE: 0x%x SEVERITY: 0x%x\n\tMESSAGE: %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "[**ERROR** GL]:" : "[Warning Gl]:"),
			type, severity, message);
}
