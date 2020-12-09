#ifndef VECTOR_FIELD_GRAPHER_SHADER_H
#define VECTOR_FIELD_GRAPHER_SHADER_H

#include "cinder/gl/gl.h"

namespace vectorfield {

/**
 * A shader is a piece of code that runs on the GPU. This class is a wrapper for
 * using an openGL shader to make it easier to use
 */
class Shader {
  //Source: https://learnopengl.com/Getting-started/Shaders
 public:
    unsigned int ID;

    /**
     * Takes in file paths to the vertex and fragment shaders parses them
     * Sets up the shaders with openGL and compiles them
     * @param vertexPath - path to vertex shader
     * @param fragmentPath - path to fragment shader
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * Used to indicate that the shader will be used
     */
    void use();

    /**
     * Sends given matrix as uniform to shader
     * @param name - name ID of the uniform that matrix is being sent to
     * @param mat - matrix that the shader should use
     */
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

 private:

  /**
   * Checks for any compile errors in attempting to initialize shaders
   * @param shader - ID to shader that is being checked for compile errors
   * @param type - Type of shader being checked
   */
  void checkCompileErrors(unsigned int shader, std::string type);
};
}

#endif  // VECTOR_FIELD_GRAPHER_SHADER_H
