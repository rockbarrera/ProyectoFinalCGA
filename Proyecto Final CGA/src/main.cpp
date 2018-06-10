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

Sphere sp(1.5, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere sp2(1.5, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);
Sphere luna(1.5, 50, 50, MODEL_MODE::VERTEX_COLOR);


//SBB para modelos
Sphere e1(1.0, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere e2(1.0, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);
Sphere e3(1.0, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere e4(1.0, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);
Sphere e5(1.0, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere e6(1.0, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);
Sphere e7(1.0, 50, 50, MODEL_MODE::VERTEX_COLOR);
Sphere e8(1.0, 50, 50, MODEL_MODE::VERTEX_LIGHT_TEXTURE);


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
	
	e2.init();
	e2.load();
	
	e3.init();
	e3.load();

	e4.init();
	e4.load();

	e5.init();
	e5.load();

	e6.init();
	e6.load();

	e7.init();
	e7.load();

	e8.init();
	e8.load();



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
	//	obj8.loadModel("../objects/Mujer3/Woman 1 N150408.3ds");
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


	SBB sbb1 = getSBB(obj1.getMeshes());
	SBB sbb2 = getSBB(obj2.getMeshes());
	SBB sbb3 = getSBB(obj3.getMeshes());
	SBB sbb4 = getSBB(obj4.getMeshes());
	SBB sbb5 = getSBB(obj5.getMeshes());
	SBB sbb6 = getSBB(obj6.getMeshes());
	SBB sbb7 = getSBB(obj7.getMeshes());
	SBB sbb8 = getSBB(obj9.getMeshes());




	asigVert(30);
	glm::vec3 cubePositions[] =
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, -1.0f) };

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
		glUniform3fv(viewPosLoc,1, glm::value_ptr(inputManager.getCameraFPS()->Position));

		//	modelo1.render(&envCubeShader);

		envCubeShader.turnOff();

		modShader.turnOn();

		
		glm::mat4 view2 = inputManager.getCameraFPS()->GetViewMatrix();
		glm::mat4 projection2 = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc2 = modShader.getUniformLocation("model");
		GLint viewLoc2 = modShader.getUniformLocation("view");
		GLint projLoc2 = modShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view2));
		glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection2));

		for (GLuint i = 0; i < 5; i++) {
			glm::mat4 model2;
			if (i == 0){
				model2 = glm::translate(model2, posPiso[500]);
				model2 = glm::scale(model2, glm::vec3(0.8f, 0.8f, 0.8f));
				
			}
			if (i == 1){
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
			edificio[i].render(&modShader);
		}

		//Modelo 
		glm::mat4 model1;
		model1 = glm::translate(model1, glm::vec3(5.0f,0.0, 5.0f));
		model1 = glm::scale(model1, glm::vec3(0.8f, 0.8f, 0.8f));
		//model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		obj1.render(&modShader);



		//Modelo 
		glm::mat4 model2;

		model2 = glm::rotate(model2, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model2 = glm::rotate(glm::radians(inputManager.roty), glm::vec3(0.0f, 1.0f, 0.0f));
		//model2 = glm::translate(model2, glm::vec3(5.0f, -1.75f, 0.0f + inputManager.getAvanza()));
		model2 = glm::scale(model2, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		obj2.render(&modShader);
		//inputManager.camera_look_at = glm::vec4(glm::rotate(glm::radians(inputManager.roty), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0, 0.0, inputManager.getAvanza(), 1.0f));



		//Modelo 
		glm::mat4 model3;
		model3 = glm::translate(model3, glm::vec3(-6.0f,2.5f, -5.0f));
		model3 = glm::scale(model3, glm::vec3(0.05f, 0.05f, 0.05f));
		model3 = glm::rotate(model3, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
		obj3.render(&modShader);

		//Modelo 
		glm::mat4 model4;
		model4 = glm::translate(model4, glm::vec3(-5.0f,0.0f, 0.0f));
		model4 = glm::scale(model4, glm::vec3(0.03f, 0.03f, 0.03f));
		model4 = glm::rotate(model4, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
		obj4.render(&modShader);

		//Modelo 
		glm::mat4 model5;
		model5 = glm::translate(model5, glm::vec3(0.0f, 0.0f, -3.0f));
		model5 = glm::scale(model5, glm::vec3(0.01f,0.01f,0.01f));
		model5 = glm::rotate(model5, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
		obj5.render(&modShader);

		//Modelo 
		glm::mat4 model6;
		model6 = glm::translate(model6, glm::vec3(5.0f,0.0f, 0.0f));
		model6 = glm::scale(model6, glm::vec3(0.5f, 0.5f, 0.5f));
		//model6 = glm::rotate(model6, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
		obj6.render(&modShader);

		//Modelo 
		glm::mat4 model7;
		model7 = glm::translate(model7, glm::vec3(10.0f,0.0f, 0.0f));
		model7 = glm::scale(model7, glm::vec3(0.5f, 0.5f, 0.5f));
		//model7 = glm::rotate(model7, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));
		obj7.render(&modShader);

		/*
		//Modelo 
		glm::mat4 model8;
		model8 = glm::translate(model8, glm::vec3(0.0f,0.0f, -4.0f));
		model8 = glm::scale(model8, glm::vec3(0.01f, 0.01f, 0.01f));
		//model8 = glm::rotate(model8, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model8));
		obj8.render(&modShader);
		*/

		//Modelo 
		glm::mat4 model9;
		model9 = glm::translate(model9, glm::vec3(7.5f,0.0f, 0.0f));
		//model9 = glm::scale(model9, glm::vec3(0.9f, 0.9f, 0.9f));
		//model9 = glm::rotate(model9, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model9));
		obj9.render(&modShader);
		modShader.turnOff();

	 
		/*
		//*******Colisiones****************

		lightingShader.turnOn();
		modelLoc = lightingShader.getUniformLocation("model");
		viewLoc = lightingShader.getUniformLocation("view");
		projLoc = lightingShader.getUniformLocation("projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model1 = glm::translate(model1, glm::vec3(sbb1.center.x, sbb1.center.y, sbb1.center.z));
		model1 = glm::scale(model1,
		glm::vec3(sbb1.ratio, sbb1.ratio, sbb1.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		e1.render();

		model2 = glm::translate(model2, glm::vec3(sbb2.center.x, sbb2.center.y, sbb2.center.z));
		model2 = glm::scale(model2,
		glm::vec3(sbb2.ratio, sbb2.ratio, sbb2.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		e1.render();

		model3 = glm::translate(model3, glm::vec3(sbb3.center.x, sbb3.center.y, sbb3.center.z));
		model3 = glm::scale(model3,
		glm::vec3(sbb3.ratio, sbb3.ratio, sbb3.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
		e1.render();

		model4 = glm::translate(model4, glm::vec3(sbb4.center.x, sbb4.center.y, sbb4.center.z));
		model4 = glm::scale(model4,
		glm::vec3(sbb4.ratio, sbb4.ratio, sbb4.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
		e1.render();

		model5 = glm::translate(model5, glm::vec3(sbb5.center.x, sbb5.center.y, sbb5.center.z));
		model5 = glm::scale(model5,
		glm::vec3(sbb5.ratio, sbb5.ratio, sbb5.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
		e1.render();

		model6 = glm::translate(model6, glm::vec3(sbb6.center.x, sbb6.center.y, sbb6.center.z));
		model6 = glm::scale(model6,
		glm::vec3(sbb6.ratio, sbb6.ratio, sbb6.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
		e1.render();

		model7 = glm::translate(model7, glm::vec3(sbb7.center.x, sbb7.center.y, sbb7.center.z));
		model7 = glm::scale(model7,
		glm::vec3(sbb7.ratio, sbb7.ratio, sbb7.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));
		e1.render();

		model9 = glm::translate(model9, glm::vec3(sbb8.center.x, sbb8.center.y, sbb8.center.z));
		model9 = glm::scale(model9,
		glm::vec3(sbb8.ratio, sbb8.ratio, sbb8.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model9));
		e1.render();

		lightingShader.turnOff();


		// Render the light sphere model
		lampShader.turnOn();
		// Create transformations
		modelLoc = lampShader.getUniformLocation("model");
		viewLoc = lampShader.getUniformLocation("view");
		projLoc = lampShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model10;
		model10=glm::translate(glm::mat4(), lightPos);
		model10=glm::scale(model10, glm::vec3(0.2, 0.2, 0.2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model10));
		e1.render();
		lampShader.turnOff();


		// For test collision sphere vs sphere
		SBB s1, s2;
		s1.center = glm::vec3(model1 * glm::vec4(0, 0, 0, 1));
		s1.ratio = sbb1.ratio * 0.2f;
		s2.center = glm::vec3(model2 * glm::vec4(0, 0, 0, 1));
		s2.ratio = sbb2.ratio * 0.8f;
		if (testSphereSphereIntersection(s1, s2))
		std::cout << "Model collision:" << std::endl;

		*/


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

		//Dibujo de los edificios
		


		//Creacion del sol y su giro

		lampShader.turnOn();
		// Create transformations

		GLfloat timeValue = TimeManager::Instance().GetTime() - lastTime;
		GLfloat trasladoSen = (sin(0.1*timeValue));
		GLfloat trasladoCos = (cos(0.1*timeValue));


		GLint offset_y = lampShader.getUniformLocation("offset_y");
		glUniform1f(offset_y, 60*trasladoSen);
		GLint offset_z = lampShader.getUniformLocation("offset_z");
		glUniform1f(offset_z, 60*trasladoCos);

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

		//Creacion de la luna
		lampShader.turnOn();
		// Create transformations

		timeValue = TimeManager::Instance().GetTime() - lastTime;
		trasladoSen = (sin(0.1*timeValue));
		trasladoCos = (cos(0.1*timeValue));


		offset_y = lampShader.getUniformLocation("offset_y");
		glUniform1f(offset_y, -60 * trasladoSen);
		offset_z = lampShader.getUniformLocation("offset_z");
		glUniform1f(offset_z, -60 * trasladoCos);

		modelLoc = lampShader.getUniformLocation("model");
		viewLoc = lampShader.getUniformLocation("view");
		projLoc = lampShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::translate(glm::mat4(), lightPos);
		model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		luna.render();
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

