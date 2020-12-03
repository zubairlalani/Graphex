#include "core/camera.h"
namespace vectorfield {

  Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front_(glm::vec3(0.0f, 0.0f, -1.0f)),
      mouse_sensitivity_(kSensitivity),
      zoom_(kZoom) {
    position_ = position;
    world_up_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
  }

  glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
      if (pitch_ > 89.0f)
        pitch_ = 89.0f;
      if (pitch_ < -89.0f)
        pitch_ = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
  }

  void Camera::ProcessMouseScroll(float yoffset) {
    zoom_ -= (float)yoffset;
    if (zoom_ < 1.0f)
      zoom_ = 1.0f;
    if (zoom_ > 45.0f)
      zoom_ = 45.0f;
  }

  void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right_ = glm::normalize(glm::cross(
        front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up_ = glm::normalize(glm::cross(right_, front_));
  }
}