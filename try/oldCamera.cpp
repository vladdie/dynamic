﻿#include "oldCamera.h"

extern GLFWwindow* window;

oldCamera::oldCamera(void)
{
	position = glm::vec3(0.0f, 50.0f, 60.0f); 
	//position = glm::vec3(0.0f, 0.0f, 5.0f); 
	target = glm::vec3(0.0f, -0.2f, 1.0f);
	up = glm::vec3(0.0, 1.0f, 0.0f);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	initialFoV = 60.0f;
	speed = 3.0f;
	mouseSpeed = 0.005f;
}


oldCamera::~oldCamera(void)
{
}


void oldCamera::computeMatricesFromInputs(){


	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	//// Get mouse position
	//double xpos, ypos;
	//glfwGetCursorPos(window, &xpos, &ypos);

	//// Reset mouse position for next frame
	//glfwSetCursorPos(window, 1024/2, 768/2);

	//// Compute new orientation, glfwGetWindowSize
	////1024/2 - xpos:鼠标离窗口中心点的距离,值越大，转动角越大
	//horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	//verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	//direction is the sight direction of camera
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector, the direction is pointing out of screen, with values on X & Z, Y=0
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
		);

	// Up vector
	up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 1.0f, 10000.0f);

	glm::vec3 origin = glm::vec3(0,0,0);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		position,           // Camera is here
		position+direction, // and looks here : at the same position, plus "direction"
		//origin,
		//target,
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

glm::mat4 oldCamera::getViewMatrix(){

	return ViewMatrix;
}

glm::mat4 oldCamera::getProjectionMatrix(){
	return ProjectionMatrix;
}

glm::mat4 oldCamera::setViewMatrix(glm::vec3 pos, glm::vec3 dir)
{
	return glm::lookAt(pos,dir,up);
}

glm::vec3 oldCamera::getEyePosition(){
	return position;
}