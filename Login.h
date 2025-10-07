#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class Login;
}

class MainWindow; // Forward declaration

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_signInButton_clicked();
    void on_forgotpButton_clicked();

private:
    Ui::Login *ui;
    MainWindow *mainWindow; // Add this line
};

#endif
