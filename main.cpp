#include <QApplication>
#include "Login.h"
#include "connection.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("DriveSmart");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("DriveSmart Inc.");

    // Initialisation de la connexion à la base de données Oracle
    Connection c;
    QString connectionStatus;
    bool connected = c.createconnect();
    
    if (connected) {
        // La connexion a réussi
        connectionStatus = "Connexion réussie!\n";
        QMessageBox::information(nullptr, "Connexion réussie", connectionStatus);

    } else {
        connectionStatus = "ERREUR : Impossible de se connecter à Oracle !\n";
        QMessageBox::critical(nullptr, "Erreur de connexion", connectionStatus);
    }

    Login loginWindow;
    loginWindow.show();

    return app.exec();
}
