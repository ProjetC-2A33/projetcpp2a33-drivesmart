# Instructions pour corriger l'erreur CIN_CANDIDAT

## Problème

L'erreur `ORA-00904: "CIN_CANDIDAT": invalid identifier` indique que la colonne `CIN_CANDIDAT` n'existe pas dans votre table PLANNING.

## Solution

### Étape 1 : Vérifier si la colonne existe

Exécutez le script de vérification :
```sql
@verify_cin_candidat.sql
```

Ce script vous dira si la colonne `CIN_CANDIDAT` existe ou non.

### Étape 2 : Ajouter la colonne CIN_CANDIDAT

Si la colonne n'existe pas, exécutez le script de correction :
```sql
@add_cin_candidat_to_planning.sql
```

Ce script va :
- Ajouter la colonne `CIN_CANDIDAT` (type NUMBER(8)) à la table PLANNING
- Créer un index pour améliorer les performances
- Afficher la structure finale de la table

### Étape 3 : Vérifier que tout fonctionne

Après avoir exécuté le script, testez avec cette requête :
```sql
SELECT CIN_CANDIDAT FROM PLANNING WHERE ROWNUM <= 1;
```

Si cette requête fonctionne sans erreur, la colonne a été ajoutée avec succès.

## Comment exécuter les scripts SQL

### Option 1 : SQL Developer
1. Ouvrez SQL Developer
2. Connectez-vous à votre base de données (projet/projet)
3. Ouvrez le fichier `add_cin_candidat_to_planning.sql`
4. Exécutez le script (F5 ou bouton "Run Script")

### Option 2 : SQL*Plus (ligne de commande)
```bash
sqlplus projet/123@projet @add_cin_candidat_to_planning.sql
```

### Option 3 : Oracle SQL Command Line
```bash
sqlplus projet/123@projet
SQL> @add_cin_candidat_to_planning.sql
```

## Après avoir ajouté la colonne

1. **Recompilez l'application** (si nécessaire)
2. **Testez l'ajout d'une séance** dans le module Planning
3. L'erreur devrait être résolue

## Note importante

La colonne `CIN_CANDIDAT` stocke uniquement le **numéro CIN** (NUMBER), pas le nom du candidat. Le nom complet est récupéré via une jointure avec la table CONDIDAT lors de l'affichage.

## Dépannage

Si vous avez encore des erreurs après avoir exécuté le script :

1. Vérifiez que vous êtes connecté à la bonne base de données
2. Vérifiez que vous avez les droits ALTER TABLE
3. Vérifiez que la table PLANNING existe
4. Exécutez `verify_cin_candidat.sql` pour voir l'état actuel

