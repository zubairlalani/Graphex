#include "core/particle.h"

namespace vectorfield {
Particle::Particle(size_t radius, const glm::vec2& pos) {
  radius_ = radius;
  position_ = pos;
  initial_pos_ = position_;
}

void Particle::SetVelocity(const glm::vec2 &vel) {
  velocity_ = vel;
}

void Particle::UpdatePosition() {

  position_.x += velocity_.x;
  position_.y -= velocity_.y; // coordinates on screen are opposite for y
}

size_t Particle::GetRadius() const {
  return radius_;
}

glm::vec2 Particle::GetVelocity() const {
  return velocity_;
}

double Particle::GetXPosition() const {
  return position_.x;
}

double Particle::GetYPosition() const {
  return position_.y;
}

glm::vec2 Particle::GetInitialPos() const {
  return initial_pos_;
}

void Particle::SetPosition(const glm::vec2 &pos) {
  position_ = pos;
}
}
