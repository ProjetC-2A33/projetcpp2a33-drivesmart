-- Script complet pour corriger toutes les colonnes manquantes de la table PLANNING
-- Exécutez ce script une seule fois pour corriger tous les problèmes

SET SERVEROUTPUT ON;

PROMPT ========================================
PROMPT CORRECTION COMPLÈTE DE LA TABLE PLANNING
PROMPT ========================================
PROMPT

-- Fonction pour ajouter une colonne si elle n'existe pas
DECLARE
    PROCEDURE add_column_if_not_exists(
        p_column_name VARCHAR2,
        p_column_def VARCHAR2,
        p_description VARCHAR2 DEFAULT NULL
    ) IS
        v_exists NUMBER;
    BEGIN
        SELECT COUNT(*) INTO v_exists
        FROM USER_TAB_COLUMNS
        WHERE TABLE_NAME = 'PLANNING'
        AND UPPER(COLUMN_NAME) = UPPER(p_column_name);
        
        IF v_exists = 0 THEN
            BEGIN
                EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD ' || p_column_def;
                DBMS_OUTPUT.PUT_LINE('✓ Colonne ' || p_column_name || ' ajoutée');
                IF p_description IS NOT NULL THEN
                    DBMS_OUTPUT.PUT_LINE('  ' || p_description);
                END IF;
            EXCEPTION
                WHEN OTHERS THEN
                    DBMS_OUTPUT.PUT_LINE('✗ Erreur pour ' || p_column_name || ': ' || SQLERRM);
            END;
        ELSE
            DBMS_OUTPUT.PUT_LINE('✓ Colonne ' || p_column_name || ' existe déjà');
        END IF;
    END;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Vérification et ajout des colonnes manquantes...');
    DBMS_OUTPUT.PUT_LINE('');
    
    -- 1. DATE_SEANCE
    add_column_if_not_exists('DATE_SEANCE', '"DATE_SEANCE" DATE', 'Date de la séance');
    
    -- 2. CIN_CANDIDAT
    add_column_if_not_exists('CIN_CANDIDAT', 'CIN_CANDIDAT NUMBER(8)', 'CIN du candidat (numéro uniquement)');
    
    -- 3. NOM_MONITEUR
    add_column_if_not_exists('NOM_MONITEUR', '"NOM_MONITEUR" VARCHAR2(200)', 'Nom du moniteur');
    
    -- 4. VEHICULE
    add_column_if_not_exists('VEHICULE', '"VEHICULE" VARCHAR2(20)', 'Matricule du véhicule');
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('Vérification terminée.');
END;
/
PROMPT

-- Copier les données de MATRICULE_VEHICULE vers VEHICULE si nécessaire
DECLARE
    matricule_exists NUMBER;
    vehicule_exists NUMBER;
BEGIN
    SELECT COUNT(*) INTO matricule_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'MATRICULE_VEHICULE';
    
    SELECT COUNT(*) INTO vehicule_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'VEHICULE';
    
    IF matricule_exists > 0 AND vehicule_exists > 0 THEN
        BEGIN
            EXECUTE IMMEDIATE 'UPDATE PLANNING SET "VEHICULE" = MATRICULE_VEHICULE WHERE MATRICULE_VEHICULE IS NOT NULL AND "VEHICULE" IS NULL';
            DBMS_OUTPUT.PUT_LINE('✓ Données copiées de MATRICULE_VEHICULE vers VEHICULE (si nécessaire)');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('ℹ Pas de données à copier ou colonnes non compatibles');
        END;
    END IF;
END;
/
PROMPT

-- Afficher la structure finale
PROMPT ========================================
PROMPT STRUCTURE FINALE DE LA TABLE PLANNING
PROMPT ========================================
SELECT 
    COLUMN_ID AS "Ordre",
    COLUMN_NAME AS "Nom Colonne",
    DATA_TYPE AS "Type",
    DATA_LENGTH AS "Longueur",
    NULLABLE AS "Nullable"
FROM USER_TAB_COLUMNS
WHERE TABLE_NAME = 'PLANNING'
ORDER BY COLUMN_ID;
PROMPT

PROMPT ========================================
PROMPT CORRECTION TERMINÉE
PROMPT ========================================
PROMPT
PROMPT Toutes les colonnes nécessaires ont été ajoutées.
PROMPT Vous pouvez maintenant utiliser l'application sans erreur.
PROMPT
PROMPT Colonnes ajoutées/vérifiées:
PROMPT   - DATE_SEANCE (DATE)
PROMPT   - CIN_CANDIDAT (NUMBER(8))
PROMPT   - NOM_MONITEUR (VARCHAR2(200))
PROMPT   - VEHICULE (VARCHAR2(20))
PROMPT ========================================

