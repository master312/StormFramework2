#pragma once
#include "../SWidgetProperty.h"
#include "../../src/core/resources/StormFileSystem.h"

/* This widget opens resource(file) browser with path set to default filesystem specified in StormEngine singleton class.
 * After resource has been selected, smart pointer to StormResourceFile will be created and stored here. */

class SResourceLineEdit;

class SWidgetPropertyResource : public SWidgetProperty {
    Q_OBJECT

public:
    SWidgetPropertyResource(SWidgetComponent* parent = nullptr, const std::string& name = "");

    void setResource(spStormResourceFile resource);

    /* Gets currently selected resource.
     * Returns nullptr if none is selected */
    spStormResourceFile getResource();

    /* Returns resource filename with path relative to filesystem root */
    std::string getResourceFilename() const;
public slots:
    void resourceSelected();

signals:
    /* Fires when resource changes */
    void resourceChanged();

private:
    spStormResourceFile _ResourceFile;
    SResourceLineEdit* _ResourceLineEdit;
};
