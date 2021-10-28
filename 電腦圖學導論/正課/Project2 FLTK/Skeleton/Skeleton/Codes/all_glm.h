#ifndef _ALL_GLM_H_
#define _ALL_GLM_H_
#include<iostream>
#include <glm\glm.hpp>
#include <glm\mat4x4.hpp>
#include <glm\vec4.hpp>
#include <glm\vec3.hpp>
#include <glm\matrix.hpp>
#include <glm\geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
void print_mat4x4(glm::mat4x4&);
class global{
public:
	static float aspect;
	static glm::mat4x4 model_view;
	static glm::mat4x4 projection;
};

//extern glm::mat4x4 MVP;
//extern int xxx = 0;
#endif // !_ALL_GLM_H_
