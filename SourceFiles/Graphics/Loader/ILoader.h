#ifndef _I_LOADER_H_
#define _I_LOADER_H_

#include "Graphics/Mesh/Mesh.h"

struct ILoader
{
	using Meshes = std::vector<Graphics::Mesh>;

	virtual ~ILoader() {}

	virtual Meshes Load(const std::string& path) = 0;
};

#endif