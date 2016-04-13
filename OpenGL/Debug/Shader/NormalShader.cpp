#include "NormalShader.h"

NormalShader::NormalShader()
: Shader("./Shader/VertexShader.vert", "./Shader/FragmentShader.frag")
{
	
}

void NormalShader::setLighting(std::vector<std::shared_ptr<PointLight>> *allLights) {

}

