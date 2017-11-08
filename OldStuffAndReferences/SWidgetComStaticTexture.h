#pragma once
#include "../SWidgetComponent.h"
#include "../../src/core/utils/math/Vector4.h"

class SSceneComSprite;
class SWidgetPropertyResource;
class SWidgetPropertyFloat;

class SWidgetComStaticTexture : public SWidgetComponent {
    Q_OBJECT

public:
    SWidgetComStaticTexture(QWidget* parent = nullptr);

    virtual void initialize();

    /* Usualy called from Property widgets when values ware edited, to update storm component */
    virtual void refresh();

public slots:
    void resourceChanged();

private:
    SSceneComSprite* _TextureComponent;
    SWidgetPropertyResource* _ResourceSelectWidget;
    float _CurrentAlpha;
    SWidgetPropertyFloat* _AlphaControll;
    Vector4 _ColorOverlay;
};
