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

#include "Headers\collision.h"

// OpenAL include
#include <AL/alut.h>

#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1

// OpenAL config
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };

ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };

ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };

ALfloat source2Pos[] = { 0.0, 0.0, -4.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };

ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];

ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;


Sphere sp(1.5, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere sp2(1.5, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);
Sphere luna(1.5, 50, 50, MODEL_MODE::VERTEX_COLOR);


//SBB para modelos
Sphere e1(1.0, 50, 50, MODEL_MODE::VERTEX_COLOR);

Shader lightingShader;
Shader lampShader;
Shader cubemapShader;
Shader envCubeShader;
Shader modShader;
Shader modShader2;
//Shader para renderizar el plano
Shader shader;

//Modelos
Model modelo1;
Model chappel;
Model edificio[10];

//Modelos
Model obj1;
Model obj2;
Model obj3;
Model obj4;
Model obj5;
Model obj6;
Model obj7;
Model obj8;
Model obj9;

Texture textureDifuse(GL_TEXTURE_2D, "../Textures/container2.png");
Texture textureSpecular(GL_TEXTURE_2D, "../Textures/container2_specular.png");
CubemapTexture * cubeMaptexture = new CubemapTexture("../Textures/mp_hexagon", "hexagon_ft.tga", "hexagon_bk.tga", "hexagon_up.tga", "hexagon_dn.tga", "hexagon_rt.tga", "hexagon_lf.tga");
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

float avanzax;
float avanzaz;
float giro;
GLfloat mov = 0.0;
bool recorrido1;
bool recorrido2;

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

	luna.init();
	luna.load();

	e1.init();
	e1.load();




	modelo1.loadModel("../objects/cyborg/cyborg.obj");
	edificio[2].loadModel("../objects/house_obj/house_obj.obj");
	edificio[1].loadModel("../objects/chapel_obj/chapel_obj.obj");
	edificio[3].loadModel("../objects/Cabaña/WoodenCabinObj.obj");
	edificio[0].loadModel("../objects/HousF/HouseF.obj");
	edificio[4].loadModel("../objects/domik/domik.3DS");
	//edificio[5].loadModel("../objects/house4/CH_building1.obj");

	//Modelos personas
	obj1.loadModel("../objects/chico/Boy stand N240414.3ds");
	obj2.loadModel("../objects/Mujer2/Moxxi.obj");
	obj3.loadModel("../objects/casa/m15/m15.3ds");
	obj4.loadModel("../objects/Mujer1/Girl walking N090814.3ds");
	obj5.loadModel("../objects/Hombre1/Man N151016.3ds");
	obj6.loadModel("../objects/gato/Cat.obj");
	obj7.loadModel("../objects/perro/Dog.obj");
	obj8.loadModel("../objects/Nubes/Cloud3.fbx");
	obj9.loadModel("../objects/casa/Tree/Tree.obj");

	modShader.initialize("../Shaders/model.vs", "../Shaders/model.fs");
	modShader2.initialize("../Shaders/model.vs", "../Shaders/model.fs");
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

	// OpenAL init
	alutInit(0, NULL);

	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);

	alGetError(); // clear any error messages

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}

	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);

	buffer[0] = alutCreateBufferFromFile("../sounds/Saint seiya_violin_2.wav");
	//buffer[0] = alutCreateBufferHelloWorld();

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}

	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 1.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 1200);

	/*alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 1.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 1.0f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);*/

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
			posPiso[B*i + j] = glm::vec3((float)(-A + i), 0.0f,
				(float)(-A + j));
	}
}

void applicationLoop() {
	bool psi = true;


	SBB sbb1 = getSBB(obj1.getMeshes());
	SBB sbb2 = getSBB(obj2.getMeshes());
	SBB sbb3 = getSBB(obj3.getMeshes());
	SBB sbb4 = getSBB(obj4.getMeshes());
	SBB sbb5 = getSBB(obj5.getMeshes());
	SBB sbb6 = getSBB(obj6.getMeshes());
	SBB sbb7 = getSBB(obj7.getMeshes());
	SBB sbb8 = getSBB(obj9.getMeshes());




	asigVert(30);

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	double lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		psi = processInput(true);
		// This is new, need clear depth buffer bit
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create camera transformations
		glm::mat4 view = inputManager.getCameraFPS()->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);


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
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(inputManager.getCameraFPS()->Position));

		//	modelo1.render(&envCubeShader);

		envCubeShader.turnOff();

		//Creacion del sol y su giro

		lampShader.turnOn();
		// Create transformations

		GLfloat timeValue = TimeManager::Instance().GetTime() - lastTime;
		GLfloat trasladoSen = (sin(0.1*timeValue));
		GLfloat trasladoCos = (cos(0.1*timeValue));


		glm::mat4 view3 = inputManager.getCameraFPS()->GetViewMatrix();
		glm::mat4 projection3 = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		/*
		GLint offset_y = lampShader.getUniformLocation("offset_y");
		glUniform1f(offset_y, 60 * trasladoSen);
		GLint offset_z = lampShader.getUniformLocation("offset_z");
		glUniform1f(offset_z, 60 * trasladoCos);*/

		GLint modelLoc3 = lampShader.getUniformLocation("model");
		GLint viewLoc3 = lampShader.getUniformLocation("view");
		GLint projLoc3 = lampShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view3));
		glUniformMatrix4fv(projLoc3, 1, GL_FALSE, glm::value_ptr(projection3));

		glm::mat4 model10;
		model10 = glm::translate(glm::mat4(), glm::vec3(lightPos.x, 30 * trasladoSen, 30 * trasladoCos));
		model10 = glm::scale(model10, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(model10));
		sp.render();
		lampShader.turnOff();

		//Creacion de la luna
		lampShader.turnOn();
		// Create transformations

		timeValue = TimeManager::Instance().GetTime() - lastTime;
		trasladoSen = (sin(0.1*timeValue));
		trasladoCos = (cos(0.1*timeValue));

		modelLoc3 = lampShader.getUniformLocation("model");
		viewLoc3 = lampShader.getUniformLocation("view");
		projLoc3 = lampShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view3));
		glUniformMatrix4fv(projLoc3, 1, GL_FALSE, glm::value_ptr(projection3));

		glm::mat4 model11;
		model11 = glm::translate(model11, glm::vec3(lightPos.x, -30 * trasladoSen, -30 * trasladoCos));
		model11 = glm::scale(model11, glm::vec3(0.2, 0.2, 0.2));
		glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(model11));
		luna.render();
		lampShader.turnOff();

		lightingShader.turnOn();
		viewPosLoc = lightingShader.getUniformLocation("viewPos");
		/*glUniform3f(viewPosLoc, inputManager.getCameraFPS()->Position.x, inputManager.getCameraFPS()->Position.y,
			inputManager.getCameraFPS()->Position.z);*/
		glUniform3f(viewPosLoc, lightPos.x, 30 * trasladoSen, 30 * trasladoCos);

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

		glm::mat4 view2 = inputManager.getCameraFPS()->GetViewMatrix();
		glm::mat4 projection2 = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc2 = lightingShader.getUniformLocation("model");
		GLint viewLoc2 = lightingShader.getUniformLocation("view");
		GLint projLoc2 = lightingShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view2));
		glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection2));

		for (GLuint i = 0; i < 5; i++) {
			glm::mat4 model2;

			if (i == 0) {
				model2 = glm::translate(model2, posPiso[500]);
				model2 = glm::scale(model2, glm::vec3(0.8f, 0.8f, 0.8f));
				

			}
			if (i == 1) {
				model2 = glm::translate(model2, posPiso[1500]);
				model2 = glm::rotate(model2, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model2 = glm::scale(model2, glm::vec3(0.009f, 0.009f, 0.009f));
			}
			if (i == 2) {
				model2 = glm::translate(model2, posPiso[2600]);
				model2 = glm::scale(model2, glm::vec3(0.01f, 0.01f, 0.01f));
			}
			if (i == 3) {
				model2 = glm::translate(model2, posPiso[100]);
				model2 = glm::rotate(model2, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));
			}
			if (i == 4) {
				model2 = glm::translate(model2, posPiso[3000]);
				model2 = glm::translate(model2, glm::vec3(0.0f, 5.8f, 0.0f));
				model2 = glm::rotate(model2, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model2 = glm::scale(model2, glm::vec3(0.005f, 0.005f, 0.005f));
			}
			glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
			edificio[i].render(&lightingShader);
		}

		for (GLuint i = 0; i < 5; i++) {
			glm::mat4 model20;

			if (i == 0) {
				model20 = glm::translate(model20, posPiso[500]);
				model20 = glm::scale(model20, glm::vec3(0.8f, 0.8f, 0.8f));


			}
			if (i == 1) {
				model20 = glm::translate(model20, glm::vec3(25.0f, 0.0f, 20.0f));
				model20 = glm::rotate(model20, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model20 = glm::scale(model20, glm::vec3(0.009f, 0.009f, 0.009f));
			}
			if (i == 2) {
				model20 = glm::translate(model20, glm::vec3(2.0f, 0.0f, 12.0f));
				model20 = glm::scale(model20, glm::vec3(0.01f, 0.01f, 0.01f));
			}
			if (i == 3) {
				model20 = glm::translate(model20, glm::vec3(-30.0f, 0.0f, 20.0f));
				model20 = glm::rotate(model20, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model20 = glm::scale(model20, glm::vec3(0.1f, 0.1f, 0.1f));
			}
			if (i == 4) {
				model20 = glm::translate(model20, glm::vec3(-18.0f, 0.0f, 22.0f));
				model20 = glm::translate(model20, glm::vec3(0.0f, 5.8f, 0.0f));
				model20 = glm::rotate(model20, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model20 = glm::scale(model20, glm::vec3(0.005f, 0.005f, 0.005f));
			}
			glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model20));
			edificio[i].render(&lightingShader);
		}

		//Modelo 
		glm::mat4 model1;
		model1 = glm::translate(model1, glm::vec3(5.0f+mov, 0.0, 5.0f));
		model1 = glm::scale(model1, glm::vec3(0.8f, 0.8f, 0.8f));
		//model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		obj1.render(&lightingShader);



		//Modelo 
		glm::mat4 model2;

		model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model2 = glm::rotate(glm::radians(inputManager.roty), glm::vec3(0.0f, 1.0f, 0.0f));
		model2 = glm::translate(model2, glm::vec3(1.0f+ inputManager.getAvanza(),0.0f,
			5.0f + inputManager.getAvanza1()));
		model2 = glm::scale(model2, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		obj2.render(&lightingShader);
		inputManager.camera_look_at = glm::vec4(glm::rotate(glm::radians(inputManager.roty),
			glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0, 0.0, inputManager.getAvanza(), 1.0f));



		//Modelo 
		glm::mat4 model3;
		model3 = glm::translate(model3, glm::vec3(-18.0f, 2.5f, -5.0f));
		model3 = glm::scale(model3, glm::vec3(0.05f, 0.05f, 0.05f));
		model3 = glm::rotate(model3, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
		obj3.render(&lightingShader);

		//Modelo 
		glm::mat4 model4;
		model4 = glm::translate(model4, glm::vec3(-5.0f, 0.0f, 0.0f+mov));
		model4 = glm::scale(model4, glm::vec3(0.03f, 0.03f, 0.03f));
		model4 = glm::rotate(model4, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
		obj4.render(&lightingShader);

		//Modelo 
		glm::mat4 model5;
		model5 = glm::translate(model5, glm::vec3(0.0f+mov, 0.0f, -3.0f));
		model5 = glm::scale(model5, glm::vec3(0.01f, 0.01f, 0.01f));
		model5 = glm::rotate(model5, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
		obj5.render(&lightingShader);

		//Modelo 
		glm::mat4 model6;
		model6 = glm::translate(model6, glm::vec3(5.0f, 0.0f, 0.0f+mov));
		model6 = glm::scale(model6, glm::vec3(0.5f, 0.5f, 0.5f));
		//model6 = glm::rotate(model6, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
		obj6.render(&lightingShader);

		//Modelo 
		glm::mat4 model7;
		model7 = glm::translate(model7, glm::vec3(10.0f+mov, 0.0f, 0.0f+mov));
		model7 = glm::scale(model7, glm::vec3(0.5f, 0.5f, 0.5f));
		//model7 = glm::rotate(model7, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));
		obj7.render(&lightingShader);

		//Modelo
		glm::mat4 model8;
		//model8 = glm::translate(model8, glm::vec3(mov, 7.50f, -4.0f));
		model8 = glm::translate(model8, glm::vec3(10*trasladoSen, 10.0f, 5*trasladoCos));
		model8 = glm::scale(model8, glm::vec3(0.05f, 0.05f, 0.05f));
		//model8 = glm::rotate(model8, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model8));
		obj8.render(&lightingShader);

		//Modelo
		glm::mat4 model15;
		//model8 = glm::translate(model8, glm::vec3(mov, 7.50f, -4.0f));
		model15 = glm::translate(model15, glm::vec3(-15.0f, 10.0f, -15.0f));
		model15 = glm::translate(model15, glm::vec3(10 * trasladoSen, 10.0f, 5 * trasladoCos));
		model15 = glm::scale(model15, glm::vec3(0.05f, 0.05f, 0.05f));
		//model8 = glm::rotate(model8, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model15));
		obj8.render(&lightingShader);

		//Modelo
		glm::mat4 model16;
		//model8 = glm::translate(model8, glm::vec3(mov, 7.50f, -4.0f));
		model16 = glm::translate(model16, glm::vec3(15.0f, 10.0f, -15.0f));
		model16 = glm::translate(model16, glm::vec3(10 * trasladoSen, 10.0f, 5 * trasladoCos));
		model16 = glm::scale(model16, glm::vec3(0.05f, 0.05f, 0.05f));
		//model8 = glm::rotate(model8, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model16));
		obj8.render(&lightingShader);

		//Modelo
		glm::mat4 model17;
		//model8 = glm::translate(model8, glm::vec3(mov, 7.50f, -4.0f));
		model17 = glm::translate(model17, glm::vec3(10.0f, 10.0f, 5.0f));
		model17 = glm::translate(model17, glm::vec3(10 * trasladoSen, 10.0f, 5 * trasladoCos));
		model17 = glm::scale(model17, glm::vec3(0.05f, 0.05f, 0.05f));
		//model8 = glm::rotate(model8, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model17));
		obj8.render(&lightingShader);

		//Modelo
		glm::mat4 model18;
		//model8 = glm::translate(model8, glm::vec3(mov, 7.50f, -4.0f));
		model18 = glm::translate(model18, glm::vec3(10.0f, 10.0f,-7.50f));
		model18 = glm::translate(model18, glm::vec3(10 * trasladoSen, 10.0f, 5 * trasladoCos));
		model18 = glm::scale(model18, glm::vec3(0.05f, 0.05f, 0.05f));
		//model8 = glm::rotate(model8, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model18));
		obj8.render(&lightingShader);

		//Modelo 
		glm::mat4 model9;
		model9 = glm::translate(model9, glm::vec3(25.0f, 0.0f, 0.0f));
		//model9 = glm::scale(model9, glm::vec3(0.9f, 0.9f, 0.9f));
		//model9 = glm::rotate(model9, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model9));
		obj9.render(&lightingShader);
		lightingShader.turnOff();


		//*******Colisiones****************

		lampShader.turnOn();
		modelLoc = lampShader.getUniformLocation("model");
		viewLoc = lampShader.getUniformLocation("view");
		projLoc = lampShader.getUniformLocation("projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		model1 = glm::translate(model1, glm::vec3(sbb1.center.x, sbb1.center.y, sbb1.center.z));
		model1 = glm::scale(model1,
			glm::vec3(sbb1.ratio, sbb1.ratio, sbb1.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		e1.render1();

		model2 = glm::translate(model2, glm::vec3(sbb2.center.x, sbb2.center.y, sbb2.center.z));
		model2 = glm::scale(model2,
			glm::vec3(sbb2.ratio, sbb2.ratio, sbb2.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		e1.render1();

		model3 = glm::translate(model3, glm::vec3(sbb3.center.x, sbb3.center.y, sbb3.center.z));
		model3 = glm::scale(model3,
			glm::vec3(sbb3.ratio, sbb3.ratio, sbb3.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
		e1.render1();

		model4 = glm::translate(model4, glm::vec3(sbb4.center.x, sbb4.center.y, sbb4.center.z));
		model4 = glm::scale(model4,
			glm::vec3(sbb4.ratio, sbb4.ratio, sbb4.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
		e1.render1();

		model5 = glm::translate(model5, glm::vec3(sbb5.center.x, sbb5.center.y, sbb5.center.z));
		model5 = glm::scale(model5,
			glm::vec3(sbb5.ratio, sbb5.ratio, sbb5.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
		e1.render1();

		model6 = glm::translate(model6, glm::vec3(sbb6.center.x, sbb6.center.y, sbb6.center.z));
		model6 = glm::scale(model6,
			glm::vec3(sbb6.ratio, sbb6.ratio, sbb6.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
		e1.render1();

		model7 = glm::translate(model7, glm::vec3(sbb7.center.x, sbb7.center.y, sbb7.center.z));
		model7 = glm::scale(model7,
			glm::vec3(sbb7.ratio, sbb7.ratio, sbb7.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));
		e1.render1();

		model9 = glm::translate(model9, glm::vec3(sbb8.center.x, sbb8.center.y, sbb8.center.z));
		model9 = glm::scale(model9,
			glm::vec3(sbb8.ratio, sbb8.ratio, sbb8.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model9));
		e1.render1();

		lampShader.turnOff();

		// For test collision sphere vs sphere
		SBB s1, s2;
		s1.center = glm::vec3(model1 * glm::vec4(0, 0, 0, 1));
		s1.ratio = sbb1.ratio * 0.2f;
		s2.center = glm::vec3(model2 * glm::vec4(0, 0, 0, 1));
		s2.ratio = sbb2.ratio * 0.8f;
		if (testSphereSphereIntersection(s1, s2)){
			avanzax = 0.0;
			avanzaz = 0.0;
			std::cout << "Model collision:" << std::endl;
		}

		lightingShader.turnOn();

		viewPosLoc = lightingShader.getUniformLocation("viewPos");
		/*glUniform3f(viewPosLoc, inputManager.getCameraFPS()->Position.x, inputManager.getCameraFPS()->Position.y,
		inputManager.getCameraFPS()->Position.z);*/
		glUniform3f(viewPosLoc, lightPos.x, -30 * trasladoSen, -30 * trasladoCos);

		// Set material properties
		matDiffuseLoc = lightingShader.getUniformLocation(
			"material.diffuse");
		matSpecularLoc = lightingShader.getUniformLocation(
			"material.specular");
		matShineLoc = lightingShader.getUniformLocation(
			"material.shininess");
		glUniform1i(matDiffuseLoc, 0);
		glUniform1i(matSpecularLoc, 1);
		glUniform1f(matShineLoc, 15.0f);

		// Set lights properties
		lightAmbientLoc = lightingShader.getUniformLocation(
			"light.ambient");
		lightDiffuseLoc = lightingShader.getUniformLocation(
			"light.diffuse");
		lightSpecularLoc = lightingShader.getUniformLocation(
			"light.specular");
		lightPosLoc = lightingShader.getUniformLocation("light.position");
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f); // Let's darken the light a bit to fit the scene
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		view2 = inputManager.getCameraFPS()->GetViewMatrix();
		projection2 = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		// Get the uniform locations
		modelLoc2 = lightingShader.getUniformLocation("model");
		viewLoc2 = lightingShader.getUniformLocation("view");
		projLoc2 = lightingShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view2));
		glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection2));

		lightingShader.turnOff();

		//Creacion del piso

		shader.turnOn();


		// Bind Texture
		texturaPlano->bind(GL_TEXTURE0);
		glUniform1i(shader.getUniformLocation("textura1"), 0);
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

		for (GLint i = 0; i < 3600; i++) {
			glm::mat4 model;
			model = glm::translate(model, posPiso[i]);
			//model = glm::translate(model, cubePositions[i]);
			//model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}


		shader.turnOff();

		//Movimiento de las nubes

		recorrido1 = true;
		if (recorrido1) {

			mov += 0.01;
			if (mov >= 20) {
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2) {
			mov -= 0.01;
			if (mov <= -15) {
				recorrido2 = false;
				recorrido1 = true;
			}

		}

		//Dibujo de los edificio

		glfwSwapBuffers(window);

		source0Pos[0] = model[3].x;
		source0Pos[1] = model[3].y;
		source0Pos[2] = model[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		listenerPos[0] = inputManager.getCameraFPS()->Position.x;
		listenerPos[1] = inputManager.getCameraFPS()->Position.y;
		listenerPos[2] = inputManager.getCameraFPS()->Position.z;
		alListenerfv(AL_POSITION, listenerPos);

		listenerOri[0] = inputManager.getCameraFPS()->Front.x;
		listenerOri[1] = inputManager.getCameraFPS()->Front.y;
		listenerOri[2] = inputManager.getCameraFPS()->Front.z;
		listenerOri[3] = inputManager.getCameraFPS()->Up.x;
		listenerOri[4] = inputManager.getCameraFPS()->Up.y;
		listenerOri[5] = inputManager.getCameraFPS()->Up.z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		if (inputManager.getKeyState()[InputCodes::u]) {
			alSourcePlay(source[0]);
		}

	}
}

int main(int argc, char ** argv) {
	init(800, 600, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}