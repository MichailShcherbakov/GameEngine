#ifndef _MESH_H_
#define _MESH_H_

#include <glm.hpp>
#include <vector>
#include <memory>

namespace Graphics
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	class Material;
	class Mesh
	{
	public:
		void AddVertex(const Vertex& vertex);
		void AddIndex(const uint64_t& index);

	public:
		void AddVertexWithIndex(const Vertex& vertex, const uint64_t& index);

	public:
		void SetPolygonVertices(const std::vector<Vertex>& vertices);
		void SetIndices(const std::vector<uint64_t>& indices);

	public:
		std::vector<Vertex> GetPolygonVertices() const;
		std::vector<uint64_t> GetIndices() const;

	public:
		bool HasIndices() const;

	private:
		std::vector<Vertex> m_vertices;
		std::vector<uint64_t> m_indices;
		std::shared_ptr<Material> m_material;
	};
}

#endif // _MESH_H_