#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class vertex_array
{
private:
	unsigned int m_renderer_id_;
public:
	vertex_array();
	~vertex_array();

	void add_buffer(const vertex_buffer& vb, const vertex_buffer_layout& layout);
	void bind() const;
	void unbind() const;
};
