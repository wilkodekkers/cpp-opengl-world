#include "VertexBuffer.h"
#include "Renderer.h"

vertex_buffer::vertex_buffer(const void* data, const unsigned size)
{
	glCall(glGenBuffers(1, &m_renderer_id));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
	glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

vertex_buffer::~vertex_buffer()
{
	glCall(glDeleteBuffers(1, &m_renderer_id));
}

void vertex_buffer::bind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
}

void vertex_buffer::unbind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
