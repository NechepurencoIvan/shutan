#pragma once
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

static const GLfloat g_vertex_buffer_data[] = { 
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,

	 1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,

	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,

	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,

	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,

	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,

	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,

	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,

	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,

	 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,

	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,

	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f
};

static const GLfloat g_uv_buffer_data[] = { 
	0.000059f, 0.000004f, 
	0.000103f, 0.336048f, 
	0.335973f, 0.335903f, 

	1.000023f, 0.000013f, 
	0.667979f, 0.335851f, 
	0.999958f, 0.336064f, 

	0.667979f, 0.335851f, 
	0.336024f, 0.671877f, 
	0.667969f, 0.671889f, 

	1.000023f, 0.000013f, 
	0.668104f, 0.000013f, 
	0.667979f, 0.335851f, 

	0.000059f, 0.000004f, 
	0.335973f, 0.335903f, 
	0.336098f, 0.000071f, 

	0.667979f, 0.335851f, 
	0.335973f, 0.335903f, 
	0.336024f, 0.671877f, 

	1.000004f, 0.671847f, 
	0.999958f, 0.336064f, 
	0.667979f, 0.335851f, 

	0.668104f, 0.000013f, 
	0.335973f, 0.335903f, 
	0.667979f, 0.335851f, 

	0.335973f, 0.335903f, 
	0.668104f, 0.000013f, 
	0.336098f, 0.000071f, 

	0.000103f, 0.336048f, 
	0.000004f, 0.671870f, 
	0.336024f, 0.671877f, 

	0.000103f, 0.336048f, 
	0.336024f, 0.671877f, 
	0.335973f, 0.335903f, 

	0.667969f, 0.671889f, 
	1.000004f, 0.671847f, 
	0.667979f, 0.335851f
};

glm::vec3 generate_enemy_coord() {
	glm::vec3 vec{0, 0, 0};
	float scale = 80;
	vec[0] = scale * (-0.5 + my_random());
	vec[1] = scale * (-0.5 + my_random());
	vec[2] = scale * (-0.5 + my_random());
	for(size_t i = 0; i < 3; ++i){
		float d = vec[i] - get_position()[i];
	}
	return vec;
}

glm::vec3 get_enemy_speed(glm::vec3 pos) {
	glm::vec3 speed(0, 0, 0);
	glm::vec3 player_position = get_position();
	for(size_t i = 0; i < 3; ++i) {
		speed[i] = 0;//(player_position[i] - pos[i]) / 500.;
	}
	return speed;
}

struct EnemyObject {
	size_t num_triangles = 12;
	glm::vec3 coord;
	glm::vec3 speed;
	float angle;
	glm::vec3 myRotationAxis;

	EnemyObject(glm::vec3 coord_, glm::vec3 speed_){
		angle = 0;
		myRotationAxis = glm::vec3(1., 0., 0.);
		coord = coord_;
		speed = speed_;
	}

	void update() {
		angle += 0.03;
		for(size_t i = 0; i < 3; ++i){
			coord[i] += speed[i];
		}			
	}


	void apply_rotation(GLfloat* pts) {
		glm::mat4 RotationMatrix = glm::rotate(angle,  myRotationAxis);

		for(size_t i = 0; i < num_triangles * 3; ++ i){
			glm::vec4 temp;
			temp[3] = 1;
			for(size_t j = 0; j < 3; ++j) {
				temp[j] = pts[3 * i + j];
			}
			temp = RotationMatrix * temp;

			for(size_t j = 0; j < 3; ++j) {
				pts[3 * i + j] = temp[j];
			}
			
		}
	}

	GLuint make_enemy(){
		GLfloat* pts = new GLfloat[num_triangles * 9];

		for(size_t i = 0; i < num_triangles * 9; ++i){
			pts[i] = g_vertex_buffer_data[i];
		}

		apply_rotation(pts);		

		for(size_t i = 0; i < num_triangles * 9; ++i){
			pts[i] += coord[i % 3];
		}

		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), pts, GL_STATIC_DRAW);
		delete[] pts;
		return vertexbuffer;
	}
	
};
