#ifndef VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H
#define VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H

#include <core/particle.h>

namespace vectorfield {

class ParticleManager {

 public:

  void DrawParticles();

  void UpdateParticles();

  void AddParticle(size_t radius, const glm::vec2& position);

  void ClearParticles();



 private:

  std::vector<Particle> particles_;


};
}

#endif  // VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H
