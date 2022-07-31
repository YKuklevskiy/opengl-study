#pragma once

#include <glm/glm.hpp>

const float FOV = 75.0f;
const float INITIAL_YAW = -90.0f;
const float INITIAL_PITCH = 0.0f;
const float SENSITIVITY = 0.25f;
const float CAMERA_SPEED = 4.0f;
const glm::vec3 INITIAL_CAMERA_POSITION(0.0f, 0.0f, 2.0f);
const glm::vec3 INITIAL_LIGHT_POSITION(0.0f, 0.0f, 2.0f);

// chrome
const glm::vec3 AMBIENT_COLOR(0.25f, 0.25f, 0.25f);
const glm::vec3 DIFFUSE_COLOR(0.4f, 0.4f, 0.4f);
const glm::vec3 SPECULAR_COLOR(0.774597f, 0.774597f, 0.774597f);
const float SHININESS = 128 * 0.6f;

const glm::vec3 AMBIENT_LIGHT_COLOR(0.8f, 0.8f, 0.8f);
const glm::vec3 DIFFUSE_LIGHT_COLOR(0.5f, 0.5f, 0.5f);
const glm::vec3 SPECULAR_LIGHT_COLOR(1.0f, 1.0f, 1.0f);