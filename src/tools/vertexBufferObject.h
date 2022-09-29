#ifndef VERTEX_BUFFER_OBJECT_CLASS_HH
#define VERTEX_BUFFER_OBJECT_CLASS_HH

#include <iostream>
#include <cstdint>
#include <array>

#include <GL/glew.h>

template <typename std::size_t array_size>
class VertexBufferObject
{
public:
	VertexBufferObject(const std::array<float, array_size>& vertices);
	auto bind() -> void;
	auto unbind() -> void;
	auto getBufferSize() const -> std::size_t;

	~VertexBufferObject();

private:
	std::uint32_t m_id{};
	std::size_t m_buffer_size{};
};

template<typename std::size_t array_size>
inline VertexBufferObject<array_size>::VertexBufferObject(const std::array<float, array_size>& vertices)
	:	m_buffer_size{ vertices.size() * sizeof(float) }
{
	glGenBuffers(1, &this->m_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
	glBufferData(GL_ARRAY_BUFFER, m_buffer_size, vertices.data(), GL_STATIC_DRAW);
}

template<typename std::size_t array_size>
inline auto VertexBufferObject<array_size>::bind() -> void
{
	glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
}

template<typename std::size_t array_size>
inline auto VertexBufferObject<array_size>::unbind() -> void
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


template<typename std::size_t array_size>
inline auto VertexBufferObject<array_size>::getBufferSize() const -> std::size_t
{
	return this->m_buffer_size;
}

template<typename std::size_t array_size>
inline VertexBufferObject<array_size>::~VertexBufferObject()
{
	glDeleteBuffers(1, &this->m_id);
}


#endif	// END VERTEX_BUFFER_OBJECT_CLASS_HH