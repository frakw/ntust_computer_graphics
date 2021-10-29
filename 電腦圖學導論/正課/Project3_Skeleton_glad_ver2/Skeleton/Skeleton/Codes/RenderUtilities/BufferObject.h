#pragma once
#include <glad\glad.h>

#define MAX_FBO_TEXTURE_AMOUNT 4
#define MAX_VAO_VBO_AMOUNT 3

struct VAO
{
	GLuint vao;
	GLuint vbo[MAX_VAO_VBO_AMOUNT];
	GLuint ebo;
	union
	{
		unsigned int element_amount;//for draw element
		unsigned int count;			//for draw array
	};
};
struct UBO
{
	GLuint ubo;
	GLsizeiptr size;
};
struct FBO
{
	GLuint fbo;	//frame buffer
	GLuint textures[MAX_FBO_TEXTURE_AMOUNT];	//attach to color buffer
	GLuint rbo;	//attach to depth and stencil
};