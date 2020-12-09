#ifndef VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H
#define VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H

#include <core/function_handler.h>
#include <core/particle.h>

namespace vectorfield {

using std::string;

/**
 * Handles drawing, updating, etc of all the particles on the field
 */
class ParticleManager {

 public:
  /**
   * Draws all the particles onto the screen
   */
  void DrawParticles();

  /**
   * Updates position of every particle based on the velocity field and
   * whether it is returning back to the particle shop or not
   * @param i_comp - i-hat component of vector field
   * @param j_comp - j-hat component of vector field
   */
  void UpdateParticles(const string& i_comp, const string& j_comp);

  /**
   * Adds a particle of the given radius at the given position on the graph
   * @param radius - radius of the drawn particle
   * @param pos - position where particle will be drawn
   */
  void AddParticle(size_t radius, const vec2& pos);

  /**
   * Erases all particles from the screen
   */
  void ClearParticles();

  /**
   * Draws a particle at the given mouse position
   * @param pos - mouse coordinates
   */
  void DrawMouseParticle(const vec2& pos);

  /**
   * Location of particle shop - the place where particles can be picked up
   * so that they can be dropped onto the field
   * @return
   */
  vec2 GetParticleShopPos() const;

  /**
   * Draw the a box around the particle spawner
   */
  void DrawParticleShopBox();

  /**
   * Finds the particle that has been clicked (if any has been clicked) and adds that particle
   * to the list of particles that should return back to the particle shop
   * @param pos - mouse coordinates
   */
  void AddReturnParticle(const vec2& pos);

 private:
  const float kReturnVelocityMultiplier = 7; // Determines how fast the particle should zoom back to the particle shop
  const ci::Color kParticleColor = ci::Color(204, 0, 204); // color of particles
  const size_t kShopBoxDist = 10; // determines how big the particle shop box is
  const vec2 kParticleShopPos = vec2(680, 20); // where the shop is (top left)
  std::vector<Particle> particles_; // holds all particles on the screen that are actively moving through the field
  FunctionHandler function_handler_; // Handles all calculations with vector field that need to be made
  std::vector<Particle> return_particles_; // holds all particles that are on their way back to the particle shop
};
}

#endif  // VECTOR_FIELD_GRAPHER_PARTICLE_MANAGER_H
