#pragma once
#include <iostream>
#include "../utils/math/Vector2.h"
#include "../utils/math/Matrix.h"
#include "../utils/math/Color.h"

class StormShader {
public:
    StormShader();
    ~StormShader();
    
    /* Loads shader code from char array and compile it.
     * Returns < 0 on error */
    int compileFromSource(const char* vertexSource, const char* fragmentSource);
    /* Adds an attribute to shader. 
     * Retunrs attribute location. */
    int  bindAttribute(const std::string& attributeName);
    void linkShaders();

    void use();
    void unuse();

    /* Returns uniform location, or 0 of not found */
    int32_t getUniformLocation(const std::string& uniformName) const;

    /* Uniform setter methods */
    void setUniformInt32(const std::string& uniformName, const int32_t value);
    void setUniformUint32(const std::string& uniformName, const uint32_t value);
    void setUniformFloat(const std::string& uniformName, const float value);
    void setUniformVector2(const std::string& uniformName, const Vector2& value);
    void setUniformMatrix4(const std::string& uniformName, const Matrix& value);
    void setUniformColor(const std::string& uniformName, Color& value);

    /* TODO: Create alternative version of this methids like so: 
        void setUniformInt32(int32_t location, const int32_t value);
        void setUniformUint32(int32_t location, const uint32_t value);
        etc...
    */

    /* Destroys shader program */
    void destroy();
private:
    int _AttributesCount;

    uint32_t _ProgramId;
    uint32_t _VertexShaderId;
    uint32_t _FragmentShaderId;

    /* Compiles actual shader. 
     * Return < 0 on error */
    int compileShader(const char* source, uint32_t id);
};