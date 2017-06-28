#include "StormRenderBatch.h"

StormRenderBatch::StormRenderBatch() {
    _Texture = nullptr;
    _Vertices = nullptr;
    _VerticesCount = 0;
    _VerticesMaxCount = 0;
    _Indices = nullptr;
    _IndicesCount = 0;
    _IndicesMaxCount = 0;
}

StormRenderBatch::~StormRenderBatch() {
    if (_Vertices) {
        delete[] _Vertices;
        _Vertices = nullptr;
    }
    if (_Indices) {
        delete[] _Indices;
        _Indices = nullptr;
    }
    _VerticesCount = 0;
    _VerticesMaxCount = 0;
    _IndicesCount = 0;
    _IndicesMaxCount = 0;
    _Texture = nullptr;
}

void StormRenderBatch::setTexture(StormTexture* texture) {
    _Texture = texture;
}

void StormRenderBatch::pushVertices(StormVertex* vertices, uint16_t count) {
    allocateVerticesBlock(count);
    memcpy(_Vertices + _VerticesCount, vertices, count * sizeof(StormVertex));
    _VerticesCount += count;
}

void StormRenderBatch::pushIndices(uint32_t* indices, uint16_t count) {
    allocateIndicesBlock(count);
    memcpy(_Indices + _IndicesCount, indices, count * sizeof(uint32_t));
    for (int i = _IndicesCount; i < _IndicesCount + count; i++) {
        _Indices[i] += _IndicesCount;
        std::cout << "I " << i << " VAL " << _Indices[i] << std::endl;
        std::cout << "V " << i << " VAL X " << _Vertices[i].position.x << " Y " << _Vertices[i].position.y << std::endl;
    }
    _IndicesCount += count;
}

void StormRenderBatch::reset() {
    _VerticesCount = 0;
    _IndicesCount = 0;
    _Texture = nullptr;
}

StormTexture* StormRenderBatch::getTexture() {
    return _Texture;    
}

int StormRenderBatch::getVerticesCount() {
    return _VerticesCount;
}

int StormRenderBatch::getIndicesCount() {
    return _IndicesCount;
}

StormVertex* StormRenderBatch::getVertices() {
    return _Vertices;
}

uint32_t* StormRenderBatch::getIndices() {
    return _Indices;
}

void StormRenderBatch::allocateVerticesBlock(int requiredSize) {
    if (!_Vertices) {
        _Vertices = new StormVertex[ALOC_BLOCK_SIZE];
        _VerticesMaxCount = ALOC_BLOCK_SIZE;
        allocateVerticesBlock(requiredSize);
        return;
    } else if (!_VerticesCount) {
        return;
    }

    if (requiredSize + _VerticesCount < _VerticesMaxCount) {
        return;
    }

    int newSize = _VerticesMaxCount + ALOC_BLOCK_SIZE;
    StormVertex* newArray = new StormVertex[newSize];
    
    memcpy(newArray, _Vertices, _VerticesCount);
    
    _VerticesMaxCount = newSize;
    
    delete[] _Vertices;
    _Vertices = newArray;
}

void StormRenderBatch::allocateIndicesBlock(int requiredSize) {
    if (!_Indices) {
        _Indices = new uint32_t[ALOC_BLOCK_SIZE];
        _IndicesMaxCount = ALOC_BLOCK_SIZE;
        allocateIndicesBlock(requiredSize);
        return;
    } else if (!_IndicesCount) {
        return;
    }

    if (requiredSize + _IndicesCount < _IndicesMaxCount) {
        return;
    }

    int newSize = _IndicesMaxCount + ALOC_BLOCK_SIZE;
    uint32_t* newArray = new uint32_t[newSize];
    
    memcpy(newArray, _Indices, _IndicesCount);    
    
    delete[] _Indices;
    _Indices = newArray;
    _IndicesMaxCount = newSize;
}