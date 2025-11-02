// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <thread>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>


#define OCTAGON_RADIUS (1.55)

int main( void )
{
    // Initialize GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    // Setup window
    glfwWindowHint(GLFW_SAMPLES, 4); // Use 4 sample for multisample anti-aliasing 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Major version of API
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Minor version of API
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Specifies if OpenGL context should be forward-compatible
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Specifies which OpenGL profile to create the context for

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 09 - Loading with AssImp", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the keys being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    
    // Accept fragment if it is closer to the camera than the former one
    glDepthFunc(GL_LESS); 

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Get a handle for "is_ell_active"
    static GLint is_ell_active = 0;
    static bool is_ell_pressed = false;
    GLuint is_ell_active_handle = glGetUniformLocation(programID, "is_ell_active");
    glUniform1i(is_ell_active_handle, is_ell_active);

    /* For suzanne */
    
    // Load the texture
    GLuint Texture = loadDDS("uvmap.DDS");
    
    // Read our .obj file
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    bool res = loadAssImp("suzanne.obj", indices, indexed_vertices, indexed_uvs, indexed_normals);

    /* Load it into a VBO */
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    /* End suzanne */


    /* For green square/rectange */

    // Load the texture
    GLuint rect_Texture = loadDDS("greenTile.dds");

    // Read our .obj file
    std::vector<unsigned short> rect_indices;
    std::vector<glm::vec3> rect_indexed_vertices;
    std::vector<glm::vec2> rect_indexed_uvs;
    std::vector<glm::vec3> rect_indexed_normals;
    bool rect_res = loadAssImp("green_tile.obj", rect_indices, rect_indexed_vertices, rect_indexed_uvs, rect_indexed_normals);

    /* Load it into a VBO */
    GLuint rect_vertexbuffer;
    glGenBuffers(1, &rect_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, rect_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, rect_indexed_vertices.size() * sizeof(glm::vec3), &rect_indexed_vertices[0], GL_STATIC_DRAW);

    GLuint rect_uvbuffer;
    glGenBuffers(1, &rect_uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, rect_uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, rect_indexed_uvs.size() * sizeof(glm::vec2), &rect_indexed_uvs[0], GL_STATIC_DRAW);

    GLuint rect_normalbuffer;
    glGenBuffers(1, &rect_normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, rect_normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, rect_indexed_normals.size() * sizeof(glm::vec3), &rect_indexed_normals[0], GL_STATIC_DRAW);

    GLuint rect_elementbuffer;
    glGenBuffers(1, &rect_elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect_elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rect_indices.size() * sizeof(unsigned short), &rect_indices[0] , GL_STATIC_DRAW);

    /* End green rectangle */

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    glm::vec3 lightPos = glm::vec3(4, 4, 4);
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do{

        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
            // printf and reset
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard input
        // Turn off specular and diffuse light components
        if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS)
        {
            if((is_ell_active == 0) && (is_ell_pressed == false))
            {
                is_ell_active = 1;
            }
            else if((is_ell_active == 1) && (is_ell_pressed == false))
            {
                is_ell_active = 0;
            }
            is_ell_pressed = true;
        }
        else
        {
            is_ell_pressed = false;
        }
        glUniform1i(is_ell_active_handle, is_ell_active);
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);


        /* Draw green rectangle */

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rect_Texture);
        glUniform1i(TextureID, 0); // Set our "myTextureSampler" sampler to use Texture Unit 0

        // Compute the MVP matrix from keyboard input
        glm::mat4 rect_matrix = glm::mat4(1.0);
        glm::mat4 rect_MVP0 = ProjectionMatrix * ViewMatrix * rect_matrix;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &rect_MVP0[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &rect_matrix[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, rect_vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, rect_uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, rect_normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect_elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, rect_indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End green rectangle */


        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(TextureID, 0); // Set our "myTextureSampler" sampler to use Texture Unit 0


        /* Render the zeroth head */
        
        // Compute the MVP matrix from keyboard input
        glm::mat4 ModelMatrix0 = glm::mat4(1.0);
        ModelMatrix0 = glm::translate(ModelMatrix0, glm::vec3(0.0f, 0.984375f, OCTAGON_RADIUS*glm::tan(glm::radians(67.5))));
        glm::mat4 MVP0 = ProjectionMatrix * ViewMatrix * ModelMatrix0;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP0[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix0[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End zeroth head */


        /* Render the first head */

        // Compute the MVP matrix from keyboard input
        glm::mat4 ModelMatrix1 = glm::mat4(1.0);
        ModelMatrix1 = glm::rotate(ModelMatrix1, glm::radians(45.0f), glm::vec3(0, 1, 0));
        ModelMatrix1 = glm::translate(ModelMatrix1, glm::vec3(0.0f, 0.984375f, OCTAGON_RADIUS*glm::tan(glm::radians(67.5))));
        glm::mat4 MVP1 = ProjectionMatrix * ViewMatrix * ModelMatrix1;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix1[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End first head */


        /* Render the second head */

        // Compute the MVP matrix from keyboard input
        glm::mat4 ModelMatrix2 = glm::mat4(1.0);
        ModelMatrix2 = glm::rotate(ModelMatrix2, glm::radians(90.0f), glm::vec3(0, 1, 0));
        ModelMatrix2 = glm::translate(ModelMatrix2, glm::vec3(0.0f, 0.984375f, OCTAGON_RADIUS*glm::tan(glm::radians(67.5))));
        glm::mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix2[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End second head */


        /* Render the third head */

        // Compute the MVP matrix from keyboard input
        glm::mat4 ModelMatrix3 = glm::mat4(1.0);
        ModelMatrix3 = glm::rotate(ModelMatrix3, glm::radians(135.0f), glm::vec3(0, 1, 0));
        ModelMatrix3 = glm::translate(ModelMatrix3, glm::vec3(0.0f, 0.984375f, OCTAGON_RADIUS*glm::tan(glm::radians(67.5))));
        glm::mat4 MVP3 = ProjectionMatrix * ViewMatrix * ModelMatrix3;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix3[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End third head */


        /* Render the fourth head */

        // Compute the MVP matrix from keyboard input
        glm::mat4 ModelMatrix4 = glm::mat4(1.0);
        ModelMatrix4 = glm::rotate(ModelMatrix4, glm::radians(180.0f), glm::vec3(0, 1, 0));
        ModelMatrix4 = glm::translate(ModelMatrix4, glm::vec3(0.0f, 0.984375f, OCTAGON_RADIUS*glm::tan(glm::radians(67.5))));
        glm::mat4 MVP4 = ProjectionMatrix * ViewMatrix * ModelMatrix4;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP4[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix4[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End fourth head */


        /* Render the fifth head */

        // Compute the MVP matrix from keyboard input
        glm::mat4 ModelMatrix5 = glm::mat4(1.0);
        ModelMatrix5 = glm::rotate(ModelMatrix5, glm::radians(225.0f), glm::vec3(0, 1, 0));
        ModelMatrix5 = glm::translate(ModelMatrix5, glm::vec3(0.0f, 0.984375f, OCTAGON_RADIUS*glm::tan(glm::radians(67.5))));
        glm::mat4 MVP5 = ProjectionMatrix * ViewMatrix * ModelMatrix5;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP5[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix5[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End fifth head */


        /* Render the sixth head */

        // Compute the MVP matrix from keyboard input
        glm::mat4 ModelMatrix6 = glm::mat4(1.0);
        ModelMatrix6 = glm::rotate(ModelMatrix6, glm::radians(270.0f), glm::vec3(0, 1, 0));
        ModelMatrix6 = glm::translate(ModelMatrix6, glm::vec3(0.0f, 0.984375f, OCTAGON_RADIUS*glm::tan(glm::radians(67.5))));
        glm::mat4 MVP6 = ProjectionMatrix * ViewMatrix * ModelMatrix6;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP6[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix6[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End sixth head */


        /* Render the seventh head */

        // Compute the MVP matrix from keyboard input
        glm::mat4 ModelMatrix7 = glm::mat4(1.0);
        ModelMatrix7 = glm::rotate(ModelMatrix7, glm::radians(315.0f), glm::vec3(0, 1, 0));
        ModelMatrix7 = glm::translate(ModelMatrix7, glm::vec3(0.0f, 0.984375f, OCTAGON_RADIUS*glm::tan(glm::radians(67.5))));
        glm::mat4 MVP7 = ProjectionMatrix * ViewMatrix * ModelMatrix7;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP7[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix7[0][0]);
        
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        /* End seventh head */


        // Disable vertex arrays
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteBuffers(1, &rect_vertexbuffer);
    glDeleteBuffers(1, &rect_uvbuffer);
    glDeleteBuffers(1, &rect_normalbuffer);
    glDeleteBuffers(1, &rect_elementbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteTextures(1, &rect_Texture);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

