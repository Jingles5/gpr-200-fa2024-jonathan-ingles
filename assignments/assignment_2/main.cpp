#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

float vertices[] = {
	// positions          // colors           // texture coords
	 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};
//texture stuff
float texCoords[] = {
	0.0f, 0.0f,  // lower-left corner  
	1.0f, 0.0f,  // lower-right corner
	0.5f, 1.0f   // top-center corner
};

const char* bgVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

out vec4 Color; //Varying
uniform float uTime;
void main()
{
	gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord * vec2(20, 20) + vec2(uTime,sin(uTime)); 
}
)";

const char* fgVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

out vec4 Color; //Varying
uniform float uTime;
void main()
{
	gl_Position = vec4(aPos * abs(sin(uTime)), 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
/*
	Color = aColor; //pass through
	vec3 pos = aPos;
	pos.y += (sin(2 * uTime - pos.x))/2;
	pos.x += (cos(2 * uTime - pos.x))/2;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);*/
}
)";

const char* bgFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;


uniform sampler2D texture1;
uniform sampler2D texture2;

in vec4 Color;
uniform float uTime = 1;
uniform vec4 uColor;
uniform sampler2D ourTexture;
void main()
{
	vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.1)
        discard;
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); 
}

)"; const char* fgFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;


uniform sampler2D texture1;
uniform sampler2D texture2;

in vec4 Color;
uniform float uTime = 1;
uniform vec4 uColor;
uniform sampler2D ourTexture;
void main()
{
	//FragColor = Color*sin(uTime);
	//FragColor = texture(ourTexture, TexCoord);
	//FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);  
	vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}
)";

//functions
unsigned int loadTexture2D(const char* filePath, int filterMode, int wrapMode) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

	int width, height, nrChannels; 
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	return textureID;
}

unsigned int compileShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//vertex yay

	//compile erroesint  
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	}

	//now the same for fragments
	//fragment
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	};//just incase for linking error

	//freeing data
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);//deleting the shaders as we no longer need them

	return shaderProgram;
}


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
	//square ver
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glDisable(GL_DEPTH_TEST);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);

	//texturesquare
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//blend start
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	unsigned int bgShaderProgram = compileShader(bgVertexShaderSource, bgFragmentShaderSource);
	unsigned int fgShaderProgram = compileShader(fgVertexShaderSource, fgFragmentShaderSource);

	
	//texture code overall
	unsigned int bgtexture = loadTexture2D("assets/container.jpg", GL_LINEAR, GL_MIRRORED_REPEAT);
	unsigned int fgtexture = loadTexture2D("assets/notlilguybutlil.png", GL_NEAREST, GL_REPEAT);

	stbi_set_flip_vertically_on_load(true);
	unsigned int texture2 = loadTexture2D("assets/yippee.png", GL_LINEAR, GL_REPEAT);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();

		//bg
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Drawing happens here!
		glUseProgram(bgShaderProgram);//using this shader specifically
		int timeLoc = glGetUniformLocation(bgShaderProgram, "uTime");
		glUniform1f(timeLoc, time);
		glUniform1i(glGetUniformLocation(bgShaderProgram, "texture1"), 0);
		glUniform1i(glGetUniformLocation(bgShaderProgram, "texture2"), 1);
		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, bgtexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//the two commented below were for triangles, we like squares now!
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//end of bg
		
		//lilguy
		
		//Drawing happens here!
		glUseProgram(fgShaderProgram);//using this shader specifically
		int timeLoc2 = glGetUniformLocation(fgShaderProgram, "uTime");
		glUniform1f(timeLoc2, time);
		glUniform1i(glGetUniformLocation(fgShaderProgram, "texture2"), 1);

		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, fgtexture);
		//the two commented below were for triangles, we like squares now!
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//end of lilguy
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
