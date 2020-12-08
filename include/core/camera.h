#ifndef VECTOR_FIELD_GRAPHER_CAMERA_H
#define VECTOR_FIELD_GRAPHER_CAMERA_H

#include "cinder/gl/gl.h"

namespace vectorfield {

const float YAW         = -90.0f; // Euler angles
const float PITCH       =  0.0f;
const float kSensitivity =  0.1f; // mouse sensitivity
const float kZoom =  45.0f; // initial field of view is 45 degrees

/**
 * Controls all aspects related to the camera in the 3d world
 * Used this source: https://learnopengl.com/Getting-started/Camera
 */
class Camera {

 public:
  float zoom_;

  /**
   *
   * @param position - camera's position in the world
   * @param up - direction that is considered up in the world
   * @param yaw - similar to theta in spherical coordinates - within xy plane
   * @param pitch - rotation above the xy-plane (can think of this as 90 - phi from spherical coordinates)
   */
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

  /**
   * Gets view matrix that converts coordinates from world space to view space (world as seen from camera)
   * @return
   */
  glm::mat4 GetViewMatrix();

  /**
   * Adjust graph position on screen based on mouse movement
   * @param xoffset - how much the mouse's x value has changed
   * @param yoffset - how much the mouse's y value has changed
   * @param constrainPitch - whether or not the amount of pitch should be limit
   *           to certain values (prevents weird camera movement when pitch>=90)
   */
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

  /**
   * When mouse is scrolled, the zoom value increases/decreases
   * @param yoffset - how much the mouse  has been scrolled
   */
  void ProcessMouseScroll(float yoffset);

 private:
  /**
   * Adjusts the camera's front, right, and up vectors based on the camera's position in the world
   */
  void updateCameraVectors();

  // camera Attributes
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f); // location of camera in the world
  glm::vec3 front_; // vector pointing out in front of the camera
  glm::vec3 up_; // vector pointing directly above the camera
  glm::vec3 right_; // vector pointing to the right of the camera
  glm::vec3 world_up_ = glm::vec3(0.0f, 1.0f, 0.0f); // What direction is considered up in the world

  // euler Angles
  float yaw_ = YAW; // Rotation in xy plane (theta in spherical coordinates)
  float pitch_ = PITCH; // Rotation above xy plane (90-phi in spherical coordinates)

  // camera options
  float mouse_sensitivity_; // how sensitive the camera movement should be to the mouse movement

};

}

#endif  // VECTOR_FIELD_GRAPHER_CAMERA_H
