#pragma once
#include <iostream>
#include "StormTexture.h"
#include "../StormCommon.h"
#include "../utils/StormVertex.h"

/* INFO: Svaki render commad ima u sebi float vertex[] i float indices[], i gl buffer
    Kad se napravi command updateuje se vertex[] i indices[], i stavljaju se u buffer.
    Kad se executuje command, binduje se buffer, i onda se iscrta.

    ----- HERE ---- 
    - TODO: After testing performance, create object bool for vertex and index data
    -----


    * Ovo ce raditi neako ovako
       -  Prvo se binduju bufferi
       -  Onda se napine kordinatama
       - 

    * Recimo StormRenderCommandPlane
      - StormRenderCommandPlane(Plane* plane)
      
  */

class StormRenderer;

class StormRenderCommand {
    friend class StormRenderer;

public:
    StormRenderCommand();
    virtual ~StormRenderCommand();

    virtual void setTexture(StormTexture* texture);
    virtual void setVertexData(StormVertex* vertices, uint16_t count);
    virtual void setIndexData(uint32_t* indices, uint16_t count);
    /* All currently set vertex data will be indexed in order it was added */
    virtual void setDefaultIndices();
protected:

    StormTexture* _Texture;
    StormVertex* _VertexData;
    uint32_t* _IndexData;
    uint16_t _VertexCount;

    /* GL buffers must be binded before calling this method.
     * This method fill buffers with data, and execute draw call. */
    void execute(StormRenderer* renderer);
};
