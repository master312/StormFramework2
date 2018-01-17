#include "SEObjectsTreeWidget.h"
#include <QTreeWidgetItem>

SEObjectsTreeWidget::SEObjectsTreeWidget(QWidget* parent) : QTreeWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(parent->width(), 200);
    setMaximumSize(2000, 2000);

    QTreeWidgetItem* i = new QTreeWidgetItem(this);
    i->setText(0, "ASD");
    insertTopLevelItem(0, i);
    i->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

    i = new QTreeWidgetItem(this);
    i->setText(0, "ASD DVAAA");
    insertTopLevelItem(1, i);
    i->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

}

SEObjectsTreeWidget::~SEObjectsTreeWidget() {
}

void SEObjectsTreeWidget::populateList(SStormScene* scene) {

}
