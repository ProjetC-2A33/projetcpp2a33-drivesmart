-- Script complet pour corriger TOUTES les colonnes manquantes de la table PLANNING
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
    
    -- 1. DATE_SEANCE (DATE pour la date de la séance)
    add_column_if_not_exists('DATE_SEANCE', '"DATE_SEANCE" DATE', 'Date de la séance');
    
    -- 2. CIN_CANDIDAT (NUMBER pour stocker le CIN du candidat)
    add_column_if_not_exists('CIN_CANDIDAT', 'CIN_CANDIDAT NUMBER(8)', 'CIN du candidat (numéro uniquement)');
    
    -- 3. CIN_EMPLOYEE (NUMBER pour stocker le CIN de l'employé - clé étrangère)
    add_column_if_not_exists('CIN_EMPLOYEE', 'CIN_EMPLOYEE NUMBER(8)', 'CIN de l''employé (clé étrangère vers EMPLOYEE.CIN)');
    
    -- 4. VEHICULE ou MATRICULE_VEHICULE (VARCHAR2 pour la matricule)
    -- Vérifier d'abord si MATRICULE_VEHICULE existe
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
        
        IF matricule_exists = 0 AND vehicule_exists = 0 THEN
            -- Aucune des deux n'existe, créer MATRICULE_VEHICULE (plus descriptif)
            BEGIN
                EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD MATRICULE_VEHICULE VARCHAR2(20)';
                DBMS_OUTPUT.PUT_LINE('✓ Colonne MATRICULE_VEHICULE ajoutée');
                DBMS_OUTPUT.PUT_LINE('  Matricule du véhicule');
            EXCEPTION
                WHEN OTHERS THEN
                    DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de MATRICULE_VEHICULE: ' || SQLERRM);
            END;
        ELSIF matricule_exists > 0 THEN
            DBMS_OUTPUT.PUT_LINE('✓ Colonne MATRICULE_VEHICULE existe déjà');
        ELSIF vehicule_exists > 0 THEN
            DBMS_OUTPUT.PUT_LINE('✓ Colonne VEHICULE existe déjà');
        END IF;
    END;
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('Vérification terminée.');
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
PROMPT   - CIN_EMPLOYEE (NUMBER(8))
PROMPT   - MATRICULE_VEHICULE ou VEHICULE (VARCHAR2(20))
PROMPT ========================================

