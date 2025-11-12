-- Script pour ajouter les colonnes manquantes à la table PLANNING
-- Ce script ajoute DATE_SEANCE et vérifie/corrige la structure

SET SERVEROUTPUT ON;

PROMPT ========================================
PROMPT CORRECTION DE LA TABLE PLANNING
PROMPT ========================================
PROMPT

-- 1. Ajouter DATE_SEANCE si elle n'existe pas
DECLARE
    col_exists NUMBER;
BEGIN
    SELECT COUNT(*) INTO col_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'DATE_SEANCE';
    
    IF col_exists = 0 THEN
        BEGIN
            EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD "DATE_SEANCE" DATE';
            DBMS_OUTPUT.PUT_LINE('✓ Colonne DATE_SEANCE ajoutée avec succès');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de DATE_SEANCE: ' || SQLERRM);
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Colonne DATE_SEANCE existe déjà');
    END IF;
END;
/
PROMPT

-- 2. Vérifier et ajouter CIN_CANDIDAT si elle n'existe pas
DECLARE
    col_exists NUMBER;
BEGIN
    SELECT COUNT(*) INTO col_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'CIN_CANDIDAT';
    
    IF col_exists = 0 THEN
        BEGIN
            EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD CIN_CANDIDAT NUMBER(8)';
            DBMS_OUTPUT.PUT_LINE('✓ Colonne CIN_CANDIDAT ajoutée avec succès');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de CIN_CANDIDAT: ' || SQLERRM);
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Colonne CIN_CANDIDAT existe déjà');
    END IF;
END;
/
PROMPT

-- 3. Vérifier et ajouter NOM_MONITEUR si elle n'existe pas
DECLARE
    col_exists NUMBER;
BEGIN
    SELECT COUNT(*) INTO col_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'NOM_MONITEUR';
    
    IF col_exists = 0 THEN
        BEGIN
            EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD "NOM_MONITEUR" VARCHAR2(200)';
            DBMS_OUTPUT.PUT_LINE('✓ Colonne NOM_MONITEUR ajoutée avec succès');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de NOM_MONITEUR: ' || SQLERRM);
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Colonne NOM_MONITEUR existe déjà');
    END IF;
END;
/
PROMPT

-- 4. Vérifier et ajouter VEHICULE si elle n'existe pas (sans guillemets)
DECLARE
    col_exists NUMBER;
BEGIN
    SELECT COUNT(*) INTO col_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'VEHICULE';
    
    IF col_exists = 0 THEN
        BEGIN
            -- Vérifier si MATRICULE_VEHICULE existe (peut être un alias)
            SELECT COUNT(*) INTO col_exists
            FROM USER_TAB_COLUMNS
            WHERE TABLE_NAME = 'PLANNING'
            AND UPPER(COLUMN_NAME) = 'MATRICULE_VEHICULE';
            
            IF col_exists > 0 THEN
                DBMS_OUTPUT.PUT_LINE('ℹ Colonne MATRICULE_VEHICULE existe (peut être utilisée à la place de VEHICULE)');
            ELSE
                EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD "VEHICULE" VARCHAR2(20)';
                DBMS_OUTPUT.PUT_LINE('✓ Colonne VEHICULE ajoutée avec succès');
            END IF;
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de VEHICULE: ' || SQLERRM);
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Colonne VEHICULE existe déjà');
    END IF;
END;
/
PROMPT

-- Afficher la structure finale
PROMPT === STRUCTURE FINALE DE LA TABLE PLANNING ===
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
PROMPT Les colonnes manquantes ont été ajoutées.
PROMPT Vous pouvez maintenant utiliser l'application sans erreur.

