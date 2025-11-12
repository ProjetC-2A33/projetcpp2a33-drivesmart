-- Script pour ajouter la colonne DATE_SEANCE à la table PLANNING
-- Cette colonne stocke la date de la séance

SET SERVEROUTPUT ON;

PROMPT ========================================
PROMPT AJOUT DE LA COLONNE DATE_SEANCE
PROMPT ========================================
PROMPT

-- Vérifier et ajouter la colonne DATE_SEANCE
DECLARE
    col_exists NUMBER;
BEGIN
    -- Vérifier si la colonne existe déjà
    SELECT COUNT(*) INTO col_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'DATE_SEANCE';
    
    IF col_exists = 0 THEN
        -- La colonne n'existe pas, l'ajouter
        BEGIN
            EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD "DATE_SEANCE" DATE';
            DBMS_OUTPUT.PUT_LINE('✓ Colonne DATE_SEANCE ajoutée avec succès');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de la colonne: ' || SQLERRM);
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Colonne DATE_SEANCE existe déjà');
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
PROMPT COLONNE DATE_SEANCE AJOUTÉE
PROMPT ========================================

