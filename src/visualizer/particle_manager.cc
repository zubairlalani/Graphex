#include "visualizer/particle_manager.h"

namespace vectorfield {

void ParticleManager::AddParticle(size_t radius, const glm::vec2& position) {
  particles_.emplace_back(Particle(radius, position));
}

void ParticleManager::DrawParticles() {
  ci::gl::color(0, 0, 200);
  for(const Particle& particle : particles_) {
    ci::gl::drawSolidCircle(particle.GetPosition(), particle.GetRadius());
  }
}

void ParticleManager::UpdateParticles() {

  //Calculate each particles velocity based on its current position
  for(Particle& particle : particles_) {
    particle.SetVelocity(glm::vec2(2, 1));
    particle.UpdatePosition();
  }

}

void ParticleManager::ClearParticles() {
  particles_.clear();
}

} // namespace vectorfield
