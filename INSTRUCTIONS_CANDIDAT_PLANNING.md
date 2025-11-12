# Instructions pour le champ CIN Candidat dans Planning

## Modifications effectuées

J'ai ajouté le support pour un champ CIN de candidat dans le formulaire de planning. Le code cherche automatiquement un QComboBox nommé avec "candidat" dans son nom d'objet.

## Nom du champ dans l'UI

Le code cherche automatiquement le champ QComboBox pour le candidat avec les noms suivants (dans cet ordre) :
1. `cin_candidat`
2. `candidat_cin`
3. `cin_candidat_3`
4. Tout QComboBox dont le nom contient "candidat" (insensible à la casse) et qui n'est pas `cin_4` ou `cin_5`

## Fonctionnalités ajoutées

1. **Chargement automatique** : La fonction `loadCandidats()` charge tous les CINs depuis la table CONDIDAT au démarrage
2. **Affichage formaté** : Les candidats sont affichés sous la forme "CIN - Nom Prénom" pour faciliter la sélection
3. **Validation** : Le code vérifie qu'un candidat est sélectionné avant d'ajouter/modifier une séance
4. **Édition** : Lors de l'édition d'une séance, le CIN du candidat est automatiquement chargé dans le champ
5. **Réinitialisation** : Le champ candidat est réinitialisé lors du reset du formulaire

## Comment utiliser

1. **Dans Qt Designer** :
   - Ajoutez un QComboBox dans le formulaire planning.ui
   - Nommez-le `cin_candidat` (ou un des noms supportés ci-dessus)
   - Placez-le où vous voulez dans le formulaire

2. **Recompiler** :
   - Recompilez l'application
   - Le champ se remplira automatiquement avec les candidats de la base de données

## Format des données

- **Affichage** : "12345678 - Dupont Jean" (CIN - Nom Prénom)
- **Valeur stockée** : Le CIN (ex: "12345678") est stocké dans la colonne `NOM_CANDIDAT` de la table PLANNING

## Note importante

Actuellement, le CIN est stocké dans la colonne `NOM_CANDIDAT` de la table PLANNING. Si vous préférez stocker le nom complet au lieu du CIN, vous pouvez modifier la fonction `ajouter()` et `modifier()` dans `planning.cpp` pour récupérer le nom complet depuis la table CONDIDAT en utilisant le CIN.

## Dépannage

Si le champ ne se remplit pas :
1. Vérifiez que le QComboBox a un nom qui contient "candidat"
2. Vérifiez que la table CONDIDAT existe et contient des données
3. Vérifiez les messages d'avertissement dans la console (qDebug)

## Exemple de requête pour obtenir le nom complet

Si vous voulez stocker le nom complet au lieu du CIN, vous pouvez modifier le code pour faire une jointure :

```cpp
// Dans ajouter() ou modifier(), après avoir récupéré le CIN :
QSqlQuery nameQuery;
nameQuery.prepare("SELECT nom || ' ' || prenom FROM CONDIDAT WHERE cin_condidat = :cin");
nameQuery.bindValue(":cin", condidat);
if (nameQuery.exec() && nameQuery.next()) {
    condidat = nameQuery.value(0).toString(); // Utiliser le nom complet
}
```

