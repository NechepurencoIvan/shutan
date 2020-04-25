#pragma once
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
const int N_PHI = 50;
const int N_PSI = 50;
const float PI = atan2(0., -1.);

const size_t sphere_num_triangles = 2 * N_PSI * N_PHI ;

GLfloat* get_sphere_coords(){

    glm::vec3 vertices[N_PSI][N_PHI];

    for (int j = 1; j < N_PSI; ++j) {
        for (int i = 0; i < N_PHI; ++i) {
            float phi = i * 2 * PI / N_PHI;
            float psi = j * PI / N_PSI;
            vertices[j][i] = glm::vec3(sin(psi) * cos(phi), sin(psi) * sin(phi), cos(psi));
        }
    }

    glm::vec3 north_pole = vec3(0., 0., 1.);
    glm::vec3 south_pole = vec3(0., 0., -1.);

    GLfloat* g_vertex_buffer_data = new GLfloat[sphere_num_triangles * 9];

    int cur_idx = 0;

    for (int i = 0; i < N_PHI; ++i) {
        g_vertex_buffer_data[cur_idx++] = north_pole.x;
        g_vertex_buffer_data[cur_idx++] = north_pole.y;
        g_vertex_buffer_data[cur_idx++] = north_pole.z;

        g_vertex_buffer_data[cur_idx++] = vertices[1][i].x;
        g_vertex_buffer_data[cur_idx++] = vertices[1][i].y;
        g_vertex_buffer_data[cur_idx++] = vertices[1][i].z;

        g_vertex_buffer_data[cur_idx++] = vertices[1][(i + 1) % N_PHI].x;
        g_vertex_buffer_data[cur_idx++] = vertices[1][(i + 1) % N_PHI].y;
        g_vertex_buffer_data[cur_idx++] = vertices[1][(i + 1) % N_PHI].z;
    }

    for (int i = 0; i < N_PHI; ++i) {
        g_vertex_buffer_data[cur_idx++] = south_pole.x;
        g_vertex_buffer_data[cur_idx++] = south_pole.y;
        g_vertex_buffer_data[cur_idx++] = south_pole.z;

        g_vertex_buffer_data[cur_idx++] = vertices[N_PSI - 1][i].x;
        g_vertex_buffer_data[cur_idx++] = vertices[N_PSI - 1][i].y;
        g_vertex_buffer_data[cur_idx++] = vertices[N_PSI - 1][i].z;

        g_vertex_buffer_data[cur_idx++] = vertices[N_PSI - 1][(i + 1) % N_PHI].x;
        g_vertex_buffer_data[cur_idx++] = vertices[N_PSI - 1][(i + 1) % N_PHI].y;
        g_vertex_buffer_data[cur_idx++] = vertices[N_PSI - 1][(i + 1) % N_PHI].z;
    }

    for (int j = 1; j < N_PSI - 1; ++j) {
        for (int i = 0; i < N_PHI; ++i) {
            g_vertex_buffer_data[cur_idx++] = vertices[j][i].x;
            g_vertex_buffer_data[cur_idx++] = vertices[j][i].y;
            g_vertex_buffer_data[cur_idx++] = vertices[j][i].z;

            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][i].x;
            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][i].y;
            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][i].z;

            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][(i + 1) % N_PHI].x;
            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][(i + 1) % N_PHI].y;
            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][(i + 1) % N_PHI].z;

            g_vertex_buffer_data[cur_idx++] = vertices[j][i].x;
            g_vertex_buffer_data[cur_idx++] = vertices[j][i].y;
            g_vertex_buffer_data[cur_idx++] = vertices[j][i].z;

            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][(i + 1) % N_PHI].x;
            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][(i + 1) % N_PHI].y;
            g_vertex_buffer_data[cur_idx++] = vertices[j + 1][(i + 1) % N_PHI].z;

            g_vertex_buffer_data[cur_idx++] = vertices[j][(i + 1) % N_PHI].x;
            g_vertex_buffer_data[cur_idx++] = vertices[j][(i + 1) % N_PHI].y;
            g_vertex_buffer_data[cur_idx++] = vertices[j][(i + 1) % N_PHI].z;

        }
    }
    //std::cout<<sizeof(g_vertex_buffer_data)<<std::endl;
    return g_vertex_buffer_data;
}


GLfloat* get_sphere_colors(){
	GLfloat* color_buff = new GLfloat[sphere_num_triangles * 9];
	for (size_t i = 0; i < sphere_num_triangles * 9; ++i){
		color_buff[i] = 0.;
		if(i % 3 == 0){
			color_buff[i] = 1.; //bullet_color_buffer_data[i];
		}
	}
	return color_buff;	
}

struct SphereObject {
	glm::vec3 coord;
	glm::vec3 speed;
	float r = 0;

	SphereObject(glm::vec3 coord_, glm::vec3 speed_){
		coord = coord_;
		speed = speed_;
	}

	void update() {
		r += 0.1;
		for(size_t i = 0; i < 3; ++i){
			coord[i] += speed[i];
		}			
	}


	GLuint make_sphere(){
		GLfloat* pts = get_sphere_coords();
		//std::cout<<sizeof(pts)<<std::endl;

		for(size_t i = 0; i < sphere_num_triangles * 9; ++i){
			pts[i] = pts[i] * r + coord[i % 3];
		}

		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sphere_num_triangles * 9 * 10, pts, GL_STATIC_DRAW);
		delete[] pts;
		return vertexbuffer;
	}
	
};
