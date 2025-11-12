-- Script pour ajouter la colonne CIN_CANDIDAT à la table PLANNING
-- Cette colonne stockera uniquement le CIN (numéro) du candidat

SET SERVEROUTPUT ON;

PROMPT ========================================
PROMPT AJOUT DE LA COLONNE CIN_CANDIDAT
PROMPT ========================================
PROMPT

-- Vérifier et ajouter la colonne CIN_CANDIDAT
DECLARE
    col_exists NUMBER;
BEGIN
    -- Vérifier si la colonne existe déjà
    SELECT COUNT(*) INTO col_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'CIN_CANDIDAT';
    
    IF col_exists = 0 THEN
        -- La colonne n'existe pas, l'ajouter
        BEGIN
            EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD CIN_CANDIDAT NUMBER(8)';
            DBMS_OUTPUT.PUT_LINE('✓ Colonne CIN_CANDIDAT ajoutée avec succès (NUMBER pour stocker le CIN)');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de la colonne: ' || SQLERRM);
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Colonne CIN_CANDIDAT existe déjà');
    END IF;
END;
/
PROMPT

-- Créer un index pour améliorer les performances
DECLARE
    idx_exists NUMBER;
BEGIN
    SELECT COUNT(*) INTO idx_exists
    FROM USER_INDEXES
    WHERE TABLE_NAME = 'PLANNING'
    AND INDEX_NAME = 'IDX_PLANNING_CIN_CANDIDAT';
    
    IF idx_exists = 0 THEN
        BEGIN
            EXECUTE IMMEDIATE 'CREATE INDEX IDX_PLANNING_CIN_CANDIDAT ON PLANNING(CIN_CANDIDAT)';
            DBMS_OUTPUT.PUT_LINE('✓ Index IDX_PLANNING_CIN_CANDIDAT créé avec succès');
        EXCEPTION
            WHEN OTHERS THEN
                DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de la création de l''index: ' || SQLERRM);
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Index IDX_PLANNING_CIN_CANDIDAT existe déjà');
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
PROMPT COLONNE CIN_CANDIDAT AJOUTÉE
PROMPT ========================================
PROMPT
PROMPT La colonne CIN_CANDIDAT stockera uniquement le numéro CIN du candidat (NUMBER).
PROMPT Vous pouvez maintenant utiliser cette colonne dans votre application.

