#pragma once
#include "../utils/StormObjectPool.h"
#include "../utils/StormVertex.h"
#include "../utils/math/Matrix.h"
#include "StormShader.h"
#include "StormTexture.h"
#include <GL/glew.h>
#include <vector>

/* This class queues and executes render commands. */

typedef enum StormRenderMode {
    S_RENDER_TRIANGLE_STRIP     = GL_TRIANGLE_STRIP,
    S_RENDER_TRIANGLE_FAN       = GL_TRIANGLE_FAN,
    S_RENDER_TRIANGLES          = GL_TRIANGLES,
};

class StormRenderer {    
public:
    StormRenderer();
    ~StormRenderer();

    /* Initialize renderer. Create default buffers, shaders and command pools. 
     * Returns < 0 on error. */
    int initialize();

    /* Clear render buffers and clear memory */
    void deinitialize();

    /* Sets render perspective */
    void setPerspective(float left, float top, float right, float bottom, float near = -1.0f, float far = 1.0f);

    /* Sets shader that will be used for rendering */
    void setShader(StormShader* shader);

    /* Begin rendering. 
     * Should be called after shader has already been set. */
    void startRendering();

    /* Ends rendering. Unbind buffers and shaders */
    void endRendering();

    /* Should be called before binding data to buffers to initialize stuff */
    void begin(StormRenderMode renderMode);

    /* Binds texture to OpenGL buffer */
    void bindTexture(StormTexture* texture);
    
    /* Binds vertex data to buffer */
    void bindVertexData(StormVertex* vertices, uint32_t count);
    
    /* Binds index data to buffer */
    void bindIndexData(uint32_t* indices, uint32_t count);
    
    /* Binds color overlay to shader */
    void setColor(Color color);

    /* Draws data from buffer to screen */
    void draw();
    
private:
    /* OpenGL VAO index */
    uint32_t _GLVaoId;
    /* OpenGL VBO index */
    uint32_t _GLVertexBufferId;
    /* OpenGL IBO index */
    uint32_t _GLIndicesBufferId;
    
    /* Number of vertices to be rendered */
    uint32_t _GLVertexCount;
    /* OpenGL sampler2D uniform location */
    uint32_t _GLTextureSimplerUniform;

    /* Currently active OpenGL shader program */
    StormShader* _Shader;

    /* Color overlay currently set in shader */
    Color _ColorOverlay;

    /* Currently binded texture */
    StormTexture* _BindedTexture;

    /* Current render mode */
    StormRenderMode _RenderMode;

    /* Perspective matrix. Used for managing virtual screen dimensions */
    Matrix _Perspective;

    /* Set to true if perspective was changed and uniform should be updated */
    bool _IsPerspectiveChanged;

    /* Binds @_Perspective matrix to uniform in shader */
    void bindPerspectiveMatrix();
    /* Binds @_ColorOverlay to uniform in shader */
    void bindColorUniform();
};