#ifndef SELECTPREFAB_H
#define SELECTPREFAB_H

#include <QDialog>

namespace Ui {
class SelectPrefab;
}

class SelectPrefab : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPrefab(QWidget *parent = 0);
    ~SelectPrefab();

public slots:
    void prefabSelectedToInstantiate();

private:
    Ui::SelectPrefab *ui;
};

#endif // SELECTPREFAB_H
