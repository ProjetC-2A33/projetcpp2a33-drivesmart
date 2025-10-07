#ifndef CONDIDAT_H
#define CONDIDAT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class condidat;
}
QT_END_NAMESPACE

class condidat : public QWidget
{
    Q_OBJECT

public:
    explicit condidat(QWidget *parent = nullptr);
    ~condidat();

private:
    Ui::condidat *ui;
};

#endif // CONDIDAT_H
