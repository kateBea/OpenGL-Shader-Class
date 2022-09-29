#ifndef VERTEX_ARRAY_OBJECT_CLASS_HH
#define VERTEX_ARRAY_OBJECT_CLASS_HH

#include <iostream>
#include <cstdint>
#include <array>

#include <GL/glew.h>

#include "vertexBufferObject.h"

template <typename std::size_t array_size>
class VertexArrayObject
{
public:
	VertexArrayObject();

	auto linkVertexArrayObject(VertexBufferObject<array_size>& vbo) -> void;
	auto bind() -> void;
	auto unbind() -> void;

	~VertexArrayObject();

private:
	std::uint32_t m_id{};
	const std::size_t m_components_per_vertex{ 3 };
};

template <typename std::size_t array_size>
inline VertexArrayObject<array_size>::VertexArrayObject()
{
	glGenVertexArrays(1, &this->m_id);
}

template <typename std::size_t array_size>
inline auto VertexArrayObject<array_size>::linkVertexArrayObject(
	VertexBufferObject<array_size>& vbo) -> void
{
	vbo.bind();

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	vbo.unbind();
}

template <typename std::size_t array_size>
inline auto VertexArrayObject<array_size>::bind() -> void
{
	glBindVertexArray(this->m_id);
}

template <typename std::size_t array_size>
inline auto VertexArrayObject<array_size>::unbind() -> void
{
	glBindVertexArray(0);
}

template <typename std::size_t array_size>
inline VertexArrayObject<array_size>::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &this->m_id);
}

#endif	// END VERTEX_ARRAY_OBJECT_CLASS_HH