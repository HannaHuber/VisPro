#include <iostream>
#include "SceneImporter.h"
#include "Geometry.h"
#include "TextureShader.h"
#include "AnimatedTextureShader.h"
#include "Camera.h"
#include "gtc/matrix_transform.hpp"

#include <vector>
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

aiMesh** meshes;
unsigned int numMeshes;
std::vector<std::shared_ptr<Geometry>> allGeometries;
std::vector<std::shared_ptr<PointLight>> *allLights;
std::vector<aiLight*> scene_lights;
std::shared_ptr<Camera> camera;


unsigned int importedNodes;
unsigned int importedMeshes;
unsigned int importedFaces;

//Geometry *parent;
//glm::mat4 transform;

	// based on http://assimp.sourceforge.net/lib_html/usage.html
std::vector<std::shared_ptr<Geometry>> SceneImporter::importFrom(const std::string& pFile, std::vector<std::shared_ptr<PointLight>> *allL, std::shared_ptr<Camera> c){
	allLights = allL;
	camera = c;


	
		// Create an instance of the Importer class
		Assimp::Importer importer;
		// Read the file + some postprocessing
		const aiScene* scene = importer.ReadFile(pFile,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			//aiProcess_PreTransformVertices | //TODO: delete this line when using of model matrix is correct
			aiProcess_GenNormals); 
		
		if (!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) { // import failed
			std::cerr << "ERROR ASSIMP: " << importer.GetErrorString() << std::endl;
			std::cout << "Failed to import scene file ("<<pFile<<")" << std::endl;
			// TODO throw some exception at this point?
		}
		// Access the file's contents.
		allGeometries.clear(); //TODO: could cause some trouble if there are several files imported
		
	
			std::cout << "Importing scene file " << pFile << "..." << std::endl;
			
		importedFaces = 0;
		importedMeshes = 0;
		importedNodes = 0;
		convertScene(scene);
		
		if (printInfo) {
			std::cout << "Importing scene file " << pFile << " finished. " << allGeometries.size() << " Geometry Objects created. Created Nodes: " << importedNodes << ". Imported Meshes: " << importedMeshes << ". Imported Faces: " << importedFaces << std::endl;
			for (unsigned int i = 0; i < allGeometries.size(); i++){
				std::cout << "	   Geometry object " << i << " contains " << allGeometries[i].get()->meshes.size() << " meshes" << std::endl;
			}
		}
		return allGeometries;
		// Note: Everything will be cleaned up by the assimp importer destructor
	}

	//convert scene data to our own SceneObject format
	void SceneImporter::convertScene(const aiScene* scene){
		aiNode* rootNode = scene->mRootNode;

		if (printInfo) {
			std::cout << "Starting at root node " << rootNode->mName.C_Str() << "..." << std::endl;
		}

		// Scene lights
		if (scene->HasLights()){
			for (int i = 0; i < scene->mNumLights; i++){
				scene_lights.push_back(scene->mLights[i]);				
			}
		}
		
		iterateOverAllNodes(rootNode, nullptr, scene, glm::mat4(1.0));
		
		//// Lights TODO fix const node in iteration to store absolute transformation
		// below only the relative trafo is used -> y/z swapped
		//if (scene->HasLights()){
		//	for (int i = 0; i < scene->mNumLights; i++){
		//		aiLight* a = scene->mLights[i];
		//		const aiNode* n = scene->mRootNode->FindNode(a->mName);
		//		aiLightToLight(a, assImpMatToGlmMat(n->mTransformation));
		//	}
		//	std::cout << "	"<<scene->mNumLights<<"Lights imported!" << std::endl;
		//}
		// Camera
		if (scene->HasCameras()){
			aiCamera* c = scene->mCameras[0]; //only first camera is considered
			aiCameraToCamera(c);
			if (printInfo) {
				std::cout << "	Camera imported!" << std::endl;
			}
		}
	}
	



	// as advised by assimp doc (http://assimp.sourceforge.net/lib_html/data.html), based on CopyNodesWithMeshes
	void SceneImporter::iterateOverAllNodes(const aiNode* node, Geometry* targetParent, const aiScene* scene, const glm::mat4& accTransform){

		
		if (printInfo) {
			std::cout << std::endl << "Importing node " << node->mName.C_Str() << "..." << std::endl;
		}

		Geometry* parent;

		// Get absolute model matrix
		glm::mat4 transform = accTransform*assImpMatToGlmMat(node->mTransformation);
		
		// if node has meshes, create a new scene object for it
		if (node->mNumMeshes > 0)
		{
			importedNodes++;
			unsigned int* meshPointer = node->mMeshes;
			std::vector<Mesh*> meshList; 
			int facesBefore = importedFaces;
			for (unsigned int i = 0; i < node->mNumMeshes; i++){ //iterating over all meshes of the node
				unsigned int meshIndex = node->mMeshes[i];
				aiMesh* m = scene->mMeshes[meshIndex];
				
				aiFace* facesPointer = m->mFaces;
				std::vector<Face*> facesList;
				for (unsigned int j = 0; j < m->mNumFaces; j++){ //iterating over all faces in this mesh
					importedFaces++;
					facesList.push_back(new Face((*facesPointer).mNumIndices, (*facesPointer).mIndices));
					facesPointer++;
				}
				
				std::vector<glm::vec3> vVec = aiVecToGlmVec3(m->mVertices, m->mNumVertices);
				std::vector<glm::vec3> nVec = aiVecToGlmVec3(m->mNormals, m->mNumVertices);
				std::vector<glm::vec2> uvVec = aiVecToGlmVec2(m->mTextureCoords, m->mNumVertices);
				aiMaterial* material = scene->mMaterials[m->mMaterialIndex];
				bool debug_is_tex = m->mTextureCoords[0];
				Shader* s = aiMaterialToShader(material, debug_is_tex);
				s->useShader();
				Mesh* meshVec = new Mesh(vVec, nVec, facesList, uvVec, s, transform);
				glUseProgram(0);
				meshList.push_back(meshVec);
				meshPointer++;
				importedMeshes++;
			}

			if (printInfo) {
				std::cout << "	Node consists of " << meshList.size() << " meshes with " << importedFaces - facesBefore << " faces." << std::endl;
			}

			std::shared_ptr<Geometry> newObject = std::make_shared<Geometry>(transform, meshList);// TODO 
			
			allGeometries.push_back(newObject);

			if (targetParent != nullptr){
				targetParent->addChild(newObject.get());
			}
			
			// the new object is the parent for all child nodes
			parent = newObject.get();
			
			//transform = glm::mat4(1.0); //TODO <- falsch?
			
		}
		else
		{
			// if no meshes, skip the node, but keep its transformation
			parent = targetParent;

			// TODO if node not const, could transform mTranformation and find node after iteration to apply transformation for light sources
			//node->mTransformation = node->mParent->mTransformation * node->mTransformation;

			// Check if node is light source
			std::vector<aiLight*> temp;
			for (aiLight* l : scene_lights) {
				if (node->mName == l->mName) {
					aiLightToLight(l, transform);
				}
				else {
					temp.push_back(l);
				}
			}
			scene_lights = temp;
			
		}
		
		// continue for all child nodes

		if (printInfo) {
			std::cout << "Number of children: " << node->mNumChildren << std::endl;
			//std::cout << "  Names of children: ";
			for (unsigned int i = 0; i < node->mNumChildren; i++){
				std::cout << node->mChildren[i]->mName.C_Str();
			}
			std::cout << std::endl;

		}
		for (unsigned int i = 0; i < node->mNumChildren; i++){

			if (printInfo) {
				std::cout << "Child " << i << " of " << node->mName.C_Str() << std::endl;
			}
			iterateOverAllNodes(node->mChildren[i], parent, scene, transform);
		}
		
	}
	


	glm::mat4 SceneImporter::assImpMatToGlmMat(const aiMatrix4x4 &m){
		glm::mat4 returnMat;
		CopyaiMat(m, returnMat);
		return returnMat;
	}

	std::vector<glm::vec2> SceneImporter::aiVecToGlmVec2(aiVector3D** inputVectorlist, unsigned int amount){
		std::vector<glm::vec2> result;
		if (inputVectorlist[0]){							// If texture coords exist
			for (unsigned int i = 0; i < amount; i++){
				aiVector3D vec = inputVectorlist[0][i];
				result.push_back(glm::vec2(vec.x, vec.y));
			}
		}
		else {												// Insert (0,0) if no texture coords exist
			for (unsigned int i = 0; i < amount; i++){
				result.push_back(glm::vec2(0.0f, 0.0f));
			}
		}
		return result;
	}

	std::vector<glm::vec3> SceneImporter::aiVecToGlmVec3(aiVector3D* inputVectorlist, unsigned int amount){
		std::vector<glm::vec3> result;
		for (unsigned int i = 0; i < amount; i++){
			aiVector3D vec = (*inputVectorlist);
			result.push_back(glm::vec3(vec.x, vec.y, vec.z));
			inputVectorlist++;
		}
		return result;
	}

	Shader* SceneImporter::aiMaterialToShader(aiMaterial *material, bool is_tex){
		aiColor3D ai_ambient(0, 0, 0);
		aiColor3D ai_specular(0, 0, 0);
		aiColor3D ai_diffuse(0, 0, 0);
		//aiColor3D ai_transparent(0, 0, 0);
		aiColor3D ai_emissive(0, 0, 0);
		int shininess;
		float transparency;

		material->Get(AI_MATKEY_COLOR_AMBIENT, ai_ambient);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_diffuse);
		material->Get(AI_MATKEY_COLOR_SPECULAR, ai_specular);
		//material->Get(AI_MATKEY_COLOR_TRANSPARENT, ai_transparent); // use opacity
		material->Get(AI_MATKEY_OPACITY, transparency); 
		material->Get(AI_MATKEY_COLOR_EMISSIVE, ai_emissive);
		material->Get(AI_MATKEY_SHININESS, shininess);
		
		glm::vec3 ambient = glm::vec3(ai_ambient.r, ai_ambient.g, ai_ambient.b); 
		glm::vec3 specular = glm::vec3(ai_specular.r, ai_specular.g, ai_specular.b);
		glm::vec3 diffuse = glm::vec3(ai_diffuse.r, ai_diffuse.g, ai_diffuse.b);
		//glm::vec3 transparent = glm::vec3(ai_transparent.r, ai_transparent.g, ai_transparent.b);
		if (is_tex) {
			if (printInfo) {
				std::cout << "   TextureCount= " << (material->GetTextureCount(aiTextureType_DIFFUSE)) << std::endl;
			}

			aiString path;
			material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), path);
			if (printInfo) {
				std::cout << "   TexturePath=  " << path.C_Str() << std::endl;
			}
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0){
				std::string pathString = path.C_Str();
				if (pathString.at(0) == *"/" || pathString.at(0) == *"\\"){
					pathString = "./Textures" + pathString;
				}
				else{
					pathString = "./Textures/" + pathString;
				}

				if (ai_emissive.r > 0){ // animated
					//using a trick: Store framerate in red channel of the emissive color value which would be unused otherwise.
					if (printInfo) {
						std::cout << "Importing Animated Texture with framerate=" << ai_emissive.r << std::endl;
					}
					return new AnimatedTextureShader(pathString, ambient, specular, shininess, ai_emissive.r);
				}
				else{ // not animated
					TextureShader* t = new TextureShader(pathString, ambient, specular, shininess);
					return t;
				}
			}
			else{
				TextureShader* t = new TextureShader("./Textures/tiles.jpg", ambient, specular, shininess);
				std::cout << "Warning: Texture without proper path definition found. Using standard path instead..." << std::endl;
				return t;
			}
			
		}
		 //no texture --> BlinnPhong
		if (printInfo) {
			std::cout << "   No texture found, using BlinnPhong: " << ambient.r << "," << ambient.g << "," << ambient.b << ";" << diffuse.r << "," << diffuse.g << "," << diffuse.b << ";" << specular.r << "," << specular.g << "," << specular.b << ";" << shininess << "," << std::endl;
			if (transparency < 1.0f){
				std::cout << "transparent object detected: " << transparency << std::endl;
			}
		}
		Shader* b = new BlinnPhongShader(ambient, diffuse, specular, shininess, transparency);
		return b;
		
	}

	void SceneImporter::aiLightToLight(aiLight *aiL, glm::mat4& node_transform){
		float att_const = aiL->mAttenuationConstant; 
		float att_lin = aiL->mAttenuationLinear;
		float att_quad = aiL->mAttenuationQuadratic;
		aiColor3D ai_col_amb = aiL->mColorAmbient;
		glm::vec3 col_amb = glm::vec3(ai_col_amb.r, ai_col_amb.g, ai_col_amb.b);
		aiColor3D ai_col_diff = aiL->mColorDiffuse;
		glm::vec3 col_diff = glm::vec3(ai_col_diff.r, ai_col_diff.g, ai_col_diff.b);
		aiColor3D ai_col_spec = aiL->mColorSpecular;
		glm::vec3 col_spec = glm::vec3(ai_col_spec.r, ai_col_spec.g, ai_col_spec.b);
		aiVector3D ai_pos = aiL->mPosition;
		glm::vec3 pos = glm::vec3(ai_pos.x, ai_pos.y, ai_pos.z);
		if (printInfo) {
			std::cout << "	Light imported: " << att_const << " attconst, " << att_lin << " att_lin, " << att_quad << " att_quad." << std::endl;
			std::cout << "	   " << col_amb.r << "," << col_amb.g << "," << col_amb.b << "; " << col_diff.r << "," << col_diff.g << "," << col_diff.b << "; " << col_spec.r << "," << col_spec.g << "," << col_spec.b << std::endl;
		}
		//allLights->push_back(std::make_shared<PointLight>(glm::translate(glm::mat4(1.0), pos), glm::max(glm::max(col_amb, col_diff), col_spec), att_const, att_lin, att_quad));
		allLights->push_back(std::make_shared<PointLight>(node_transform*glm::translate(glm::mat4(1.0), pos), col_diff, att_const, att_lin, att_quad));
	}

	void SceneImporter::aiCameraToCamera(aiCamera *c){
		aiMatrix4x4 ai_cam_mat;
		c->GetCameraMatrix(ai_cam_mat);
		glm::mat4& cam_mat = glm::mat4();
		CopyaiMat(ai_cam_mat, cam_mat);
		
		float farplane = c->mClipPlaneFar;
		float nearplane = c->mClipPlaneNear;
		float angle = c->mHorizontalFOV;

		camera = std::make_shared<Camera>(cam_mat);
	}

	// http://ephenationopengl.blogspot.co.at/
	void SceneImporter::CopyaiMat(const aiMatrix4x4 &from, glm::mat4 &to) {
		to[0][0] = from.a1; to[1][0] = from.a2;
		to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2;
		to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2;
		to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2;
		to[2][3] = from.d3; to[3][3] = from.d4;
	}

	void SceneImporter::setPrintInfo(bool i) {
		printInfo = i;
	}

