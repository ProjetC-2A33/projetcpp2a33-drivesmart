# Résolution des erreurs de compilation

## Erreurs rencontrées

Les erreurs de linking suivantes apparaissent :
- `undefined reference to 'MainWindow::staticMetaObject'`
- `undefined reference to 'vtable for MainWindow'`

Ces erreurs indiquent que les fichiers moc (Meta-Object Compiler) ne sont pas générés ou liés correctement.

## Solutions

### Solution 1 : Nettoyer et reconstruire dans Qt Creator (RECOMMANDÉ)

1. **Dans Qt Creator** :
   - Menu `Build` → `Clean All` (ou `Ctrl+Shift+C`)
   - Attendez que le nettoyage soit terminé
   - Menu `Build` → `Rebuild All` (ou `Ctrl+Shift+B`)

2. **Si cela ne fonctionne pas** :
   - Fermez Qt Creator
   - Supprimez le dossier `build` dans votre projet
   - Rouvrez Qt Creator
   - Ouvrez le projet
   - Recompilez

### Solution 2 : Utiliser le script de nettoyage

1. Double-cliquez sur `clean_and_rebuild.bat` dans l'explorateur Windows
2. Ou exécutez-le depuis le terminal
3. Puis recompilez dans Qt Creator

### Solution 3 : Vérifier la configuration du projet

1. Dans Qt Creator, allez dans `Projects` (onglet à gauche)
2. Vérifiez que le kit Qt est correctement configuré
3. Vérifiez que la version de Qt est correcte (6.7.3 selon les dossiers de build)

### Solution 4 : Forcer la régénération des fichiers moc

1. Dans Qt Creator, allez dans `Projects`
2. Cliquez sur `Run qmake` dans la section Build Steps
3. Puis `Build` → `Rebuild All`

## Vérifications effectuées

✅ Le fichier `mainwindow.h` contient bien `Q_OBJECT`
✅ Le fichier `mainwindow.cpp` est correct
✅ Le fichier `drivesmart.pro` est correctement configuré
✅ Tous les includes sont présents

## Cause probable

Les fichiers moc (`moc_mainwindow.cpp`) n'ont pas été régénérés après les modifications récentes de `mainwindow.h`. Qt Creator devrait les régénérer automatiquement lors d'un rebuild complet.

## Après le rebuild

Si les erreurs persistent après un rebuild complet, vérifiez :
1. Que tous les fichiers `.h` avec `Q_OBJECT` sont listés dans `HEADERS +=` dans `drivesmart.pro`
2. Qu'il n'y a pas d'erreurs de syntaxe dans `mainwindow.h`
3. Que Qt Creator utilise la bonne version de Qt

