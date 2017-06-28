#include "StormShader.h"
#include "../StormCommon.h"
#include <GL/glew.h>

StormShader::StormShader() {
    _AttributesCount = 0;
    _ProgramId = 0;
    _VertexShaderId = 0;
    _FragmentShaderId = 0;
}

StormShader::~StormShader() {
    destroy();
}

int StormShader::compileFromSource(const char* vertexSource, const char* fragmentSource) {
    if (!vertexSource || !fragmentSource) {
        LOG(ERROR) << "Could not compile shader. No source specified";
        return -1;
    }

    _ProgramId = glCreateProgram();

    _VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    if (!_VertexShaderId) {
        LOG(ERROR) << "Vertex shader could not be created.";
        return -1;
    }

    _FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    if (!_FragmentShaderId) {
        LOG(ERROR) << "Fragment shader could not be created.";
        return -1;
    }

    if (compileShader(vertexSource, _VertexShaderId) < 0) {
        LOG(ERROR) << "Vertex shader failed to compile.";
        return -1;
    }
    if (compileShader(fragmentSource, _FragmentShaderId) < 0) {
        LOG(ERROR) << "Fragment shader failed to compile.";
        return -1;
    }

    return 1;
}

void StormShader::linkShaders() {
    glAttachShader(_ProgramId, _VertexShaderId);
    glAttachShader(_ProgramId, _FragmentShaderId);

    glLinkProgram(_ProgramId);

    GLint isLinked = 0;
    glGetProgramiv(_ProgramId, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(_ProgramId, GL_INFO_LOG_LENGTH, &maxLength);

        /* maxLength includes the NULL character */
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(_ProgramId, maxLength, &maxLength, &errorLog[0]);

        /* Don't need the program anymore. */
        glDeleteProgram(_ProgramId);
        glDeleteShader(_VertexShaderId);
        glDeleteShader(_FragmentShaderId);

        LOG(ERROR) << "Could not link shader program.";
        std::printf("%s\n", &(errorLog[0]));
        return;
    }

    glDetachShader(_ProgramId, _VertexShaderId);
    glDetachShader(_ProgramId, _FragmentShaderId);
    glDeleteShader(_VertexShaderId);
    glDeleteShader(_FragmentShaderId);

    LOG(INFO) << "Shader program linked successfully.";
}

void StormShader::use() {
    glUseProgram(_ProgramId);
    for (int i = 0; i < _AttributesCount; i++) {
        glEnableVertexAttribArray(i);
    }
}

void StormShader::unuse() {
    glUseProgram(0);
    for (int i = 0; i < _AttributesCount; i++) {
        glDisableVertexAttribArray(i);
    }
}

int StormShader::bindAttribute(const std::string& attributeName) {
    glBindAttribLocation(_ProgramId, _AttributesCount++, attributeName.c_str());
    return _AttributesCount;
}

int32_t StormShader::getUniformLocation(const std::string& uniformName) const {
    GLint location = glGetUniformLocation(_ProgramId, uniformName.c_str());
    if (location == GL_INVALID_INDEX) {
        LOG(ERROR) << "Uniform " << uniformName << " not found in shader";
        return 0;
    }
    return (int32_t)location;
}

void StormShader::setUniformInt32(const std::string& uniformName, const int32_t value) {
    int32_t location = getUniformLocation(uniformName);
    glUniform1i(location, value);
}

void StormShader::setUniformUint32(const std::string& uniformName, const uint32_t value) {
    int32_t location = getUniformLocation(uniformName);
    glUniform1ui(location, value);
}

void StormShader::setUniformFloat(const std::string& uniformName, const float value) {
    int32_t location = getUniformLocation(uniformName);
    glUniform1f(location, value);
}

void StormShader::setUniformVector2(const std::string& uniformName, const Vector2& value) {
    int32_t location = getUniformLocation(uniformName);
    glUniform2f(location, value.x, value.y);
}

void StormShader::setUniformMatrix4(const std::string& uniformName, const Matrix& value) {
    int32_t location = getUniformLocation(uniformName);
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void StormShader::setUniformColor(const std::string& uniformName, Color& value) {
    int32_t location = getUniformLocation(uniformName);
    glUniform4f(location, value.floatR(), value.floatG(), value.floatB(), value.floatA());
}

void StormShader::destroy() {
    if (!_ProgramId) {
        return;
    }
    glDeleteProgram(_ProgramId);
    _ProgramId = 0;
}

int StormShader::compileShader(const char* source, uint32_t id) {
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        /* Shader failed to compile. Print error. */
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
        glDeleteShader(id); //Don't leak the shader.

        LOG(ERROR) << "Shader compilation error.";
        std::printf("%s\n", &(errorLog[0]));
        return -1;
    }
    
    return 1;
}