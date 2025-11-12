-- Script de diagnostic complet pour la base de données DriveSmart
-- Exécutez ce script dans SQL Developer ou SQL*Plus
-- Ce script vérifie la structure de toutes les tables et identifie les problèmes

SET SERVEROUTPUT ON SIZE 1000000;

PROMPT ========================================
PROMPT DIAGNOSTIC DE LA BASE DE DONNÉES
PROMPT ========================================
PROMPT

-- 1. Vérifier l'existence de toutes les tables
PROMPT === VÉRIFICATION DES TABLES ===
DECLARE
    v_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'CONDIDAT';
    DBMS_OUTPUT.PUT_LINE('CONDIDAT: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ MANQUANTE' END);
    
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'VEHICULE';
    DBMS_OUTPUT.PUT_LINE('VEHICULE: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ MANQUANTE' END);
    
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'EXAMEN';
    DBMS_OUTPUT.PUT_LINE('EXAMEN: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ MANQUANTE' END);
    
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'EMPLOYEE';
    DBMS_OUTPUT.PUT_LINE('EMPLOYEE: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ MANQUANTE' END);
    
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'PLANNING';
    DBMS_OUTPUT.PUT_LINE('PLANNING: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ MANQUANTE' END);
END;
/
PROMPT

-- 2. Structure détaillée de la table PLANNING
PROMPT === STRUCTURE DE LA TABLE PLANNING ===
SELECT 
    COLUMN_NAME AS "Nom Colonne",
    DATA_TYPE AS "Type",
    DATA_LENGTH AS "Longueur",
    NULLABLE AS "Nullable",
    DATA_DEFAULT AS "Valeur Par Défaut"
FROM USER_TAB_COLUMNS
WHERE TABLE_NAME = 'PLANNING'
ORDER BY COLUMN_ID;
PROMPT

-- 3. Vérifier spécifiquement la colonne NOM_CANDIDAT
PROMPT === VÉRIFICATION COLONNE NOM_CANDIDAT ===
DECLARE
    v_count NUMBER;
    v_col_name VARCHAR2(100);
BEGIN
    -- Vérifier avec guillemets (sensible à la casse)
    SELECT COUNT(*) INTO v_count
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND COLUMN_NAME = 'NOM_CANDIDAT';
    
    IF v_count > 0 THEN
        DBMS_OUTPUT.PUT_LINE('✓ Colonne NOM_CANDIDAT existe (avec casse exacte)');
    ELSE
        DBMS_OUTPUT.PUT_LINE('✗ Colonne NOM_CANDIDAT n''existe PAS avec ce nom exact');
        
        -- Chercher des colonnes similaires
        DBMS_OUTPUT.PUT_LINE('Recherche de colonnes similaires...');
        FOR rec IN (
            SELECT COLUMN_NAME 
            FROM USER_TAB_COLUMNS
            WHERE TABLE_NAME = 'PLANNING'
            AND (UPPER(COLUMN_NAME) LIKE '%CANDIDAT%' OR UPPER(COLUMN_NAME) LIKE '%NOM%')
        ) LOOP
            DBMS_OUTPUT.PUT_LINE('  - Colonne trouvée: ' || rec.COLUMN_NAME);
        END LOOP;
    END IF;
END;
/
PROMPT

-- 4. Afficher TOUTES les colonnes de PLANNING (pour voir les noms exacts)
PROMPT === TOUTES LES COLONNES DE PLANNING ===
SELECT 
    COLUMN_ID AS "Ordre",
    COLUMN_NAME AS "Nom Exact",
    DATA_TYPE AS "Type",
    NULLABLE AS "Nullable"
FROM USER_TAB_COLUMNS
WHERE TABLE_NAME = 'PLANNING'
ORDER BY COLUMN_ID;
PROMPT

-- 5. Tester une requête simple pour voir quelles colonnes fonctionnent
PROMPT === TEST DE REQUÊTE SIMPLE ===
BEGIN
    DECLARE
        v_test VARCHAR2(200);
    BEGIN
        -- Tester sans guillemets
        BEGIN
            SELECT NOM_CANDIDAT INTO v_test FROM PLANNING WHERE ROWNUM = 1;
            DBMS_OUTPUT.PUT_LINE('✓ NOM_CANDIDAT (sans guillemets) fonctionne');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ NOM_CANDIDAT (sans guillemets) ne fonctionne pas: ' || SQLERRM);
        END;
        
        -- Tester avec guillemets
        BEGIN
            EXECUTE IMMEDIATE 'SELECT "NOM_CANDIDAT" FROM PLANNING WHERE ROWNUM = 1' INTO v_test;
            DBMS_OUTPUT.PUT_LINE('✓ "NOM_CANDIDAT" (avec guillemets) fonctionne');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ "NOM_CANDIDAT" (avec guillemets) ne fonctionne pas: ' || SQLERRM);
        END;
    END;
END;
/
PROMPT

-- 6. Vérifier les contraintes et index
PROMPT === CONTRAINTES ET INDEX ===
SELECT 
    CONSTRAINT_NAME AS "Nom Contrainte",
    CONSTRAINT_TYPE AS "Type",
    SEARCH_CONDITION AS "Condition"
FROM USER_CONSTRAINTS
WHERE TABLE_NAME = 'PLANNING';
PROMPT

SELECT 
    INDEX_NAME AS "Nom Index",
    COLUMN_NAME AS "Colonne"
FROM USER_IND_COLUMNS
WHERE TABLE_NAME = 'PLANNING'
ORDER BY INDEX_NAME, COLUMN_POSITION;
PROMPT

-- 7. Compter les enregistrements
PROMPT === NOMBRE D'ENREGISTREMENTS ===
DECLARE
    v_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_count FROM PLANNING;
    DBMS_OUTPUT.PUT_LINE('Nombre de séances dans PLANNING: ' || v_count);
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Erreur lors du comptage: ' || SQLERRM);
END;
/
PROMPT

PROMPT ========================================
PROMPT DIAGNOSTIC TERMINÉ
PROMPT ========================================
PROMPT
PROMPT Si la colonne NOM_CANDIDAT n'existe pas, exécutez le script fix_planning_table.sql
PROMPT pour l'ajouter automatiquement.

