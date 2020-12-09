#include "visualizer/particle_manager.h"

namespace vectorfield {

void ParticleManager::AddParticle(size_t radius, const vec2& pos) {
  particles_.emplace_back(Particle(radius, pos));
}

void ParticleManager::DrawParticles() {
  DrawParticleShopBox();

  ci::gl::color(kParticleColor);
  ci::gl::drawSolidCircle(kParticleShopPos, 5);

  //Draw particles that are moving through the field
  for(int x=0; x<particles_.size(); x++) {
    ci::gl::drawSolidCircle(glm::vec2(particles_[x].GetXPosition(),
                                      particles_[x].GetYPosition()),
                            particles_[x].GetRadius());
  }

  //Draw particles that are returning to particle shop
  for(int x=0; x<return_particles_.size(); x++) {
    ci::gl::drawSolidCircle(glm::vec2(return_particles_[x].GetXPosition(),
                                      return_particles_[x].GetYPosition()),
                            return_particles_[x].GetRadius());
  }


}

void ParticleManager::UpdateParticles(const string& i_comp, const string& j_comp) {

  //Calculate each particles velocity based on its current position
  for(Particle& particle : particles_) {
    //TODO: remove magic numbers
    double x_val = (particle.GetXPosition() - 350)/30;
    double y_val = -1*(particle.GetYPosition() - 350)/30; //Dist from origin then divide by the unit scale

    if (abs(x_val) < 10 && abs(y_val) < 10) {
      vec2 new_velocity = function_handler_.EvaluateFunction(i_comp, j_comp, x_val, y_val);
      particle.SetVelocity(new_velocity);
      particle.UpdatePosition();
    } else {
      particle.SetPosition(particle.GetInitialPos());
    }
  }

  // Updates return particles position
  for(size_t i = 0; i<return_particles_.size(); i++) {
    vec2 dist = vec2(return_particles_[i].GetXPosition() - kParticleShopPos.x,
                     return_particles_[i].GetYPosition() - kParticleShopPos.y);
    double length = glm::length(dist);
    if(length < 10) {
      return_particles_.erase(return_particles_.begin() + i);
      i--;
    } else {
      return_particles_[i].UpdatePosition();
    }
  }
}

void ParticleManager::ClearParticles() {
  particles_.clear();
}

void ParticleManager::DrawMouseParticle(const vec2& pos) {
  ci::gl::color(kParticleColor);
  // Draw a particle where the mouse cursor is
  ci::gl::drawSolidCircle(pos, 5);
}

void ParticleManager::AddReturnParticle(const vec2& pos) {

  for(size_t x = 0; x< particles_.size(); x++) {
    vec2 dist = vec2(pos.x - particles_[x].GetXPosition(),
                     pos.y - particles_[x].GetYPosition());
    double length = glm::length(dist);
    if(length < 10) {
      // Make a vector from the particle to the particle shop
      vec2 dist_to_shop = vec2(kParticleShopPos.x - particles_[x].GetXPosition(),
                               particles_[x].GetYPosition() - kParticleShopPos.y);

      // Calculate the project of the particle velocity vector onto the distance vector
      vec2 proj = (abs(glm::dot(particles_[x].GetVelocity(), dist_to_shop))/glm::length(dist_to_shop)) * dist_to_shop;
      proj/=glm::length(proj);
      vec2 new_velocity = kReturnVelocityMultiplier * proj;

      particles_[x].SetVelocity(new_velocity);
      return_particles_.push_back(particles_[x]);
      particles_.erase(particles_.begin()+x);
    }
  }

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
  ci::gl::end();
}
} // namespace vectorfield
