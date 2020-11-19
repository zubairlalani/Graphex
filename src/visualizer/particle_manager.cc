#include "visualizer/particle_manager.h"

namespace vectorfield {

void ParticleManager::AddParticle(size_t radius) {
  particles_.emplace_back(Particle(radius));
}

void ParticleManager::DrawParticles() {
  ci::gl::color(0, 0, 200);
  /*
  for(Particle& particle : particles_) {
    //std::cout << "POSITION: " << particle.GetPosition() << std::endl;
    vec2 particle_pos = particle.GetPosition();
    ci::gl::drawSolidCircle(particle_pos, particle.GetRadius());
  }*/

  for(int x=0; x<particles_.size(); x++) {
    ci::gl::drawSolidCircle(glm::vec2(particles_[x].GetXPosition(),
                                      particles_[x].GetYPosition()),
                            particles_[x].GetRadius());
  }
}

void ParticleManager::UpdateParticles(const string& i_comp, const string& j_comp) {

  //Calculate each particles velocity based on its current position
  for(Particle& particle : particles_) {
    double x_val = (particle.GetXPosition() - 350)/30;
    double y_val = (particle.GetYPosition() - 350)/30; //Dist from origin then divide by the unit scale
    std::cout << function_handler_.EvaluateFunction(i_comp, j_comp, x_val, y_val) << std::endl;
    particle.SetVelocity(function_handler_.EvaluateFunction(i_comp, j_comp, x_val, y_val));
    //particle.SetVelocity(vec2(2, 1));
    particle.UpdatePosition();
  }
}

void ParticleManager::ClearParticles() {
  particles_.clear();
}

} // namespace vectorfield
