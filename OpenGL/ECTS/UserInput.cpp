#include "UserInput.h"

using namespace glm;
using namespace std;


bool useWireFrame = false;
bool showFrameTime = true;
bool useLinearTexSampling = true;
int useMipMapping = 2;
bool useViewFrustumCulling = true;
bool useTransparency = true;
bool useBloom = false;
bool countDown = true;

UserInput::UserInput(GLFWwindow* win)
: window(win)
{
	// Define screen center position for offset calculation in getLookInput()
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	screen_center = vec2(width / 2.0, height / 2.0);
}

UserInput::UserInput() {

}

UserInput::~UserInput() {

}

void UserInput::init(GLFWwindow* win, bool wire, bool frame, bool lin_tex, int mip, bool vf_cull, bool transp, bool bloom, bool count) {
	window = win;

	useWireFrame = wire;
	showFrameTime = frame;
	useLinearTexSampling = lin_tex;
	useMipMapping = mip;
	useViewFrustumCulling = vf_cull;
	useTransparency = transp;	
	useBloom = bloom;
	countDown = count;
	
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	screen_center = vec2(width / 2.0, height / 2.0);

	// Set cursor
	glfwSetCursorPos(window, screen_center.x, screen_center.y);		// Set cursor position for next frame
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Makes the cursor invisible

	// Capture key input after release
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

} 

bool UserInput::terminate() {
	return (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) || (glfwWindowShouldClose(window) == 1);
}

vec3 UserInput::getMoveInput() {
	vec3 direction = vec3(0.0);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		direction = vec3(0.0, 0.0, -1.0);					// move forward
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		direction = vec3(-1.0, 0.0, 0.0);					// move left
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		direction = vec3(0.0, 0.0, 1.0);					// move backward
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		direction = vec3(1.0, 0.0, 0.0);					// move right
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			direction += vec3(0.0, 1.0, 0.0);					// jump (move upwards)
		
	}
	return direction;
}

vec2 UserInput::getLookInput() {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);							// Get cursor position
	glfwSetCursorPos(window, screen_center.x, screen_center.y);		// Set cursor position for next frame
	return vec2(screen_center.x - xpos, ypos - screen_center.y);	// return horizontal and vertical angle
}

bool UserInput::getFireInput() {
	return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
}

void UserInput::switchFrameTimeMode() {
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		if (glfwGetTime() - lastButtonPress > minDist){
			lastButtonPress = glfwGetTime();
		

			if (showFrameTime) {
				showFrameTime = false;
		}
		else {
			showFrameTime = true;
		}
		}
	}
}
void UserInput::switchWireFrameMode() {
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		if (glfwGetTime() - lastButtonPress > minDist){
			lastButtonPress = glfwGetTime();


			if (useWireFrame) {
				useWireFrame = false;
			}
			else {
				useWireFrame = true;
			}
		}
	}

}
void UserInput::switchTexSamplingMode() {
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		if (glfwGetTime() - lastButtonPress > minDist){
			lastButtonPress = glfwGetTime();
			if (useLinearTexSampling){
				useLinearTexSampling = false;
			}
			else{
				useLinearTexSampling = true;
			}
		}

	}

}
void UserInput::switchMipMapMode() {
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
		
		if (glfwGetTime() - lastButtonPress > minDist){
			lastButtonPress = glfwGetTime();
			if (useMipMapping == 0){
				useMipMapping = 1;
			}
			else if (useMipMapping == 1){
				useMipMapping = 2;
			}
			else{
				useMipMapping = 0;
			}
		}

	}

}
void UserInput::switchCountdownMode(){
	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS) {
		if (glfwGetTime() - lastButtonPress > minDist){
			lastButtonPress = glfwGetTime();
			if (countDown){
				countDown = false;
			}
			else{
				countDown = true;
			}
		}
	}
}
void UserInput::switchViewFrustumCullingMode() {
	if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS) {
		if (glfwGetTime() - lastButtonPress > minDist){
			lastButtonPress = glfwGetTime();
			if (useViewFrustumCulling){
				useViewFrustumCulling = false;
			}
			else{
				useViewFrustumCulling = true;
			}
		}
	}

}
void UserInput::switchTransparencyMode() {
	if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS) {
		if (glfwGetTime() - lastButtonPress > minDist){
			lastButtonPress = glfwGetTime();
			if (useTransparency) {
				glDisable(GL_BLEND);
				useTransparency = false;
			}
			else {
				glEnable(GL_BLEND);
				useTransparency = true;
			}
		}
	}

}

void UserInput::switchBloomMode(){
	if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS) {
		if (glfwGetTime() - lastButtonPress > minDist){
			std::cout << "Switched Bloom mode" << std::endl;
			lastButtonPress = glfwGetTime();
			if (useBloom){
				useBloom = false;
			}
			else{
				useBloom = true;
			}
		}
	}
}

void UserInput::switchModi() {
	
		switchWireFrameMode();
		switchFrameTimeMode();
		switchTexSamplingMode();
		switchMipMapMode();
		switchViewFrustumCullingMode();
		switchTransparencyMode();
		switchBloomMode();
		switchCountdownMode();	
}