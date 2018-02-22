#pragma once
#include "../utils/StormObjectPool.h"
#include "../utils/StormVertex.h"
#include "../utils/math/Matrix.h"
#include "../StormModuleBase.h"
#include "StormShader.h"
#include "StormTexture.h"
#include <GL/glew.h>
#include <vector>
#include <map>

/* This class queues and executes render commands. */

typedef enum {
    S_RENDER_TRIANGLE_STRIP     = GL_TRIANGLE_STRIP,
    S_RENDER_TRIANGLE_FAN       = GL_TRIANGLE_FAN,
    S_RENDER_TRIANGLES          = GL_TRIANGLES,
    S_RENDER_LINES              = GL_LINES,
    S_RENDER_LINES_LOOP         = GL_LINE_LOOP
} StormRenderMode;

class StormRenderer : public StormModuleBase {
public:
    StormRenderer();
    virtual ~StormRenderer();

    /* Initialize renderer. Create default buffers, shaders and command pools. 
     * Returns < 0 on error. */
    int initialize();

    /* Clear render buffers and clear memory */
    void deinitialize();

    /* Sets render view matrix */
    void setViewMatrix(const Vector2& topLeft, const Vector2& bottomRight, float near = -1.0f, float far = 1.0f);

    /* Sets render view matrix */
    void setViewMatrix(float left, float top, float right, float bottom, float near = -1.0f, float far = 1.0f);

    /* Load and compile shader. Returns < 0 on error
     * @vsData = vertex shader code, @fsData = fragment shader code */
    int loadShader(const std::string& name, char* vsData, char* fsData);

    /* Deletes preloaded shader. 
     * This method wil do nothing if shader '@name' is currently active */
    void unloadShader(const std::string& name);

    /* Sets shader that will be used for rendering */
    void setActiveShader(const std::string& name);

    /* Begin rendering. 
     * Should be called after shader has already been set. */
    void startRendering();

    /* Ends rendering. Unbind buffers and shaders */
    void endRendering();

    /* Should be called before binding data to buffers to initialize stuff */
    void begin(StormRenderMode renderMode, bool unbindTextures = false);

    /* Binds texture to OpenGL buffer */
    void bindTexture(StormTexture* texture);
    
    /* Unbinds currently binded texture (if any) */
    void unbindTexture();

    /* Binds vertex data to buffer */
    void bindVertexData(StormVertex* vertices, uint32_t count);
    
    /* Binds index data to buffer */
    void bindIndexData(uint32_t* indices, uint32_t count);

#ifdef STORM_EDITOR
    /* TODO: IFDEFFED for editor because this function is shit */
    /* Draws line from start to end position */
    void prepareLineVertices(const Vector2& start, const Vector2& end);
#endif

    /* Binds multiply color overlay to shader */
    void setColorMultiply(Color color);

    /* Binds add color overlay to shader */
    void setColorAdd(Color color);

    /* Sets @_MultiplyColorOverlay and @_AddColorOverlay to default values */
    void resetColorsOverlay();

    /* Draws data from buffer to screen */
    void draw();
    
    /* Temporary method used for setting line size when rendering lines.
     * REMAINDER: Delete this methid after primitive rendering 
     * system has been implemented */
    void setLineWidth(float width);

private:
    static const int VERTICES_BUFFER_SIZE = 1000;

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

    /* All currently loaded shaders, maped by their names */
    std::map<std::string, StormShader*> _LoadedShaders;
    
    /* Multiply color overlay */
    Color _MultiplyColorOverlay;

    /* Add color overlay */
    Color _AddColorOverlay;

    /* Currently binded texture */
    StormTexture* _BindedTexture;

    /* Current render mode */
    StormRenderMode _RenderMode;

    /* View matrix. Used for managing virtual screen dimensions */
    Matrix _ViewMatrix;

    /* Buffer used for storing temporary vertices */
    StormVertex _VerticesBuffer[VERTICES_BUFFER_SIZE];
    uint32_t _IndicesBuffer[VERTICES_BUFFER_SIZE];

    /* Set to true if view matrix was changed and uniform should be updated */
    bool _IsViewChanged;
    
    /* Binds @_ViewMatrix matrix to uniform in shader */
    void bindViewMatrix();
    
    /* Binds @_ColorOverlay to uniform in shader */
    void bindColorUniforms();
};
