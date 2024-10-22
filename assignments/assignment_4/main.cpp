#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/external/stb_image.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "JonFolder/shader.h"
#include "JonFolder/texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


//functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//time variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//mouse variables
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
//other variabels
bool cameraSpeedBoost = false;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15)),
	glm::vec3(ew::RandomRange(-15,15), ew::RandomRange(-15, 15), ew::RandomRange(-15, 15))
};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//square ver
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	

	//vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glDisable(GL_DEPTH_TEST);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	JonFolder::Shader fgShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	JonFolder::Texture fgTexture("assets/beeeee.jpg", GL_LINEAR, GL_CLAMP_TO_BORDER);

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);


	float lastX = 400, lastY = 300;

	float yaw = -90.0f;
	float pitch = 0.0f;


	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input for player
		processInput(window);

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		//Drawing happens here!
		fgShader.use();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
	fgShader.setMat4("projection", projection);

		//camera controls
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		fgShader.setMat4("view", view);

		//actual drawing of cubes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 20; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			fgShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = static_cast<float>(5 * deltaTime) * (cameraSpeedBoost?2:1);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeedBoost = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		cameraSpeedBoost = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, glm::cross(cameraFront, cameraUp))) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, glm::cross(cameraFront, cameraUp))) * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}