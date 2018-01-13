#include "StormRenderer.h"
#include "../StormCommon.h"

StormRenderer::StormRenderer() : StormModuleBase("StormRenderer2D") {
    _GLVaoId = 0;
    _GLVertexBufferId = 0;
    _GLIndicesBufferId = 0;
    _GLVertexCount = 0;
    _GLTextureSimplerUniform = 0;
    _Shader = nullptr;
    _BindedTexture = nullptr;
    _ViewMatrix.identity();
    _RenderMode = S_RENDER_TRIANGLE_FAN;
    _IsViewChanged = false;
    
    resetColorsOverlay();
}

StormRenderer::~StormRenderer() {
    deinitialize();
}

int StormRenderer::initialize() {
    /* Generate OpenGL buffers  */
    glGenVertexArrays(1, &_GLVaoId);
    glGenBuffers(1, &_GLVertexBufferId);
    glGenBuffers(1, &_GLIndicesBufferId);

    if (!_GLVaoId || !_GLVertexBufferId || !_GLIndicesBufferId) {
        LOG(ERROR) << "Could not generate GL buffers.";
        return -1;
    } else {
        LOG(INFO) << "OpenGL buffers initialized";
    }

    glBindVertexArray(_GLVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, _GLVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _GLIndicesBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 1;
}

void StormRenderer::deinitialize() {
   _Shader = nullptr;
   _ViewMatrix.identity();

   /* Destroy GL buffers */
   if (_GLVaoId != 0) {
       glDeleteVertexArrays(1, &_GLVaoId);
       _GLVaoId = 0;
   }
   if (_GLVertexBufferId != 0) {
       glDeleteBuffers(1, &_GLVertexBufferId);
       _GLVertexBufferId = 0;
   }
   if (_GLVertexBufferId != 0) {
       glDeleteBuffers(1, &_GLIndicesBufferId);
       _GLVertexBufferId = 0;
   }
}

void StormRenderer::setViewMatrix(const Vector2& topLeft, const Vector2& bottomRight, float near /* = -1.0f */, float far /* = 1.0f */) {
    setViewMatrix(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, near, far);
}

void StormRenderer::setViewMatrix(float left, float top, float right, float bottom, float near /* = -1.0f */, float far /* = 1.0f */) {
    Matrix newPerspective = Matrix(
            2.0 / (right - left), 0, 0, 0,
            0, 2.0 / (top - bottom), 0, 0,
            0, 0, -2.0 / (far - near), 0,
            -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1
        );
        
    if (_ViewMatrix != newPerspective) {
        _ViewMatrix = newPerspective;
        _IsViewChanged = true;
    }
}

void StormRenderer::startRendering() {
    _Shader->use();

    /* Bind buffers */
    glBindVertexArray(_GLVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, _GLVertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _GLIndicesBufferId);

    if (_IsViewChanged) {
        _IsViewChanged = false;
        bindViewMatrix();
    }
}

void StormRenderer::endRendering() {
    /* Unbind buffers */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* Unbind texture just in case */
    unbindTexture();
    
    _Shader->unuse();
}

void StormRenderer::begin(StormRenderMode renderMode, bool unbindTextures /* = false */) {
    _GLVertexCount = 0;
    _RenderMode = renderMode;
    
    if (unbindTextures) {
        unbindTexture();
    }

    resetColorsOverlay();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void StormRenderer::draw() {
    if (!_GLVertexCount) {
        return;
    }

    glDrawElements(_RenderMode, _GLVertexCount, GL_UNSIGNED_INT, NULL);
}

int StormRenderer::loadShader(const std::string& name, char* vsData, char* fsData) {
    if (_LoadedShaders.find(name) != _LoadedShaders.end()) {
        /* Shader already loaded */
        LOG(INFO) << "Tryed to load shader '" << name << "' but it was already loaded";
        return 2;
    }

    StormShader* shader = new StormShader();
    if (shader->compileFromSource(vsData, fsData) < 0) {
        LOG(ERROR) << "Could not load shader '" << name << "'";
        delete shader;
        return -1;
    }
    shader->linkShaders();

    _LoadedShaders[name] = shader;

    LOG(INFO) << "Shader '" << name << "' loaded";
    return 1;
}

void StormRenderer::unloadShader(const std::string& name) {
    auto iter = _LoadedShaders.find(name);
    if (iter == _LoadedShaders.end()) {
        return;
    }
    if (iter->second == _Shader) {
        LOG(ERROR) << "Tryed to unload shader '" << name << "' and failed. Shader currently in use.";
        return;
    }
    delete iter->second;
    iter->second = nullptr;
    _LoadedShaders.erase(iter);

    LOG(DEBUG) << "Shader '" << name << "' unloaded";
}

void StormRenderer::setActiveShader(const std::string& name) {
    auto iter = _LoadedShaders.find(name);
    if (iter == _LoadedShaders.end()) {
        LOG(ERROR) << "Tryed to activate non existing shader '" << name << "'";
        return;
    }
    _Shader = iter->second;
    _Shader->use();
    _Shader->bindAttribute("vertexPosition");
    _Shader->bindAttribute("uvCoordinates");
    _Shader->bindAttribute("vertexColor");
    _GLTextureSimplerUniform = _Shader->getUniformLocation("textureUnit");

    bindViewMatrix();
    bindColorUniforms();

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(_GLTextureSimplerUniform, 0);
}

void StormRenderer::bindViewMatrix() {
    if (!_Shader) {
        return;
    }
    _Shader->setUniformMatrix4("viewMatrix", _ViewMatrix);
}

void StormRenderer::bindColorUniforms() {
    if (!_Shader) {
        return;
    }
    _Shader->setUniformColor("colorMultiply", _MultiplyColorOverlay);
    _Shader->setUniformColorNoAlpha("colorAdd", _AddColorOverlay);
}

void StormRenderer::bindTexture(StormTexture* texture) {
    if (_BindedTexture && texture && _BindedTexture->getOpenGLTextureId() == texture->getOpenGLTextureId()) {
        /* Same texture is already binded */
        return;
    }
    _BindedTexture = texture;
    glBindTexture(GL_TEXTURE_2D, _BindedTexture->getOpenGLTextureId());
}

void StormRenderer::unbindTexture() {
    _BindedTexture = nullptr;
    glBindTexture(GL_TEXTURE_2D, 0);
}

void StormRenderer::bindVertexData(StormVertex* vertices, uint32_t count) {
    _GLVertexCount = count;
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(StormVertex), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(StormVertex), (void*)offsetof(StormVertex, position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(StormVertex), (void*)offsetof(StormVertex, uv));
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(StormVertex), (void*)offsetof(StormVertex, color));
}

void StormRenderer::bindIndexData(uint32_t* indices, uint32_t count) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
}

void StormRenderer::setColorMultiply(Color color) {
    if (_MultiplyColorOverlay == color) {
        return;
    }
    _MultiplyColorOverlay = color;
    bindColorUniforms();
}

void StormRenderer::setColorAdd(Color color) {
    if (_AddColorOverlay == color) {
        return;
    }
    _AddColorOverlay = color;
    bindColorUniforms();
}

void StormRenderer::resetColorsOverlay() {
    _AddColorOverlay.set(0, 0, 0, 0);
    _MultiplyColorOverlay.set(255, 255, 255, 255);
    bindColorUniforms();
}

void StormRenderer::setLineWidth(float width) {
    glLineWidth(width);
}
