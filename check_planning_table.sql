-- Script pour vérifier la structure de la table PLANNING
-- Exécutez ce script dans SQL Developer ou SQL*Plus pour voir la structure réelle

-- 1. Vérifier si la table existe
SELECT COUNT(*) AS TABLE_EXISTS 
FROM USER_TABLES 
WHERE TABLE_NAME = 'PLANNING';

-- 2. Afficher toutes les colonnes de la table PLANNING
SELECT 
    COLUMN_NAME,
    DATA_TYPE,
    DATA_LENGTH,
    NULLABLE,
    DATA_DEFAULT
FROM USER_TAB_COLUMNS
WHERE TABLE_NAME = 'PLANNING'
ORDER BY COLUMN_ID;

-- 3. Vérifier spécifiquement les colonnes liées au candidat
SELECT 
    COLUMN_NAME,
    DATA_TYPE
FROM USER_TAB_COLUMNS
WHERE TABLE_NAME = 'PLANNING'
AND (COLUMN_NAME LIKE '%CANDIDAT%' OR COLUMN_NAME LIKE '%candidat%' OR COLUMN_NAME LIKE '%Candidat%')
ORDER BY COLUMN_ID;

-- 4. Afficher toutes les colonnes (y compris celles avec caractères spéciaux)
SELECT 
    COLUMN_NAME,
    DATA_TYPE,
    DATA_LENGTH
FROM USER_TAB_COLUMNS
WHERE TABLE_NAME = 'PLANNING'
ORDER BY COLUMN_ID;

-- 5. Tester une requête simple pour voir quelles colonnes fonctionnent
SELECT * FROM PLANNING WHERE ROWNUM <= 1;

