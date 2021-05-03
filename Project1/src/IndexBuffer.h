#pragma once

class index_buffer {
private:
	unsigned int m_renderer_id;
	unsigned int m_count;
public:
	index_buffer();
	index_buffer(const unsigned int* data, unsigned int count);
	~index_buffer();

	void bind() const;
	void unbind() const;

	inline unsigned int get_count() const { return m_count; }
};