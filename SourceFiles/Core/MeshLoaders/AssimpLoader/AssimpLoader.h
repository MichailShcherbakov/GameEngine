#ifndef _ASSIMP_LOADER_H_
#define _ASSIMP_LOADER_H_

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Loader/ILoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

class AssimpLoader : public ILoader
{
public:
	// ILoader
	virtual Meshes Load(const std::string& path) override;
	// ~ILoader

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	Graphics::Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	Meshes m_meshes;
};

#endif // _ASSIMP_LOADER_H_
