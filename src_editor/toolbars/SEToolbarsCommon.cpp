#include "SEToolbarsCommon.h"

QFont& SEToolbarsCommon::getToolFont() {
    static QFont font;
    font.setPointSize(9);
    font.setItalic(true);
    return font;
}