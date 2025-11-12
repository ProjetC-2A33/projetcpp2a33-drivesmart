# Diagnostic de la Base de Données DriveSmart

## Problème identifié

L'erreur **ORA-00904: "NOM_CANDIDAT": invalid identifier** indique que la colonne `NOM_CANDIDAT` n'existe pas dans la table `PLANNING` ou qu'elle a un nom différent.

## Scripts de diagnostic et correction

### 1. `diagnostic_database.sql` (À EXÉCUTER EN PREMIER)

Ce script vérifie :
- L'existence de toutes les tables (CONDIDAT, VEHICULE, EXAMEN, EMPLOYEE, PLANNING)
- La structure complète de la table PLANNING
- L'existence de la colonne NOM_CANDIDAT
- Les colonnes similaires qui pourraient exister
- Les contraintes et index
- Le nombre d'enregistrements

**Comment l'exécuter :**
```sql
-- Dans SQL Developer ou SQL*Plus
@diagnostic_database.sql
```

### 2. `fix_planning_table.sql` (À EXÉCUTER APRÈS LE DIAGNOSTIC)

Ce script :
- Ajoute la colonne `NOM_CANDIDAT` si elle n'existe pas
- Crée l'index `IDX_PLANNING_CANDIDAT` si nécessaire
- Affiche la structure finale de la table

**Comment l'exécuter :**
```sql
-- Dans SQL Developer ou SQL*Plus
@fix_planning_table.sql
```

### 3. `check_planning_table.sql` (Script rapide)

Script simplifié pour vérifier rapidement la structure de la table PLANNING.

## Structure attendue de la table PLANNING

Selon `create_tables.sql`, la table PLANNING doit avoir :

| Colonne | Type | Description |
|---------|------|-------------|
| ID_SÉANCE | NUMBER | Clé primaire |
| DATE_SEANCE | DATE | Date de la séance |
| **NOM_CANDIDAT** | VARCHAR2(200) | **COLONNE MANQUANTE** |
| TYPE_SÉANCE | VARCHAR2(50) | Type (code/conduit) |
| HEURE_DEBUT | TIMESTAMP | Heure de début |
| HEURE_FIN | VARCHAR2(10) | Heure de fin |
| ADRESSE_CIRCUIT | VARCHAR2(200) | Adresse du circuit |
| NOM_MONITEUR | VARCHAR2(200) | Nom du moniteur |
| VEHICULE | VARCHAR2(20) | Matricule du véhicule |

## Causes possibles du problème

1. **Table créée manuellement** : La table a été créée sans la colonne `NOM_CANDIDAT`
2. **Nom différent** : La colonne existe mais sous un autre nom (ex: `CIN_CANDIDAT`, `CANDIDAT`, etc.)
3. **Casse différente** : La colonne existe mais avec une casse différente (Oracle est sensible à la casse avec guillemets)
4. **Script non exécuté** : Le script `create_tables_safe.sql` n'a pas été exécuté complètement

## Solution recommandée

1. **Exécutez `diagnostic_database.sql`** pour voir la structure réelle
2. **Exécutez `fix_planning_table.sql`** pour ajouter la colonne manquante
3. **Recompilez l'application** et testez à nouveau

## Alternative : Recréer la table

Si la table PLANNING a une structure complètement différente, vous pouvez :

1. Sauvegarder les données existantes (si importantes)
2. Supprimer la table : `DROP TABLE PLANNING CASCADE CONSTRAINTS;`
3. Exécuter `create_tables_safe.sql` pour recréer la table avec la bonne structure

## Vérification après correction

Après avoir exécuté `fix_planning_table.sql`, testez avec cette requête :

```sql
SELECT NOM_CANDIDAT FROM PLANNING WHERE ROWNUM <= 1;
```

Si cette requête fonctionne, le problème est résolu.

