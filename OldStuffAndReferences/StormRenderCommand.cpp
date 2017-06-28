#include "StormRenderCommand.h"
#include "StormRenderer.h"
#include "../utils/StormObjectPool.h"

StormObjectPool<StormVertex> _vPool;

StormRenderCommand::StormRenderCommand() {
    _VertexCount = 0;
    _VertexData = nullptr;
    _IndexData = nullptr;
    _Texture  = nullptr;
}

StormRenderCommand::~StormRenderCommand() {
    if (_VertexData) {
        delete[] _VertexData;
        _VertexData = nullptr;
    }
    if (_IndexData) {
        delete[] _IndexData;
        _IndexData = nullptr;
    }
    _Texture = nullptr;
    _VertexCount = 0;
}

void StormRenderCommand::execute(StormRenderer* renderer) {
    renderer->bindTexture(_Texture);
    renderer->bindVertexData(_VertexData, _VertexCount);
    renderer->bindIndexData(_IndexData, _VertexCount);
}

void StormRenderCommand::setTexture(StormTexture* texture) {
    _Texture = texture;
}

void StormRenderCommand::setVertexData(StormVertex* vertices, uint16_t count) {
    _VertexCount = count;
    _VertexData = new StormVertex[count];
    memcpy(_VertexData, vertices, count * sizeof(StormVertex));
}

void StormRenderCommand::setIndexData(uint32_t* indices, uint16_t count) {
    _IndexData = new uint32_t[count];
    memcpy(_IndexData, indices, count * sizeof(uint32_t));
}

void StormRenderCommand::setDefaultIndices() {
    if (!_VertexCount) {
        return;
    }
    _IndexData = new uint32_t[_VertexCount];
    for (uint32_t i = 0; i < (uint32_t)_VertexCount; i++) {
        _IndexData[i] = i;
    }
}