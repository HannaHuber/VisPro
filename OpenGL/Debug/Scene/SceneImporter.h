#pragma once
//#include "Triangle.h"
#include "Geometry.h"
#include <list>
#include <memory>
#include "Shader.h"
#include "Camera.h"
#include "BlinnPhongShader.h"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

class SceneImporter
{
public:
	std::vector<std::shared_ptr<Geometry>> SceneImporter::importFrom(const std::string& pFile, std::vector<std::shared_ptr<PointLight>> *allLights, std::shared_ptr<Camera> camera);
	void setPrintInfo(bool i);
private:
	void convertScene(const aiScene* scene);
	void iterateOverAllNodes(const aiNode* node, Geometry* targetParent, const aiScene* scene, const glm::mat4& accTransform);
	glm::mat4 assImpMatToGlmMat(const aiMatrix4x4 &m);
	std::vector<glm::vec3> aiVecToGlmVec3(aiVector3D* inputVectorlist, unsigned int amount);
	std::vector<glm::vec2> aiVecToGlmVec2(aiVector3D** inputVectorlist, unsigned int amount);
	Shader* aiMaterialToShader(aiMaterial *material, bool is_tex);
	void aiLightToLight(aiLight *aiL, glm::mat4& transform);
	void aiCameraToCamera(aiCamera *c);
	void CopyaiMat(const aiMatrix4x4 &from, glm::mat4 &to);
	bool printInfo;
};
	