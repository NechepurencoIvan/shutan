// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include<cmath>

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

#include <vector>
#include <random>
#include <iostream>

float my_random(){
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

#include "enemies.h"
#include "bullets.h"
#include "sphere.h"

float get_dist(glm::vec3 x1, glm::vec3 x2) {
	float ans = 0;
	for(size_t i = 0; i < 3; ++i){
		ans += std::pow(x1[i] - x2[i], 2);	
	}
	return sqrt(ans);
}

void collider(std::vector<EnemyObject>& enemies, std::vector<BulletObject>& bullets, std::vector<SphereObject>& spheres, float max_dist = 3){
	size_t i = 0;
	while(i < enemies.size()){
		bool have_deleted = false;
		for(size_t j = 0; j < bullets.size(); ++j){
			float dist = get_dist(enemies[i].coord, bullets[j].coord);
			if(dist < max_dist){
				spheres.push_back(SphereObject(enemies[i].coord, glm::vec3(0, 0, 0)));
				std::swap(enemies[enemies.size() - 1], enemies[i]);
				enemies.pop_back();
				have_deleted = true;
				break;
			}
			
		}
		if(not have_deleted){
			i++;
		}
	}
}

void exploses(std::vector<SphereObject>& spheres){
	size_t i = 0;
	while(i < spheres.size()){
		if(spheres[i].r > 2){
			std::swap(spheres[spheres.size() - 1], spheres[i]);
			spheres.pop_back();
		} else {
			i++;
		}

	}
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 0 - Keyboard and Mouse", NULL, NULL);
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

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	GLuint VertexArrayID1;
	glGenVertexArrays(1, &VertexArrayID1);
	glBindVertexArray(VertexArrayID1);
	// Create and compile our GLSL program from the shaders
	GLuint programID1 = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
	// Get a handle for our "MVP" uniform
	GLuint MatrixID1 = glGetUniformLocation(programID1, "MVP");
	// Load the texture
	GLuint Texture1 = loadBMP_custom("uvtemplate.bmp");
	//GLuint Texture1 = loadDDS("uvtemplate.DDS");
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID1  = glGetUniformLocation(programID1, "myTextureSampler");
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices	
	//GLuint vertexbuffer1;
	GLuint uvbuffer1;
	glGenBuffers(1, &uvbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);



	//GLuint VertexArrayID2;
	//glGenVertexArrays(1, &VertexArrayID2);
	//glBindVertexArray(VertexArrayID2);
	// Create and compile our GLSL program from the shaders
	GLuint programID2 = LoadShaders( "TransformVertexShaderBullet.vertexshader", "ColorFragmentShaderBullet.fragmentshader" );
	// Get a handle for our "MVP" uniform
	GLuint MatrixID2 = glGetUniformLocation(programID2, "MVP");

	GLuint colorbuffer2;
	glGenBuffers(1, &colorbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bullet_color_buffer_data), bullet_color_buffer_data, GL_STATIC_DRAW);


	GLuint programID3 = LoadShaders( "TransformVertexShaderBullet.vertexshader", "ColorFragmentShaderBullet.fragmentshader" );
	GLuint MatrixID3 = glGetUniformLocation(programID2, "MVP");
	//сфера

	//GLfloat* sphere_color_buffer_data = getSphereColor();

	GLuint colorbuffer3;
	GLfloat* sphere_color_buff = get_sphere_colors();
	glGenBuffers(1, &colorbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer3);
	glBufferData(GL_ARRAY_BUFFER, sphere_num_triangles * 9 * 10, sphere_color_buff, GL_STATIC_DRAW);


	std::vector<BulletObject> bullets;
	std::vector<EnemyObject> eos;
	std::vector<SphereObject> spheres;

	int timestamp = 0;
	int cooldown = 0;
	int cooldown_time = 50;	

	do{

		if(timestamp % 200 == 0){
			for(size_t i = 0; i < 2; ++i){
				glm::vec3 coord = generate_enemy_coord();
				glm::vec3 speed = get_enemy_speed(coord);//{0.005, 0.005, 0.005};
				eos.push_back(EnemyObject(coord, speed));

				//coord = generate_enemy_coord();
				//speed = get_enemy_speed(coord);//{0.005, 0.005, 0.005};
				//spheres.push_back(SphereObject(coord, speed));

				//std::cout<<coord[0]<< " " << coord[1] << " " << coord[2] <<std::endl;
			}
		}
		timestamp++;

		if(glfwGetKey( window, GLFW_KEY_E) == GLFW_PRESS) {
			if(cooldown == 0 ){
				cooldown = cooldown_time;
				bullets.push_back(BulletObject(get_position(), get_direction(), 0.5));
			}
		}

		if(cooldown > 0) {
			cooldown--;
		}

		collider(eos, bullets, spheres);
		exploses(spheres);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID1);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform

		
		glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture1);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID1, 0);


		glEnableVertexAttribArray(0);
		for (EnemyObject& eo:eos){
			eo.update();
			GLuint vertexbuffer1 = eo.make_enemy();

			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);


			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				2,                                // size : U+V => 2
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 12 * 9); // 12*3 indices starting at 0 -> 12 triangles
			glDeleteBuffers(1, &vertexbuffer1);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		

		glUseProgram(programID2);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		
		
		for(BulletObject& bo: bullets){
			GLuint vertexbuffer2 = bo.make_bullet();
			bo.update();
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// 2nd attribute buffer : colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 4*3 * 2); // 12*3 indices starting at 0 -> 12 triangles
			glDeleteBuffers(1, &vertexbuffer2);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		

		glUseProgram(programID3);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID3, 1, GL_FALSE, &MVP[0][0]);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);


		for(SphereObject& so: spheres){
			GLuint vertexbuffer3 = so.make_sphere();
			so.update();
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// 2nd attribute buffer : colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer3);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, sphere_num_triangles * 9 * 10); // 12*3 indices starting at 0 -> 12 triangles*/
			glDeleteBuffers(1, &vertexbuffer3);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	//glDeleteBuffers(1, &vertexbuffer1);
	glDeleteBuffers(1, &uvbuffer1);
	glDeleteProgram(programID1);
	glDeleteTextures(1, &TextureID1);
	glDeleteVertexArrays(1, &VertexArrayID1);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

