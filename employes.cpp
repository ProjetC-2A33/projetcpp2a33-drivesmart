// employes.cpp
#include "employes.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMap>
#include <QVariant>

// === Constructeur par défaut ===
Employes::Employes()
    : disponibilite(false), salaire(0.0)
{
}

// === Constructeur complet ===
Employes::Employes(const QString &cin_employee,
                   const QString &nom,
                   const QString &prenom,
                   bool disponibilite,
                   double salaire,
                   const QString &telephone,
                   const QString &email,
                   const QString &poste,
                   const QString &mdp)
    : cin_employee(cin_employee),
    nom(nom),
    prenom(prenom),
    disponibilite(disponibilite),
    salaire(salaire),
    telephone(telephone),
    email(email),
    poste(poste),
    mdp(mdp)
{
}

// === Afficher tous les employés (sans MDP) ===
QSqlQueryModel* Employes::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    qDebug() << "Chargement des données de la table EMPLOYEE...";

    QSqlQuery query;
    query.prepare(
        "SELECT CIN_EMPLOYEE, NOM, PRENOM, "
        "DISPONIBILITE, "
        "SALAIRE, TELEPHONE, EMAIL, POSTE "
        "FROM EMPLOYEE ORDER BY CIN_EMPLOYEE"
        );
    
    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        delete model;
        return nullptr;
    }
    
    model->setQuery(std::move(query));
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur modèle SQL :" << model->lastError().text();
        delete model;
        return nullptr;
    }
    
    // Force data fetch
    while (model->canFetchMore()) {
        model->fetchMore();
    }

    // En-têtes
    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Disponibilité");
    model->setHeaderData(4, Qt::Horizontal, "Salaire");
    model->setHeaderData(5, Qt::Horizontal, "Téléphone");
    model->setHeaderData(6, Qt::Horizontal, "Email");
    model->setHeaderData(7, Qt::Horizontal, "Poste");

    qDebug() << "Chargés :" << model->rowCount() << "employés";
    return model;
}

// === Ajouter un employé ===
bool Employes::ajouter()
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO EMPLOYEE (CIN_EMPLOYEE, NOM, PRENOM, DISPONIBILITE, SALAIRE, "
        "TELEPHONE, EMAIL, POSTE, MDP) "
        "VALUES (:cin, :nom, :prenom, :dispo, :salaire, :tel, :email, :poste, :mdp)"
        );

    query.bindValue(":cin", cin_employee);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dispo", disponibilite ? "Disponible" : "Non disponible");
    query.bindValue(":salaire", salaire);
    query.bindValue(":tel", telephone);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":mdp", mdp);  // À hasher avant appel

    if (!query.exec()) {
        qDebug() << "Erreur ajout :" << query.lastError().text();
        return false;
    }
    return true;
}

// === Supprimer ===
bool Employes::supprimer(const QString &cin_employee)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYEE WHERE CIN_EMPLOYEE = :cin");
    query.bindValue(":cin", cin_employee);
    return query.exec();
}

// === Modifier ===
bool Employes::modifier()
{
    QSqlQuery query;
    query.prepare(
        "UPDATE EMPLOYEE SET NOM=:nom, PRENOM=:prenom, DISPONIBILITE=:dispo, "
        "SALAIRE=:salaire, TELEPHONE=:tel, EMAIL=:email, POSTE=:poste, MDP=:mdp "
        "WHERE CIN_EMPLOYEE=:cin"
        );
    query.bindValue(":cin", cin_employee);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dispo", disponibilite ? "Disponible" : "Non disponible");
    query.bindValue(":salaire", salaire);
    query.bindValue(":tel", telephone);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":mdp", mdp);

    return query.exec();
}

// Overloaded modifier to support CIN change
bool Employes::modifier(const QString &originalCIN)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE EMPLOYEE SET CIN_EMPLOYEE=:newcin, NOM=:nom, PRENOM=:prenom, DISPONIBILITE=:dispo, "
        "SALAIRE=:salaire, TELEPHONE=:tel, EMAIL=:email, POSTE=:poste, MDP=:mdp "
        "WHERE CIN_EMPLOYEE=:oldcin"
        );
    query.bindValue(":newcin", cin_employee);
    query.bindValue(":oldcin", originalCIN);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dispo", disponibilite ? "Disponible" : "Non disponible");
    query.bindValue(":salaire", salaire);
    query.bindValue(":tel", telephone);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "Error modifying employee:" << query.lastError().text();
        return false;
    }
    return true;
}

// === Vérifier unicité CIN ===
bool Employes::cinExiste(const QString &cin, const QString &cinActuel)
{
    QSqlQuery query;
    query.prepare("SELECT 1 FROM EMPLOYEE WHERE CIN_EMPLOYEE = :cin");
    query.bindValue(":cin", cin);
    if (!query.exec() || !query.next()) return false;
    return (cin == cinActuel) ? false : true;
}

// === Vérifier unicité Email ===
bool Employes::emailExiste(const QString &email, const QString &cinActuel)
{
    QSqlQuery query;
    query.prepare("SELECT 1 FROM EMPLOYEE WHERE EMAIL = :email AND CIN_EMPLOYEE != :cin");
    query.bindValue(":email", email);
    query.bindValue(":cin", cinActuel);
    return query.exec() && query.next();
}




// Ajoutez cette méthode à la fin du fichier, avant les autres méthodes de tri/statistiques

QSqlQueryModel* Employes::rechercherMulti(const QString &query)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    qDebug() << "Recherche multi-champs avec :" << query;

    QSqlQuery sqlQuery;
    sqlQuery.prepare(
        "SELECT CIN_EMPLOYEE, NOM, PRENOM, "
        "DISPONIBILITE, "
        "SALAIRE, TELEPHONE, EMAIL, POSTE "
        "FROM EMPLOYEE "
        "WHERE UPPER(CIN_EMPLOYEE) LIKE UPPER(:query) "
        "OR UPPER(NOM) LIKE UPPER(:query) "
        "OR UPPER(PRENOM) LIKE UPPER(:query)"
        );
    sqlQuery.bindValue(":query", "%" + query + "%");

    if (!sqlQuery.exec()) {
        qDebug() << "Erreur recherche multi :" << sqlQuery.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(sqlQuery));

    // En-têtes
    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Disponibilité");
    model->setHeaderData(4, Qt::Horizontal, "Salaire");
    model->setHeaderData(5, Qt::Horizontal, "Téléphone");
    model->setHeaderData(6, Qt::Horizontal, "Email");
    model->setHeaderData(7, Qt::Horizontal, "Poste");

    qDebug() << "Résultats trouvés multi-champs :" << model->rowCount();
    return model;
}



QSqlQueryModel* Employes::trierParNomCroissant()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    qDebug() << "Tri des employés par nom (ordre croissant)...";

    QSqlQuery query;
    query.prepare(
        "SELECT CIN_EMPLOYEE, NOM, PRENOM, "
        "DISPONIBILITE, "
        "SALAIRE, TELEPHONE, EMAIL, POSTE "
        "FROM EMPLOYEE "
        "ORDER BY NOM ASC"
        );
    
    if (!query.exec()) {
        qDebug() << "Erreur tri nom:" << query.lastError().text();
        delete model;
        return nullptr;
    }
    
    model->setQuery(std::move(query));
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur modèle tri nom:" << model->lastError().text();
        delete model;
        return nullptr;
    }
    
    while (model->canFetchMore()) {
        model->fetchMore();
    }

    // En-têtes
    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Disponibilité");
    model->setHeaderData(4, Qt::Horizontal, "Salaire");
    model->setHeaderData(5, Qt::Horizontal, "Téléphone");
    model->setHeaderData(6, Qt::Horizontal, "Email");
    model->setHeaderData(7, Qt::Horizontal, "Poste");

    qDebug() << "Tri par nom effectué :" << model->rowCount() << "employés";
    return model;
}

QSqlQueryModel* Employes::trierParPrenomAlphabetique()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    qDebug() << "Tri des employés par prénom (ordre alphabétique)...";

    QSqlQuery query;
    query.prepare(
        "SELECT CIN_EMPLOYEE, NOM, PRENOM, "
        "DISPONIBILITE, "
        "SALAIRE, TELEPHONE, EMAIL, POSTE "
        "FROM EMPLOYEE "
        "ORDER BY PRENOM ASC"
        );
    
    if (!query.exec()) {
        qDebug() << "Erreur tri prénom:" << query.lastError().text();
        delete model;
        return nullptr;
    }
    
    model->setQuery(std::move(query));
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur modèle tri prénom:" << model->lastError().text();
        delete model;
        return nullptr;
    }
    
    while (model->canFetchMore()) {
        model->fetchMore();
    }

    // En-têtes
    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Disponibilité");
    model->setHeaderData(4, Qt::Horizontal, "Salaire");
    model->setHeaderData(5, Qt::Horizontal, "Téléphone");
    model->setHeaderData(6, Qt::Horizontal, "Email");
    model->setHeaderData(7, Qt::Horizontal, "Poste");

    qDebug() << "Tri par prénom effectué :" << model->rowCount() << "employés";
    return model;
}

QSqlQueryModel* Employes::trierParSalaire()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    qDebug() << "Tri des employés par salaire...";

    QSqlQuery query;
    query.prepare(
        "SELECT CIN_EMPLOYEE, NOM, PRENOM, "
        "DISPONIBILITE, "
        "SALAIRE, TELEPHONE, EMAIL, POSTE "
        "FROM EMPLOYEE "
        "ORDER BY SALAIRE ASC"
        );
    
    if (!query.exec()) {
        qDebug() << "Erreur tri salaire:" << query.lastError().text();
        delete model;
        return nullptr;
    }
    
    model->setQuery(std::move(query));
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur modèle tri salaire:" << model->lastError().text();
        delete model;
        return nullptr;
    }
    
    while (model->canFetchMore()) {
        model->fetchMore();
    }

    // En-têtes
    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Disponibilité");
    model->setHeaderData(4, Qt::Horizontal, "Salaire");
    model->setHeaderData(5, Qt::Horizontal, "Téléphone");
    model->setHeaderData(6, Qt::Horizontal, "Email");
    model->setHeaderData(7, Qt::Horizontal, "Poste");

    qDebug() << "Tri par salaire effectué :" << model->rowCount() << "employés";
    return model;
}


// === Statistiques de disponibilité ===
QMap<QString, QVariant> Employes::getStatistiquesDisponibilite()
{
    QMap<QString, QVariant> statistiques;
    int disponibles = 0;
    int nonDisponibles = 0;
    int totalEmployes = 0;

    QSqlQuery query;
    query.prepare("SELECT DISPONIBILITE, COUNT(*) FROM EMPLOYEE GROUP BY DISPONIBILITE");

    if (query.exec()) {
        while (query.next()) {
            QString dispoStr = query.value(0).toString();
            int count = query.value(1).toInt();

            if (dispoStr == "Disponible") {
                disponibles = count;
            } else {
                nonDisponibles = count;
            }
            totalEmployes += count;
        }

        // Calcul des pourcentages
        double pourcentageDisponibles = totalEmployes > 0 ? (disponibles * 100.0) / totalEmployes : 0;
        double pourcentageNonDisponibles = totalEmployes > 0 ? (nonDisponibles * 100.0) / totalEmployes : 0;

        // Remplir la map avec les résultats
        statistiques["disponibles"] = disponibles;
        statistiques["nonDisponibles"] = nonDisponibles;
        statistiques["totalEmployes"] = totalEmployes;
        statistiques["pourcentageDisponibles"] = pourcentageDisponibles;
        statistiques["pourcentageNonDisponibles"] = pourcentageNonDisponibles;

        qDebug() << "Statistiques récupérées - Disponibles:" << disponibles
                 << "Non disponibles:" << nonDisponibles
                 << "Total:" << totalEmployes;
    } else {
        qDebug() << "Erreur requête statistiques:" << query.lastError().text();
        // Retourner des valeurs par défaut en cas d'erreur
        statistiques["disponibles"] = 0;
        statistiques["nonDisponibles"] = 0;
        statistiques["totalEmployes"] = 0;
        statistiques["pourcentageDisponibles"] = 0;
        statistiques["pourcentageNonDisponibles"] = 0;
    }

    return statistiques;
}
