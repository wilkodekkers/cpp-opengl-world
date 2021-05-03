#pragma once
#include <glm/glm.hpp>

class camera
{
	glm::mat4 view_;
	glm::mat4 projection_;
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
public:
	camera();
	camera(int width, int height);
	camera(int width, int height, const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);
	
	glm::mat4 get_view() const;
	glm::mat4 get_projection() const { return projection_; }
	glm::vec3 get_position() const { return position_; }
	glm::vec3 get_front() const { return front_; }
	glm::vec3 get_up() const { return up_; }
	
	void set_position(const glm::vec3 np) { position_ = np; }
	void set_front(const glm::vec3 nf) { front_ = nf; }
};
