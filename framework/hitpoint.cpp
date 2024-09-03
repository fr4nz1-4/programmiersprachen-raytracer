#include "hitpoint.hpp"

HitPoint transform(glm::mat4 const& world_mat, glm::mat4 const& tranposed_inverted_world_mat, HitPoint const& hp) {
	glm::vec4 point{ hp.intersection_point, 1.0f };
	glm::vec4 direction{ hp.direction, 0.0f };
	glm::vec4 normale{ hp.normale , 0.0f };

	point = world_mat * point;
	direction = world_mat * direction;
	normale =glm::normalize(tranposed_inverted_world_mat * normale);

	return HitPoint{hp.intersection, hp.distance, hp.name, hp.material, glm::vec3{point.x, point.y, point.z}, glm::normalize(glm::vec3{direction.x, direction.y, direction.z}), glm::normalize(glm::vec3{normale.x, normale.y, normale.z})};
}