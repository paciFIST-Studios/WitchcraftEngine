#ifndef VERTEX_RESOURCE_H
#define VERTEX_RESOURCE_H

#include <vector>

#include "engine_resource_base.h"

// NOTE: this is only configured for: 
// {x, y, u, v} for each corner of the quad
// , supplied as floats
class VertexResource : public EngineResourceBase
{
private:
protected:
public:
	std::vector<float> const vertex_list;
	std::vector<int> const index_list;
	int const vertex_stride;
	int const vertex_offset;
	int const texture_stride;
	int const texture_offset;

	VertexResource() 
		: EngineResourceBase()
		, vertex_stride(0)
		, vertex_offset(0)
		, texture_stride(0)
		, texture_offset(0)
	{}

	VertexResource(
		  std::string const & name
		, std::string const & file
		, EResourceType type
		, int scope
		, std::vector<float> & vertex_array
		, std::vector<int> & index_array
		, int vert_stride
		, int vert_offset
		, int tex_stride
		, int tex_offset
	)
		: EngineResourceBase(name, file, type, scope)
		, vertex_list(vertex_array)
		, index_list(index_array)
		, vertex_stride(vert_stride)
		, vertex_offset(vert_offset)
		, texture_stride(tex_stride)
		, texture_offset(tex_offset)
	{}

	~VertexResource() override {}
	void load() override {}
	void unload() override {}
};

#endif // !VERTEX_RESORUCE_H
