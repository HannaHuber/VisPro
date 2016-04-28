#pragma once

// Standard
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

// OpenGL extensions
#include "glew.h"
#include "glfw3.h"
#include <GL/gl.h>
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"
#include <memory>

#include "UserInput.h"
#include "Texture.h"
#include "Geometry.h"
#include "SceneObject.h"
#include "PointLight.h"
#include "Light.h"
#include "../Scene/Camera.h"
#include "../Shader/Shader.h"
#include "../Shader/BlinnPhongShader.h"
#include "../Shader/NormalShader.h"
#include "../Shader/TextureShader.h"
#include "SceneImporter.h"
#include "SceneObjectManager.h"
#include "CutawaySurface.h"
#include "Quad.h"

using namespace std;
using namespace glm;

void initScreenParameters();
void init(GLFWwindow* window);
void update(GLFWwindow* window,float deltaTime);
void createDepthImage();
void calculateCutawaySurface();
void ShadowMapShadingPass();
void draw(); 
void cleanup();

// Default screen parameters
int width = 1024;
int height = 800;
int refreshrate = 60;
bool fullscreen = false;

// Geometries
SceneObjectManager obj_manager;
vector<shared_ptr<Energy>> e_items;
vector<shared_ptr<Environment>> environment;

// Camera
Camera* camera;
float speed = 4.4f;
float look_speed = 0.001f;
float nearPlane = 0.1;
float farPlane = 180.0;
float ratio = width / height;
float fov = glm::radians(70.0f);

// Distance transform
CutawaySurface cutaway;
Quad quad();

// Control
UserInput user_input;

// Number of culled faces
int drawnFaces = 0;



int main(int argc, char** argv) {	

	// Use input parameters
	initScreenParameters();
	
	// Init GLFW
	if (!glfwInit()) {												// Init GLFW
		std::cout << "Failed to initialize GLFW\n" << std::endl;
		std::system("PAUSE");
		std::exit(-1);
	}
	
	// GLFW settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);					// OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Core profile
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);						// Resizable window	
	
	// Get monitor
	GLFWmonitor* monitor = nullptr;
	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
		glfwWindowHint(GLFW_REFRESH_RATE, refreshrate);               
	}

	// Open window
	GLFWwindow* window = glfwCreateWindow(width, height, "ExtremelyCrazyTuStress", monitor, NULL); 
	if (window == NULL){
		std::cout << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		std::exit(-1);
	}

	// Init shader, scene objects and pipeline matrices
	init(window);

	// Init frametime
	float startTime = (float)glfwGetTime();
	float lastTime = (float)glfwGetTime();
	float deltaTime = 0;
	float time_used = 0;
	int frameCount = 0;

	// Init numbers for calculating fps
	int frames = 0;
	double lastReset = 0;
	double passedTimeSinceLastReset;
	double interval = 1; 
	int fps = 0;

	// Render loop running condition
	bool isRunning = true;
	std::cout << std::endl << "Renderloop is starting..." << std::endl;
	// ---------------------------------------------------------------------------------
	// --- RENDER LOOP ---
	// ---------------------------------------------------------------------------------
	while(isRunning)  { 

		// Calculate frametime
		deltaTime = (float)glfwGetTime()-lastTime;
		lastTime = (float)glfwGetTime();
	
		frameCount++;
		frames++;
		passedTimeSinceLastReset = glfwGetTime() - lastReset;
		if (passedTimeSinceLastReset >= interval){
			fps = frames / passedTimeSinceLastReset;
			lastReset = glfwGetTime();
			frames = 0;
		}

		// Update
		update(window, deltaTime);

		// Init depth image
		createDepthImage();

		// Compute cutaway surface
		calculateCutawaySurface();

		// Draw 
		if (useWireFrame){											// Filled polygons/wireframe 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (useTransparency) {										// Alpha blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		ShadowMapShadingPass();
		draw();			// Currently does nothing
		
		// Disable wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 

		// Swap buffers
		glfwSwapBuffers(window);

		// Get window events
		glfwPollEvents();

		// Check for modi switches
		user_input.switchModi();
		
		// Check if the ESC key was pressed
		isRunning = (!user_input.terminate());
		
	} // -------------------------------------------------------------------------------------Render-Loop end

	// Framerate info
	std::cout << endl;
	std::cout << (frameCount / (glfwGetTime() - startTime)) << " fps (" << frameCount << " frames, " << (glfwGetTime() - startTime) << "s)" << std::endl;
	
	_sleep(4000);

	// Cleanup
	cleanup();

	// End GLFW
	glfwTerminate();

	std::system("PAUSE");

	std::exit(0);
}

void init(GLFWwindow* window) {

	// Create OpenGL context
	glfwMakeContextCurrent(window);

	// Window title
	glfwSetWindowTitle(window, "Visualisierung 2");
	// Get actual window size
	glfwGetWindowSize(window, &width, &height);
	ratio = float(width) / height;

	// Init GLEW
	glewExperimental = true;										// Init GLEW
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		std::exit(-1);
	}
	if (glewIsSupported("GL_VERSION_4_0")) {						// Check version support
		std::cout << "OpenGL version 4.0 supported" << endl;
	}

	// Init user input class
	user_input.init(window, false, true, true, 2, true, true, false, true);

	// Init opengl parameters	
	glClearColor(0.0, 0.0, 0.2, 0.5);						// Set backgroundcolor
	glViewport(0, 0, width, height);						// Viewport		

	if (useTransparency) {										// Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Visibility
	glEnable(GL_DEPTH_TEST);	// Depth test	
	glEnable(GL_CULL_FACE);		// Backface culling

	// Init scene importer
	SceneImporter s;
	s.setPrintInfo(false);
	std::vector<std::shared_ptr<PointLight>> allLights;
	std::shared_ptr<Camera> importedCamera; //TODO: not used yet

	// Import environment
	vector<shared_ptr<Geometry>> list = s.importFrom("./Models/freihaus_v0_9_environment.dae", &allLights, importedCamera);
	for (int i = 0; i < list.size(); i++){
		shared_ptr<Environment> f = make_shared<Environment>(list[i]->model_matrix, list[i]->meshes);
		environment.push_back(f);
	}
	
	// Import energy
	list = s.importFrom("./Models/freihaus_v0_9_energy.dae", &allLights, importedCamera);
	for (int i = 0; i < list.size(); i++){
		shared_ptr<Energy> f = make_shared<Energy>(list[i]->model_matrix, list[i]->meshes, 20);
		e_items.push_back(f);
	}

	// Init camera
	mat4 model = translate(mat4(1.0), vec3(-5.0, 1.0, 25.0));
	camera = new Camera(model);
	camera->setProjMatrix(width, height, fov, nearPlane, farPlane);
	
	// Set objects
	obj_manager.setObjects(camera,
		&allLights,
		&environment,
		&e_items);
	
	// Lighting and Shadows
	int sm_light_id = 4;
	obj_manager.setLighting(sm_light_id);
	cutaway.init(width, height);	
	
}
void update(GLFWwindow* window, float deltaTime) {

	// Camera
	vec3 direction = user_input.getMoveInput();
	camera->update(speed*deltaTime, direction);
	vec2 angle = user_input.getLookInput();
	camera->update(look_speed, angle);
	
	// Scene objects
	obj_manager.update(deltaTime);		
	
}

void createDepthImage() {
	cutaway.prepareZBufferPass();							
	obj_manager.renderToZBuffer(cutaway.z_buffer_shader, camera->proj_matrix*camera->view_matrix);
	cutaway.endZBufferPass();
}

void calculateCutawaySurface() {



	// Init step size with larger image dimension n
	int step = (width > height ? width / 2 : height / 2); 

	// Iterate over window with changing step size k = n/2^i at the ith iteration 
	while (step > 0) {

		// Draw quad + calculate distance transform


		// Update step size for next iteration
		step /= 2;
	}

}
void ShadowMapShadingPass() {
	glViewport(0, 0, width, height);				// Reset viewport
	cutaway.prepareShadingPass(2);				// Bind depth texture to unit 2
	//glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawnFaces = obj_manager.draw(&cutaway, useViewFrustumCulling);					// Draw scene objects
}

void draw() {	
}

void cleanup() {	
	delete camera;
}

void initScreenParameters(){

	// Read from file
	std::ifstream settingsFile("settings.txt");
	if (settingsFile.fail()){
		std::cout << "ERROR: Reading settingsfile failed! Using standard settings instead" << std::endl;
	}
	std::string type;
	int value;
	while (settingsFile >> type >> value){
		if (type == "width"){
			width = value;
		}
		else if (type == "height"){
			height = value;
		}
		else if (type == "refreshrate"){
			refreshrate = value;			
		}
		else if (type == "fullscreen"){
			if (value == 0){
				fullscreen = false;
			}
			else if (value == 1){
				fullscreen = true;
			}
			else{
				std::cout << "WARNING: In settings.txt is a value that has not been processed because the value of fullscreen must be 0 or 1 but is " << value << std::endl;
			}
		}
		else{
			std::cout << "WARNING: In settings.txt is a value that has not been processed because it's unknown: " << type << std::endl;
		}
	}
	std::cout << "width=" << width << ", height=" << height << ", refreshrate=" << refreshrate << ", fullscreen=" << fullscreen << std::endl;

}



