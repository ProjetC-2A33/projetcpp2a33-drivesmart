# Résolution du problème d'ajout de candidat

## Problème identifié

L'erreur "Échec d'ajout du candidat" peut avoir plusieurs causes. Le code a été amélioré pour afficher des messages d'erreur détaillés.

## Causes possibles

### 1. Table CONDIDAT n'existe pas (ORA-00942)
**Symptôme** : Message d'erreur indiquant que la table n'existe pas

**Solution** : Exécuter le script SQL
```bash
sqlplus projet/123@projet @create_tables_safe.sql
```

### 2. CIN déjà existant (ORA-00001)
**Symptôme** : Message indiquant qu'un candidat avec ce CIN existe déjà

**Solution** : Utiliser un CIN différent (le CIN doit être unique)

### 3. Contrainte CHECK violée (ORA-02290)
**Symptôme** : Les données ne respectent pas les contraintes

**Vérifications** :
- Le sexe doit être exactement 'H' ou 'F' (pas d'espaces, pas de minuscules)
- Tous les champs obligatoires doivent être remplis

### 4. Valeur NULL dans un champ NOT NULL (ORA-01400)
**Symptôme** : Message indiquant qu'un champ obligatoire est vide

**Solution** : Remplir tous les champs du formulaire

### 5. Problème de connexion à la base de données
**Vérification** : S'assurer que la connexion fonctionne

## Vérifications à effectuer

### Étape 1 : Vérifier que la table existe
Exécuter le script de vérification :
```bash
sqlplus projet/123@projet @check_database.sql
```

### Étape 2 : Vérifier la structure de la table
```sql
DESC CONDIDAT;
```

La table doit avoir les colonnes suivantes :
- `cin_condidat` NUMBER(8) PRIMARY KEY
- `nom` VARCHAR2(100) NOT NULL
- `prenom` VARCHAR2(100) NOT NULL
- `sexe` VARCHAR2(1) NOT NULL CHECK (sexe IN ('H', 'F'))
- `date_naissance` DATE NOT NULL
- `tel` NUMBER(8) NOT NULL
- `type_permis` VARCHAR2(50) NOT NULL

### Étape 3 : Tester l'insertion manuelle
```sql
INSERT INTO CONDIDAT (cin_condidat, nom, prenom, sexe, date_naissance, tel, type_permis)
VALUES (12345678, 'Test', 'User', 'H', DATE '1990-01-01', 12345678, 'B');
COMMIT;
```

Si cette insertion fonctionne, le problème est dans le code C++.
Si elle échoue, le problème est dans la base de données.

### Étape 4 : Vérifier les données existantes
```sql
SELECT * FROM CONDIDAT;
```

Vérifier s'il y a déjà des candidats avec le même CIN.

## Améliorations apportées au code

1. **Gestion d'erreurs détaillée** : Le code affiche maintenant des messages d'erreur spécifiques selon le type d'erreur SQL
2. **Vérification de l'existence de la table** : Messages clairs si la table n'existe pas
3. **Détection des doublons** : Message spécifique si le CIN existe déjà
4. **Validation des contraintes** : Messages pour les violations de contraintes CHECK

## Messages d'erreur améliorés

- **Table manquante** : "La table CONDIDAT n'existe pas dans la base de données. Veuillez exécuter le script SQL 'create_tables_safe.sql'"
- **CIN dupliqué** : "Un candidat avec ce CIN existe déjà dans la base de données. Veuillez utiliser un CIN différent."
- **Contrainte violée** : "Les données saisies ne respectent pas les contraintes de la base de données."
- **Champ NULL** : "Certains champs obligatoires sont vides."
- **Autre erreur SQL** : Affiche le message d'erreur technique complet

## Prochaines étapes

1. Recompiler l'application avec les modifications
2. Relancer l'application
3. Essayer d'ajouter un candidat
4. Si une erreur apparaît, le message sera maintenant détaillé et indiquera la cause exacte
5. Suivre les instructions du message d'erreur pour résoudre le problème

## Test rapide

Pour tester rapidement si tout fonctionne :

1. Exécuter `create_tables_safe.sql` pour créer les tables
2. Exécuter `check_database.sql` pour vérifier
3. Lancer l'application
4. Essayer d'ajouter un candidat avec :
   - CIN : 12345678
   - Nom : Test
   - Prénom : User
   - Sexe : H (homme)
   - Date de naissance : 01/01/1990
   - Téléphone : 87654321
   - Type de permis : B (ou autre selon votre liste)

