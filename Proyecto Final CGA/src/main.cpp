//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/InputManager.h"
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"

// Texture include
#include "Headers/Texture.h"

#include "Headers\CubemapTexture.h"

// Camera include
#include "Headers/CameraFPS.h"

#include "Headers/Model.h"

// Sphere include
#include "Headers/Sphere.h"

Sphere sp(1.5, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere sp2(1.5, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);
Sphere sol(1.5, 50, 50, MODEL_MODE::VERTEX_COLOR);

Shader lightingShader;
Shader lampShader;
Shader cubemapShader;
Shader envCubeShader;
//Shader para renderizar el plano
Shader shader;

Model modelo1;

Texture textureDifuse(GL_TEXTURE_2D, "../Textures/container2.png");
Texture textureSpecular(GL_TEXTURE_2D, "../Textures/container2_specular.png");
CubemapTexture * cubeMaptexture = new CubemapTexture("../Textures/mp_pr", "pr_ft.tga", "pr_bk.tga", "pr_up.tga", "pr_dn.tga", "pr_rt.tga", "pr_lf.tga");
//Para el plano
Texture * texturaPlano;

GLuint VAO, VBO, EBO;

struct VertexTexture {
	glm::vec3 m_Pos;
	glm::vec2 m_TexCoord;
};

glm::vec3 *posPiso;

int screenWidth;
int screenHeight;

GLFWwindow * window;
InputManager inputManager;
double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable test depth, must be clear depth buffer bit
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	sp.init();
	sp.load();

	sp2.init();
	sp2.load();

	modelo1.loadModel("../objects/cyborg/cyborg.obj");

	lightingShader.initialize("../Shaders/loadModelLighting.vs", "../Shaders/loadModelLighting.fs");
	lampShader.initialize("../Shaders/lampShader.vs", "../Shaders/lampShader.fs");
	cubemapShader.initialize("../Shaders/cubemapTexture.vs", "../Shaders/cubemapTexture.fs");
	envCubeShader.initialize("../Shaders/envRefCubemapTexture.vs", "../Shaders/envRefCubemapTexture.fs");
	textureDifuse.load();
	
	//Inicializar el Shader para el plano
	shader.initialize("../Shaders/mvpTexture.vs", "../Shaders/mixTexture.fs");
	cubeMaptexture->Load();

	//Vefrtices del plano
	VertexTexture vertices[6] = {
		{ glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 1.0f) }
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Texture Mapping attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
		(GLvoid*) sizeof(vertices[0].m_Pos));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	texturaPlano = new Texture(GL_TEXTURE_2D, "../Textures/ca.jpg");
	texturaPlano->load();

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();
	lightingShader.destroy();
	lampShader.destroy();
	cubemapShader.destroy();
	envCubeShader.destroy();
	shader.destroy();
	delete cubeMaptexture;
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	inputManager.keyPressed(inputManager.toApplicationKey(key), deltaTime * 10.0f,
		inputManager.toApplicationState(action));
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	inputManager.mouseMoved(xpos, ypos);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	inputManager.mouseClicked(inputManager.toMouseButtonIndex(button), xpos,
		ypos, inputManager.toApplicationState(state));
}

bool processInput(bool continueApplication) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
		|| glfwWindowShouldClose(window) != 0) {
		return false;
	}
	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	inputManager.do_movement(deltaTime);
	glfwPollEvents();
	return continueApplication;
}

void asigVert(int A) {
	int B = 2 * A;
	posPiso = new glm::vec3[B*B];
	for (int i = 0; i < B; i++) {
		for (int j = 0; j < B; j++)
			posPiso[B*i + j] = glm::vec3( (float)(-A+i), 0.0f, (float)(-A+j));
	}
}

void applicationLoop() {
	bool psi = true;

	asigVert(15);
	glm::vec3 cubePositions[] =
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, -1.0f) };

	glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
	double lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		psi = processInput(true);
		// This is new, need clear depth buffer bit
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create camera transformations
		glm::mat4 view = inputManager.getCameraFPS()->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		/*lightingShader.turnOn();
		GLint viewPosLoc = lightingShader.getUniformLocation("viewPos");
		glUniform3f(viewPosLoc, inputManager.getCameraFPS()->Position.x, inputManager.getCameraFPS()->Position.y,
			inputManager.getCameraFPS()->Position.z);
		// Set material properties
		GLint matDiffuseLoc = lightingShader.getUniformLocation(
			"material.diffuse");
		GLint matSpecularLoc = lightingShader.getUniformLocation(
			"material.specular");
		GLint matShineLoc = lightingShader.getUniformLocation(
			"material.shininess");
		glUniform1i(matDiffuseLoc, 0);
		glUniform1i(matSpecularLoc, 1);
		glUniform1f(matShineLoc, 32.0f);
		// Set lights properties
		GLint lightAmbientLoc = lightingShader.getUniformLocation(
			"light.ambient");
		GLint lightDiffuseLoc = lightingShader.getUniformLocation(
			"light.diffuse");
		GLint lightSpecularLoc = lightingShader.getUniformLocation(
			"light.specular");
		GLint lightPosLoc = lightingShader.getUniformLocation("light.position");
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f); // Let's darken the light a bit to fit the scene
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		// Get the uniform locations
		GLint modelLoc = lightingShader.getUniformLocation("model");
		GLint viewLoc = lightingShader.getUniformLocation("view");
		GLint projLoc = lightingShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// Draw a sphere
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		textureDifuse.bind(GL_TEXTURE0);
		sp2.render();
		//modelo1.render(&lightingShader);
		lightingShader.turnOff();
		*/

		cubemapShader.turnOn();

		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;

		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

		GLint viewLoc = cubemapShader.getUniformLocation("view");
		GLint projLoc = cubemapShader.getUniformLocation("projection");
		GLint modelLoc = cubemapShader.getUniformLocation("model");

		view = glm::mat3(inputManager.getCameraFPS()->GetViewMatrix());
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glm::mat4 cubeModel;
		cubeModel = glm::scale(cubeModel, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));

		cubeMaptexture->Bind(GL_TEXTURE0);
		GLuint cubeTextureId = cubemapShader.getUniformLocation("skybox");
		glUniform1f(cubeTextureId, 0);

		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		sp2.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		cubemapShader.turnOff();

		envCubeShader.turnOn();
		
		view = inputManager.getCameraFPS()->GetViewMatrix();
		viewLoc = envCubeShader.getUniformLocation("view");
		projLoc = envCubeShader.getUniformLocation("projection");
		modelLoc = envCubeShader.getUniformLocation("model");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		cubeMaptexture->Bind(GL_TEXTURE0);
		cubeTextureId = envCubeShader.getUniformLocation("skybox");
		glUniform1f(cubeTextureId, 0);

		GLuint viewPosLoc = envCubeShader.getUniformLocation("viewPos");
		glUniform3fv(viewPosLoc,1, glm::value_ptr(inputManager.getCameraFPS()->Position));

		modelo1.render(&envCubeShader);

		envCubeShader.turnOff();

		shader.turnOn();


		// Bind Texture
		texturaPlano->bind(GL_TEXTURE0);
		glUniform1i(shader.getUniformLocation("textura1"), 0);
		//texture2->bind(GL_TEXTURE1);
		//glUniform1i(shader.getUniformLocation("textura2"), 1);

		// Create transformations
		view;
		//glm::mat4 model;
		projection;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		// Get their uniform location
		shader.getUniformLocation("model");
		modelLoc = shader.getUniformLocation("model");
		viewLoc = shader.getUniformLocation("view");
		projLoc = shader.getUniformLocation("projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//model = glm::rotate(model, (float)timeValue * (-0.1f),
		//glm::vec3(1.0f, 0.0f, 0.0f));
		glBindVertexArray(VAO);

		for (GLint i = 0; i < 900; i++) {
			glm::mat4 model;
			model = glm::translate(model, posPiso[i]);
			//model = glm::translate(model, cubePositions[i]);
			//model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		shader.turnOff();

		//Creacion del sol y su giro

		lampShader.turnOn();
		// Create transformations

		GLfloat timeValue = TimeManager::Instance().GetTime() - lastTime;
		GLfloat trasladoSen = (sin(timeValue));
		GLfloat trasladoCos = (cos(timeValue));


		GLint offset_y = lampShader.getUniformLocation("offset_y");
		glUniform1f(offset_y, 4*trasladoSen);
		GLint offset_z = lampShader.getUniformLocation("offset_z");
		glUniform1f(offset_z, 4*trasladoCos);

		modelLoc = lampShader.getUniformLocation("model");
		viewLoc = lampShader.getUniformLocation("view");
		projLoc = lampShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::translate(glm::mat4(), lightPos);
		model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		lampShader.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 600, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}

