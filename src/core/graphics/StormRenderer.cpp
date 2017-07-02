#include "StormRenderer.h"
#include "../StormCommon.h"

StormRenderer::StormRenderer() {
    _GLVaoId = 0;
    _GLVertexBufferId = 0;
    _GLIndicesBufferId = 0;
    _GLVertexCount = 0;
    _GLTextureSimplerUniform = 0;
    _Shader = nullptr;
    _BindedTexture = nullptr;
    _ColorOverlay.set(255, 255, 255, 255);
    _Perspective.identity();
    _RenderMode = S_RENDER_TRIANGLE_FAN;
    _IsPerspectiveChanged = false;
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
   _Perspective.identity();

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

void StormRenderer::setPerspective(float left, float top, float right, float bottom, float near /* = -1.0f */, float far /* = 1.0f */) {
    Matrix newPerspective = Matrix(
            2.0 / (right - left), 0, 0, 0,
            0, 2.0 / (top - bottom), 0, 0,
            0, 0, -2.0 / (far - near), 0,
            -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1
        );
        
    if (_Perspective != newPerspective) {
        _Perspective = newPerspective;
        _IsPerspectiveChanged = true;
    }
}

void StormRenderer::startRendering() {
    _Shader->use();

    /* Bind buffers */
    glBindVertexArray(_GLVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, _GLVertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _GLIndicesBufferId);

    if (_IsPerspectiveChanged) {
        _IsPerspectiveChanged = false;
        bindPerspectiveMatrix();
    }
}

void StormRenderer::endRendering() {
    /* Unbind buffers */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _BindedTexture = nullptr;
    
    _Shader->unuse();
}

void StormRenderer::begin(StormRenderMode renderMode) {
    _GLVertexCount = 0;
    _RenderMode = renderMode;
    
    setColor(Color(255, 255, 255, 255));

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void StormRenderer::draw() {
    if (!_GLVertexCount) {
        return;
    }

    glDrawElements(_RenderMode, _GLVertexCount, GL_UNSIGNED_INT, NULL);
}

void StormRenderer::setShader(StormShader* shader) {
    _Shader = shader;
    _Shader->use();
    _Shader->bindAttribute("vertexPosition");
    _Shader->bindAttribute("uvCoordinates");
    _Shader->bindAttribute("vertexColor");
    _GLTextureSimplerUniform = _Shader->getUniformLocation("textureUnit");

    bindPerspectiveMatrix();
    bindColorUniform();

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(_GLTextureSimplerUniform, 0);
}

void StormRenderer::bindPerspectiveMatrix() {
    if (!_Shader) {
        return;
    }
    _Shader->setUniformMatrix4("perspective", _Perspective);
}

void StormRenderer::bindColorUniform() {
    if (!_Shader) {
        return;
    }
    _Shader->setUniformColor("colorOverlay", _ColorOverlay);
}

void StormRenderer::bindTexture(StormTexture* texture) {
    if (_BindedTexture && texture && _BindedTexture->getOpenGLTextureId() == texture->getOpenGLTextureId()) {
        /* Same texture is already binded */
        return;
    }
    _BindedTexture = texture;
    glBindTexture(GL_TEXTURE_2D, texture->getOpenGLTextureId());
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

void StormRenderer::setColor(Color color) {
    if (_ColorOverlay == color) {
        return;
    }
    _ColorOverlay = color;
    bindColorUniform();
}