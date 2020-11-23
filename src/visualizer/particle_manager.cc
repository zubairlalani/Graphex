#include "visualizer/particle_manager.h"

namespace vectorfield {

void ParticleManager::AddParticle(size_t radius, const vec2& pos) {
  particles_.emplace_back(Particle(radius, pos));
}

void ParticleManager::DrawParticles() {
  DrawParticleShopBox();

  ci::gl::color(kParticleColor);
  ci::gl::drawSolidCircle(kParticleShopPos, 5);

  for(int x=0; x<particles_.size(); x++) {
    ci::gl::drawSolidCircle(glm::vec2(particles_[x].GetXPosition(),
                                      particles_[x].GetYPosition()),
                            particles_[x].GetRadius());
  }
}

void ParticleManager::UpdateParticles(const string& i_comp, const string& j_comp) {

  //Calculate each particles velocity based on its current position
  for(Particle& particle : particles_) {
    //TODO: remove magic numbers
    double x_val = (particle.GetXPosition() - 350)/30;
    double y_val = -1*(particle.GetYPosition() - 350)/30; //Dist from origin then divide by the unit scale

    if(abs(x_val) < 10 && abs(y_val) < 10) {
      vec2 new_velocity = function_handler_.EvaluateFunction(i_comp, j_comp, x_val, y_val);
      particle.SetVelocity(new_velocity);
      particle.UpdatePosition();
    }
  }
}

void ParticleManager::ClearParticles() {
  particles_.clear();
}

void ParticleManager::DrawMouseParticle(const vec2& pos) {
  ci::gl::color(kParticleColor);
  ci::gl::drawSolidCircle(pos, 5);
}

vec2 ParticleManager::GetParticleShopPos() const {
  return kParticleShopPos;
}

void ParticleManager::DrawParticleShopBox() {
  ci::gl::begin(GL_LINE_STRIP);
  ci::gl::color(1, 1, 0);
  ci::gl::lineWidth(5.0);
  ci::gl::vertex(kParticleShopPos.x-kShopBoxDist, kParticleShopPos.y-kShopBoxDist); // top left
  ci::gl::vertex(kParticleShopPos.x-kShopBoxDist, kParticleShopPos.y+kShopBoxDist); // bottom left
  ci::gl::vertex(kParticleShopPos.x+kShopBoxDist, kParticleShopPos.y+kShopBoxDist); // bottom right
  ci::gl::vertex(kParticleShopPos.x+kShopBoxDist, kParticleShopPos.y-kShopBoxDist); // top right
  //ci::gl::vertex(kParticleShopPos.x-kShopBoxDist, kParticleShopPos.y-kShopBoxDist); // top left
  ci::gl::end();
}
} // namespace vectorfield
