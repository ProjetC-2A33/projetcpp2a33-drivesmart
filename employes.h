// employes.h
#ifndef EMPLOYES_H
#define EMPLOYES_H

#include <QString>
#include <QSqlQueryModel>

class Employes
{
private:
    QString cin_employee;
    QString nom;
    QString prenom;
    bool    disponibilite;
    double  salaire;
    QString telephone;
    QString email;
    QString poste;
    QString mdp;

public:
    // === Constructeurs ===
    Employes();
    Employes(const QString &cin_employee,
             const QString &nom,
             const QString &prenom,
             bool disponibilite,
             double salaire,
             const QString &telephone,
             const QString &email,
             const QString &poste,
             const QString &mdp);

    // === Getters ===
    QString getCinEmployee() const { return cin_employee; }
    QString getNom()         const { return nom; }
    QString getPrenom()      const { return prenom; }
    bool    isDisponible()   const { return disponibilite; }
    double  getSalaire()     const { return salaire; }
    QString getTelephone()   const { return telephone; }
    QString getEmail()       const { return email; }
    QString getPoste()       const { return poste; }
    QString getMdp()         const { return mdp; }

    // === Setters ===
    void setCinEmployee(const QString &cin) { cin_employee = cin; }
    void setNom(const QString &n)           { nom = n; }
    void setPrenom(const QString &p)        { prenom = p; }
    void setDisponibilite(bool dispo)       { disponibilite = dispo; }
    void setSalaire(double s)               { salaire = s; }
    void setTelephone(const QString &t)     { telephone = t; }
    void setEmail(const QString &e)         { email = e; }
    void setPoste(const QString &p)         { poste = p; }
    void setMdp(const QString &m)           { mdp = m; }

    // === Fonctionnalités ===
    bool ajouter();
    QSqlQueryModel* afficher();
    static bool supprimer(const QString &cin_employee);
    bool modifier();
    bool modifier(const QString &originalCIN);  // Overload to support CIN change

    // === Vérification d'unicité ===
    static bool cinExiste(const QString &cin, const QString &cinActuel = "");
    static bool emailExiste(const QString &email, const QString &cinActuel = "");
    static QSqlQueryModel* rechercherMulti(const QString &query);
    static QSqlQueryModel* trierParNomCroissant();
    static QSqlQueryModel* trierParPrenomAlphabetique();
    static QSqlQueryModel* trierParSalaire();
    static QMap<QString, QVariant> getStatistiquesDisponibilite();
};

#endif // EMPLOYES_H
