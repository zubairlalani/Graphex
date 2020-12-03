#ifndef VECTOR_FIELD_GRAPHER_CAMERA_H
#define VECTOR_FIELD_GRAPHER_CAMERA_H

#include "cinder/gl/gl.h"

namespace vectorfield {

const float YAW         = -90.0f; // Euler angles
const float PITCH       =  0.0f;
const float kSensitivity =  0.1f; // mouse sensitivity
const float kZoom =  45.0f; // initial field of view is 45 degrees


class Camera {
 public:
  float zoom_;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

  glm::mat4 GetViewMatrix();

  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

  void ProcessMouseScroll(float yoffset);

 private:
  void updateCameraVectors();

  // camera Attributes
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);

  // euler Angles
  float yaw_ = YAW;
  float pitch_ = PITCH;

  // camera options
  float mouse_sensitivity_;

};

}

#endif  // VECTOR_FIELD_GRAPHER_CAMERA_H
