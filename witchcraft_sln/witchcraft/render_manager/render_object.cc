#include "render_object.h"

RenderObject::RenderObject()
	: position(0.0f,0.0f,0.0f)
	, rotation(0.0f,0.0f,0.0f)
	, scale   (0.0f,0.0f,0.0f)
	, visible(false)
	, texture(nullptr)
{}

RenderObject::~RenderObject()
{}

OpenGLTexture * RenderObject::get_texture() const
{
	return texture;
}

void RenderObject::set_texture(OpenGLTexture & texture)
{
	this->texture = &texture;
}

glm::f32vec3 RenderObject::get_position() const
{
	return position;
}

void RenderObject::set_position(glm::f32vec3 const & pos)
{
	this->position = pos;
}

glm::f32vec3 RenderObject::get_rotation() const
{
	return rotation;
}

void RenderObject::set_rotation(glm::f32vec3 const & rotation)
{
	this->rotation = rotation;
}

glm::f32vec3 RenderObject::get_scale() const
{
	return scale;
}

void RenderObject::set_scale(glm::f32vec3 const & scale)
{
	this->scale = scale;
}

bool RenderObject::is_visible() const
{
	return visible;
}

void RenderObject::set_is_visible(bool is)
{
	visible = is;
}
