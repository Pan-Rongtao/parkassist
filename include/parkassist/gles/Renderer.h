/*******************************************************
**	Renderer
**
**	渲染物
**	
**	渲染物是描述可渲染对象的数据结构
**
**	一般而言，渲染物都有自己的模型结构、材质等
**		
**		
**		潘荣涛
**	
********************************************************/
#pragma once
#include <string>
#include <map>
#include "glm/glm.hpp"
#include "parkassist/gles/Def.h"
#include "parkassist/gles/Any.h"

struct aiNode;
struct aiMesh;
struct aiScene;
namespace nb{
	
using var = Any;

class Mesh;
class Program;
class Camera;
using MeshPtr = std::shared_ptr<Mesh>;
using ProgramPtr = std::shared_ptr<Program>;
using CameraPtr = std::shared_ptr<Camera>;

class NB_API Renderer
{
public:
	//构建一个空的Renderer
	Renderer();

	//构建一个空的Renderer
	Renderer(MeshPtr mesh);

	//构建一个Renderer，它的模型为model，程序为program
	Renderer(MeshPtr mesh, ProgramPtr program);

	//模型
	void setMesh(MeshPtr mesh);
	MeshPtr mesh();

	//设置program
	void setProgram(ProgramPtr program);

	//获取program
	ProgramPtr program();

	//存储uniform变量，以便下次刷新使用
	template<class T>
	void storeUniform(const std::string &name, const T &v)
	{
		static_assert(
			std::is_same<T, short>::value ||
			std::is_same<T, unsigned short>::value ||
			std::is_same<T, int>::value ||
			std::is_same<T, unsigned int>::value ||
			std::is_same<T, long>::value ||
			std::is_same<T, unsigned long>::value ||
			std::is_same<T, float>::value ||
			std::is_same<T, double>::value ||
			std::is_same<T, glm::vec2>::value ||
			std::is_same<T, glm::vec3>::value ||
			std::is_same<T, glm::vec4>::value ||
			std::is_same<T, glm::ivec2>::value ||
			std::is_same<T, glm::ivec3>::value ||
			std::is_same<T, glm::ivec4>::value ||
			std::is_same<T, glm::mat2x2>::value ||
			std::is_same<T, glm::mat3x3>::value ||
			std::is_same<T, glm::mat4x4>::value ||

			std::is_same<T, std::vector<float>>::value ||
			std::is_same<T, std::vector<glm::vec2>>::value ||
			std::is_same<T, std::vector<glm::vec3>>::value ||
			std::is_same<T, std::vector<glm::vec4>>::value ||
			std::is_same<T, std::vector<int>>::value ||
			std::is_same<T, std::vector<glm::ivec2>>::value ||
			std::is_same<T, std::vector<glm::ivec3>>::value ||
			std::is_same<T, std::vector<glm::ivec4>>::value ||
			std::is_same<T, std::vector<glm::mat2x2>>::value ||
			std::is_same<T, std::vector<glm::mat3x3>>::value ||
			std::is_same<T, std::vector<glm::mat4x4>>::value
			, "only support type[short, int, long, float, double, vec2, vec3, vec4, mat2x2, mat3x3, mat4x4 or their vector types].");

		m_uniforms[name] = v;
	}
	
	//绘制，重写此方法以构建自己的渲染方式
	virtual void draw(CameraPtr camera) const;

private:
	MeshPtr		m_mesh;
	ProgramPtr	m_program;
	std::map<std::string, var>	m_uniforms;
};

}
