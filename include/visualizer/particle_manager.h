#ifndef VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H
#define VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H

#include <core/function_handler.h>
#include <core/particle.h>

namespace vectorfield {

using std::string;

class ParticleManager {

 public:

  void DrawParticles();

  void UpdateParticles(const string& i_comp, const string& j_comp);

  void AddParticle(size_t radius);

  void ClearParticles();


 private:

  std::vector<Particle> particles_;
  FunctionHandler function_handler_;

};
}

#endif  // VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H
