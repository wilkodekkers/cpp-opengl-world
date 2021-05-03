#include "VertexArray.h"

#include "Renderer.h"

vertex_array::vertex_array()
{
	glCall(glGenVertexArrays(1, &m_renderer_id_));
}

vertex_array::~vertex_array()
{
	glCall(glDeleteVertexArrays(1, &m_renderer_id_));
}

void vertex_array::add_buffer(const vertex_buffer& vb, const vertex_buffer_layout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.get_elements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glCall(glEnableVertexAttribArray(i));
		glCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (const void*)offset));
		offset += element.count * vertex_buffer_element::get_size_of_type(element.type);
	}
}

void vertex_array::bind() const
{
	glCall(glBindVertexArray(m_renderer_id_));
}

void vertex_array::unbind() const
{
	glCall(glBindVertexArray(0));
}
