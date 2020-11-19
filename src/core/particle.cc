#include "core/particle.h"

namespace vectorfield {
Particle::Particle(size_t radius, const glm::vec2& position) {
  radius_ = radius;
  position_ = position;
}

void Particle::SetVelocity(const glm::vec2 &vel) {
  velocity_ = vel;
}

void Particle::UpdatePosition() {
  position_ += velocity_;
}

size_t Particle::GetRadius() const {
  return radius_;
}

glm::vec2 Particle::GetVelocity() const {
  return velocity_;
}

glm::vec2 Particle::GetPosition() const {
  return position_;
}
}
