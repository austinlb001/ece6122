// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}



glm::vec3 position = glm::vec3( 0, 0, 0 ); // Initial position : on +Z
glm::vec3 origin = glm::vec3( 0, 0, 0 );
float initialFoV = 45.0f; // Initial Field of View
float radius = 10.0;
const float deltaRadius = 0.01;
float theta = 20.0; // Vertical Angle
const float deltaTheta = 0.01;
float phi = 0.0; // Horizontal Angle
const float deltaPhi = 0.25;


void computeMatricesFromInputs(){

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    
    // Up vector
    glm::vec3 up = glm::vec3( 0, 1, 0 );

    // Zoom in
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
        radius -= deltaRadius;
    }
    // Zoom out
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
        radius += deltaRadius;
    }
    // Move camera left
    if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
        phi -= deltaPhi;
    }
    // Move camera right
    if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
        phi += deltaPhi;
    }
    // Move camera up
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
        theta += deltaTheta;
    }
    // Move camera down
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        theta -= deltaTheta;
    }

    float FoV = initialFoV; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Calculate coordinates
    position.x = radius*cos(glm::radians(theta))*sin(glm::radians(phi));
    position.y = radius*sin(theta);
    position.z = radius*cos(glm::radians(theta))*cos(glm::radians(phi));

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix       = glm::lookAt(
                                position,           // Camera is here
                                origin, // and looks here
                                up                  // Head is up (set to 0,-1,0 to look upside-down)
                           );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime; 
}