#include "parkassist/gles/Renderer.h"
#include <GLES2/gl2.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

using namespace nb;

Renderer::Renderer()
	: Renderer(nullptr, nullptr)
{
}

Renderer::Renderer(MeshPtr mesh)
	: Renderer(mesh, nullptr)
{
}

Renderer::Renderer(MeshPtr mesh, ProgramPtr program)
	: m_mesh(mesh)
	, m_program(program)
{
}

void Renderer::setMesh(MeshPtr mesh)
{
	m_mesh = mesh;
}

MeshPtr Renderer::mesh()
{
	return m_mesh;
}

void Renderer::setProgram(ProgramPtr program)
{
	m_program = program;
}

ProgramPtr Renderer::program()
{
	return m_program;
}

void Renderer::draw(CameraPtr camera) const
{
	if (!m_mesh || !m_program)
		return;

	auto &program = m_program;
	program->use();

	//计算后的mvp，以及分开的m/v/p
	auto const &m = m_mesh->matrix;
	auto const &v = camera->viewMatrix();
	auto const &p = camera->projectionMatrix();
	auto mvp = p * v * m;
	program->uniform(program->getUniformLocation(Program::nbMvpStr), mvp);
	program->uniform(program->getUniformLocation(Program::nbMStr), m);
	program->uniform(program->getUniformLocation(Program::nbVStr), v);
	program->uniform(program->getUniformLocation(Program::nbPStr), p);

	//storage中的uniform
	for (auto const &iter : m_uniforms)
	{
		auto location = program->getUniformLocation(iter.first.data());
		auto const &v = iter.second;
		if (v.is_type<int>())							program->uniform(location, v.get_value<int>());
		else if (v.is_type<unsigned int>())				program->uniform(location, (int)v.get_value<unsigned int>());
		else if (v.is_type<short>())					program->uniform(location, (int)v.get_value<short>());
		else if (v.is_type<unsigned short>())			program->uniform(location, (int)v.get_value<unsigned short>());
		else if (v.is_type<long>())						program->uniform(location, (int)v.get_value<long>());
		else if (v.is_type<unsigned long>())			program->uniform(location, (int)v.get_value<unsigned long>());
		else if (v.is_type<float>())					program->uniform(location, v.get_value<float>());
		else if (v.is_type<double>())					program->uniform(location, (float)v.get_value<double>());
		else if (v.is_type<glm::vec2>())				program->uniform(location, v.get_value<glm::vec2>());
		else if (v.is_type<glm::vec3>())				program->uniform(location, v.get_value<glm::vec3>());
		else if (v.is_type<glm::vec4>())				program->uniform(location, v.get_value<glm::vec4>());
		else if (v.is_type<glm::mat2x2>())				program->uniform(location, v.get_value<glm::mat2x2>());
		else if (v.is_type<glm::mat3x3>())				program->uniform(location, v.get_value<glm::mat3x3>());
		else if (v.is_type<glm::mat4x4>())				program->uniform(location, v.get_value<glm::mat4x4>());
		else if (v.is_type<glm::ivec2>())				program->uniform(location, v.get_value<glm::ivec2>());
		else if (v.is_type<glm::ivec3>())				program->uniform(location, v.get_value<glm::ivec3>());
		else if (v.is_type<glm::ivec4>())				program->uniform(location, v.get_value<glm::ivec4>());
		else if (v.is_type<std::vector<int>>())			program->uniform(location, v.get_value<std::vector<int>>());
		else if (v.is_type<std::vector<float>>())		program->uniform(location, v.get_value<std::vector<float>>());
		else if (v.is_type<std::vector<glm::vec2>>())	program->uniform(location, v.get_value<std::vector<glm::vec2>>());
		else if (v.is_type<std::vector<glm::vec3>>())	program->uniform(location, v.get_value<std::vector<glm::vec3>>());
		else if (v.is_type<std::vector<glm::vec4>>())	program->uniform(location, v.get_value<std::vector<glm::vec4>>());
		else if (v.is_type<std::vector<glm::mat2x2>>())	program->uniform(location, v.get_value<std::vector<glm::mat2x2>>());
		else if (v.is_type<std::vector<glm::mat3x3>>())	program->uniform(location, v.get_value<std::vector<glm::mat3x3>>());
		else if (v.is_type<std::vector<glm::mat4x4>>())	program->uniform(location, v.get_value<std::vector<glm::mat4x4>>());
		else if (v.is_type<std::vector<glm::ivec2>>())	program->uniform(location, v.get_value<std::vector<glm::ivec2>>());
		else if (v.is_type<std::vector<glm::ivec3>>())	program->uniform(location, v.get_value<std::vector<glm::ivec3>>());
		else if (v.is_type<std::vector<glm::ivec4>>())	program->uniform(location, v.get_value<std::vector<glm::ivec4>>());
		else											printf("%s is not a supported type for glsl uniform.\n", v.get_type().get_name().data());
	}

	program->vertexAttributePointer(Program::nbPositionLocation, Vertex::positionDimension, Vertex::stride, m_mesh->positionData());
	program->vertexAttributePointer(Program::nbColorLocation, Vertex::colorDimension, Vertex::stride, m_mesh->colorData());
	program->vertexAttributePointer(Program::nbTexCoordLocaltion, Vertex::texCoordDimension, Vertex::stride, m_mesh->textureCoordinateData());
	program->vertexAttributePointer(Program::nbNormalLocation, Vertex::normalDimension, Vertex::stride, m_mesh->normalData());

	glDrawElements(m_mesh->mode, m_mesh->indices.size(), GL_UNSIGNED_SHORT, m_mesh->indices.data());
	program->disuse();
}
