#ifndef VEHICULE_H
#define VEHICULE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class vehicule;
}
QT_END_NAMESPACE
class vehicule : public QMainWindow
{
    Q_OBJECT

public:
    vehicule(QWidget *parent = nullptr);
    ~vehicule();

private:
    Ui::vehicule *ui;
};
#endif // VEHICULE_H
