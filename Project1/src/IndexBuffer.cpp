#include "IndexBuffer.h"
#include "Renderer.h"

index_buffer::index_buffer() : m_count(0)
{
	
}

index_buffer::index_buffer(const unsigned int* data, const unsigned int count) : m_count(count)
{
	glCall(glGenBuffers(1, &m_renderer_id));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

index_buffer::~index_buffer()
{
	glCall(glDeleteBuffers(1, &m_renderer_id));
}

void index_buffer::bind() const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
}

void index_buffer::unbind() const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
