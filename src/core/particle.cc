#include "core/particle.h"

namespace vectorfield {
Particle::Particle(size_t radius) {
  radius_ = radius;
}

void Particle::SetVelocity(const glm::vec2 &vel) {
  velocity_ = vel;
}

void Particle::UpdatePosition() {
  std::cout <<  "POSITION : " << position_ << std::endl;
  std::cout <<  "VELOCITY : " << velocity_ << std::endl;

  position_ += velocity_;
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
}
