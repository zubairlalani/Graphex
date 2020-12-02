#ifndef VECTOR_FIELD_GRAPHER_SHADER_H
#define VECTOR_FIELD_GRAPHER_SHADER_H

#include "cinder/gl/gl.h"

namespace vectorfield {
class Shader {
 public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

 private:
    void checkCompileErrors(unsigned int shader, std::string type);
};
}

#endif  // VECTOR_FIELD_GRAPHER_SHADER_H
