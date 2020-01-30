#include "Mesh.h"


void Graphics::Mesh::AddVertex(const Vertex& vertex)
{
	m_vertices.push_back(vertex);
}

void Graphics::Mesh::AddIndex(const uint64_t& index)
{
	m_indices.push_back(index);
}

void Graphics::Mesh::AddVertexWithIndex(const Vertex& vertex, const uint64_t& index)
{
	AddVertex(vertex);
	AddIndex(index);
}

void Graphics::Mesh::SetPolygonVertices(const std::vector<Vertex>& vertices)
{
	m_vertices = vertices;
}

void Graphics::Mesh::SetIndices(const std::vector<uint64_t>& indices)
{
	m_indices = indices;
}

std::vector<Graphics::Vertex> Graphics::Mesh::GetPolygonVertices() const
{
	return m_vertices;
}

std::vector<uint64_t> Graphics::Mesh::GetIndices() const
{
	return m_indices;
}

bool Graphics::Mesh::HasIndices() const
{
	return !m_indices.empty();
}
