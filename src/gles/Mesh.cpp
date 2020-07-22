#include "parkassist/gles/Mesh.h"
#include <GLES2/gl2.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/ext/matrix_projection.hpp>
#include "parkassist/gles/Material.h"

using namespace nb;

//class Vertex
Vertex::Vertex()
{
}

Vertex::Vertex(const glm::vec3 & position)
	: Vertex(position, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(), glm::vec3())
{
}

Vertex::Vertex(const glm::vec3 & position, const glm::vec4 & color)
	: Vertex(position, color, glm::vec2(), glm::vec3())
{
}

Vertex::Vertex(const glm::vec3 & position, const glm::vec2 & texCoord)
	: Vertex(position, glm::vec4(0.0, 0.0, 0.0, 1.0), texCoord, glm::vec3())
{
}

Vertex::Vertex(const glm::vec3 & position, const glm::vec4 & color, const glm::vec2 & texCoord)
	: Vertex(position, color, texCoord, glm::vec3())
{
}

Vertex::Vertex(const glm::vec3 & position, const glm::vec4 & color, const glm::vec2 & texCoord, const glm::vec3 & normal)
	: position(position)
	, color(color)
	, texCoord(texCoord)
	, normal(normal)
{
}

//class Mesh
Mesh::Mesh()
	: Mesh({}, {})
{
}

Mesh::Mesh(const std::vector<Vertex>& vertexs, const std::vector<uint16_t>& indices)
	: vertexs(vertexs)
	, indices(indices)
	, matrix(1.0f)
	, mode(GL_TRIANGLES)
{
}

Mesh::Mesh(const std::vector<Vertex> &vertexs, const std::vector<uint16_t> &indices, const Material &materia)
	: vertexs(vertexs)
	, indices(indices)
	, material(materia)
	, matrix(1.0f)
{
}

Mesh::Mesh(const Mesh & other)
	: vertexs(other.vertexs)
	, indices(other.indices)
	, material(other.material)
	, matrix(other.matrix)
{
}

Mesh::Mesh(const Mesh && other)
	: vertexs(std::move(other.vertexs))
	, indices(std::move(other.indices))
	, material(std::move(other.material))
	, matrix(std::move(other.matrix))
{

}

void Mesh::operator = (const Mesh &other)
{
	vertexs = other.vertexs;
	indices = other.indices;
	material = other.material;
	matrix = other.matrix;
}

void Mesh::operator = (const Mesh &&other)
{
	vertexs = std::move(other.vertexs);
	indices = std::move(other.indices);
	material = std::move(other.material);
	matrix = std::move(other.matrix);
}

float *Mesh::positionData()
{
	return !vertexs.empty() ? glm::value_ptr(vertexs[0].position) : nullptr;
}

const float *Mesh::positionData() const
{
	return const_cast<Mesh *>(this)->positionData();
}

float *Mesh::colorData()
{
	return !vertexs.empty() ? glm::value_ptr(vertexs[0].color) : nullptr;
}

const float *Mesh::colorData() const
{
	return const_cast<Mesh *>(this)->colorData();
}

float *Mesh::textureCoordinateData()
{
	return !vertexs.empty() ? glm::value_ptr(vertexs[0].texCoord) : nullptr;
}

const float *Mesh::textureCoordinateData() const
{
	return const_cast<Mesh *>(this)->textureCoordinateData();
}

float *Mesh::normalData()
{
	return !vertexs.empty() ? glm::value_ptr(vertexs[0].normal) : nullptr;
}

const float *Mesh::normalData() const
{
	return const_cast<Mesh *>(this)->normalData();
}

void Mesh::unifyColor(const glm::vec4 &color)
{
	for (auto &vertex : vertexs)
		vertex.color = color;
}
