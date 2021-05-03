#pragma once
#include <vector>
#include <GL/glew.h>

struct vertex_buffer_element
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int get_size_of_type(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		default:return 0;
		}
	}
};

class vertex_buffer_layout
{
private:
	std::vector<vertex_buffer_element> m_elements_;
	unsigned int m_stride_;
public:
	vertex_buffer_layout() : m_stride_(0) {}

	template<typename T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements_.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride_ += count * vertex_buffer_element::get_size_of_type(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride_ += count * vertex_buffer_element::get_size_of_type(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride_ += count * vertex_buffer_element::get_size_of_type(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<vertex_buffer_element> get_elements() const { return m_elements_; }
	inline unsigned int get_stride() const { return m_stride_; }
};