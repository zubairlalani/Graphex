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

  void AddParticle(size_t radius, const vec2& pos);

  void ClearParticles();

  void DrawMouseParticle(const vec2& pos);

  vec2 GetParticleShopPos() const;

  void DrawParticleShopBox();

 private:
  const ci::Color kParticleColor = ci::Color(204, 0, 204);
  const size_t kShopBoxDist = 10;
  const vec2 kParticleShopPos = vec2(680, 20);
  std::vector<Particle> particles_;
  FunctionHandler function_handler_;

};
}

#endif  // VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H
