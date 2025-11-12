# Configuration de la Base de Données DriveSmart

## Problème résolu

L'erreur **ORA-00942: table or view does not exist** indiquait que la table `PLANNING` (et possiblement d'autres tables) n'existait pas dans la base de données Oracle.

## Solution

Deux scripts SQL ont été créés pour résoudre ce problème :

### 1. `create_tables.sql`
Script SQL standard qui crée toutes les tables nécessaires. **Attention** : Ce script échouera si les tables existent déjà.

### 2. `create_tables_safe.sql` (RECOMMANDÉ)
Script SQL sécurisé qui vérifie l'existence des tables avant de les créer. Ce script peut être exécuté plusieurs fois sans erreur.

## Tables créées

Le script crée les tables suivantes :

1. **CONDIDAT** - Table des candidats pour le permis de conduire
   - cin_condidat (PK)
   - nom, prenom, sexe, date_naissance, tel, type_permis

2. **VEHICULE** - Table des véhicules de l'auto-école
   - MATRICULE (PK)
   - MODELE, KILOMETRAGE, DISPONIBILITE, TYPE_ENERGIE, ETAT_MAINTENANCE, DATE_MAINTENANCE

3. **EXAMEN** - Table des examens de code et de conduite
   - ID_EXAMEN (PK)
   - TYPE_EXAMEN, TYPE_PERMIS, DATE_EXAMEN

4. **EMPLOYEE** - Table des employés/moniteurs
   - ID_EMPLOYEE (PK, auto-généré)
   - NOM, PRENOM, CIN, TEL, EMAIL, DATE_EMBAUCHE, POSTE

5. **PLANNING** - Table de planification des séances de conduite (TABLE MANQUANTE)
   - ID_SÉANCE (PK)
   - DATE_SEANCE, NOM_CANDIDAT, TYPE_SÉANCE, HEURE_DEBUT, HEURE_FIN
   - ADRESSE_CIRCUIT, NOM_MONITEUR, VEHICULE (FK vers VEHICULE)

## Comment exécuter les scripts

### Option 1 : SQL*Plus (ligne de commande)
```bash
sqlplus projet/123@projet @create_tables_safe.sql
```

### Option 2 : SQL Developer
1. Ouvrir SQL Developer
2. Se connecter à la base de données (projet/projet)
3. Ouvrir le fichier `create_tables_safe.sql`
4. Exécuter le script (F5 ou bouton "Run Script")

### Option 3 : Oracle SQL Command Line
```bash
sqlplus projet/123@projet
SQL> @create_tables_safe.sql
```

## Vérification

Après l'exécution du script, vous pouvez vérifier que les tables ont été créées :

```sql
SELECT table_name FROM user_tables 
WHERE table_name IN ('CONDIDAT', 'VEHICULE', 'EXAMEN', 'EMPLOYEE', 'PLANNING')
ORDER BY table_name;
```

## Informations de connexion

D'après le fichier `connection.cpp`, les paramètres de connexion sont :
- **DSN/Base de données** : projet
- **Utilisateur** : projet
- **Mot de passe** : 123
- **Type** : QODBC (Oracle ODBC)

## Notes importantes

1. Le script `create_tables_safe.sql` est recommandé car il peut être exécuté plusieurs fois sans erreur.

2. Les séquences `SEQ_PLANNING` et `SEQ_PLANNING_ID` sont créées pour générer automatiquement les IDs de séance.

3. Des index ont été créés pour améliorer les performances des requêtes.

4. La table PLANNING a une clé étrangère vers VEHICULE, donc assurez-vous que la table VEHICULE existe avant de créer PLANNING (le script gère cela automatiquement).

## Après l'exécution

Une fois les tables créées, l'application DriveSmart devrait fonctionner sans l'erreur ORA-00942.

Si vous rencontrez encore des problèmes :
1. Vérifiez que vous êtes connecté avec le bon utilisateur (projet)
2. Vérifiez que toutes les tables ont été créées (voir section Vérification)
3. Vérifiez les permissions de l'utilisateur sur les tables

