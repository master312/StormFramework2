#pragma once
#include <iostream>
#include "StormTexture.h"
#include "../utils/StormVertex.h"

class StormRenderBatch {
public:
    StormRenderBatch();
    ~StormRenderBatch();

    void setTexture(StormTexture* texture);

    void pushVertices(StormVertex* vertices, uint16_t count);

    void pushIndices(uint32_t* indices, uint16_t count);    
    
    /* Bind index, vertex and texture data to OpenGL buffer.
     * Resets whole batch to default variables.
     * Dose not free any allocated memory. That is done in destructor.
     * WARNING: Index and vertex buffer must be binded before using
     *          this method. Use glBindBuffer() to bind them. */
    //void flushBatch();

    /* Reset all batch data to default values. 
     * Dose not free already allcoated memory */
    void reset();

    StormTexture* getTexture();

    int getVerticesCount();
    int getIndicesCount();
    StormVertex* getVertices();
    uint32_t* getIndices();
public:
    /* How much memory should be prealocated for vertex data */
    static const int ALOC_BLOCK_SIZE = 80;

    StormTexture* _Texture;
    
    /* Array of vertices */
    StormVertex* _Vertices;
    
    /* Size of @_Vertices array*/
    int _VerticesMaxCount;

    /* Count of currently used vertices in @_Vertices array */
    int _VerticesCount;


    /* Array of indices */
    uint32_t* _Indices;

    /* Size of @_Indices array */
    int _IndicesMaxCount;

    /* Count of currently used indices in @_Indices array */
    int _IndicesCount;

    /* Allocate vertices and indices memory block if @requiredSize can not fit
     * into currently allocated arrays. */
    void allocateVerticesBlock(int requiredSize);
    void allocateIndicesBlock(int requiredSize);
};