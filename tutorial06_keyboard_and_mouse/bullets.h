#pragma once
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

static const GLfloat bullet_vertex_buffer_data[] = { 
	-1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f,

	 0.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	 0.0f, -1.0f, 0.0f,

	 0.0f, 0.0f, 0.0f,
	 0.0f, -1.0f, 0.0f,
	 0.0f, 0.0f,-1.0f,

	 -1.0f, 0.0f, 0.0f,
	 0.0f, -1.0f, 0.0f,
	 0.0f, 0.0f,-1.0f,

	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	 0.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,

	 0.0f, 0.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,
	 0.0f, 0.0f,1.0f,

	 1.0f, 0.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,
	 0.0f, 0.0f,1.0f


};

static const GLfloat bullet_color_buffer_data[] = { 
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
};


struct BulletObject {
	size_t num_triangles = 8;
	glm::vec3 coord;
	glm::vec3 speed;
	float scale = 0.2;

	BulletObject(glm::vec3 coord_, glm::vec3 direction, float velocity) {
		for(size_t i = 0; i < 3; ++i){
			coord[i] = coord_[i];
			speed[i] = 0;//direction[i] * velocity;
		}
	}

	void update() {
		for(size_t i = 0; i < 3; ++i){
			coord[i] += speed[i];
		}			
	}

	GLuint make_bullet(){
		GLfloat* pts = new GLfloat[num_triangles * 9];

		for(size_t i = 0; i < num_triangles * 9; ++i){
			pts[i] = bullet_vertex_buffer_data[i] * scale;
		}

		for(size_t i = 0; i < num_triangles * 9; ++i){
			pts[i] += coord[i % 3];
		}

		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(bullet_vertex_buffer_data), pts, GL_STATIC_DRAW);
		delete[] pts;
		return vertexbuffer;
	}
	
};
