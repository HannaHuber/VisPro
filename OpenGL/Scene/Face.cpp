#include "Face.h"
#include <iostream>


Face::Face(unsigned int v_count, unsigned int* v_indices)
: v_count(v_count)
, v_indices(v_indices)
{
	if (v_count < 3){
		std::cout << "OMG! There is a Face with less than 3 vertices! v_count="<<v_count<< std::endl;
		system("PAUSE");
	}
}
std::vector<GLuint> Face::getIndexVector(){
	std::vector<GLuint> result;
	result.assign(v_indices,v_indices+v_count);
	return result;
}

Face::~Face() {

}