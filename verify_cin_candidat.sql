-- Script de vérification rapide pour CIN_CANDIDAT
-- Exécutez ce script pour vérifier si la colonne existe

SET SERVEROUTPUT ON;

PROMPT ========================================
PROMPT VÉRIFICATION DE LA COLONNE CIN_CANDIDAT
PROMPT ========================================
PROMPT

-- Vérifier si la colonne CIN_CANDIDAT existe
DECLARE
    col_exists NUMBER;
    col_name VARCHAR2(100);
BEGIN
    SELECT COUNT(*) INTO col_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'CIN_CANDIDAT';
    
    IF col_exists > 0 THEN
        DBMS_OUTPUT.PUT_LINE('✓ Colonne CIN_CANDIDAT EXISTE');
        
        -- Afficher les détails de la colonne
        SELECT COLUMN_NAME, DATA_TYPE, DATA_LENGTH, NULLABLE
        INTO col_name, col_name, col_name, col_name
        FROM USER_TAB_COLUMNS
        WHERE TABLE_NAME = 'PLANNING'
        AND UPPER(COLUMN_NAME) = 'CIN_CANDIDAT'
        AND ROWNUM = 1;
        
    ELSE
        DBMS_OUTPUT.PUT_LINE('✗ Colonne CIN_CANDIDAT N''EXISTE PAS');
        DBMS_OUTPUT.PUT_LINE('');
        DBMS_OUTPUT.PUT_LINE('ACTION REQUISE: Exécutez le script add_cin_candidat_to_planning.sql');
    END IF;
END;
/
PROMPT

-- Afficher toutes les colonnes de PLANNING
PROMPT === COLONNES ACTUELLES DE LA TABLE PLANNING ===
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
PROMPT Si CIN_CANDIDAT n'existe pas, exécutez:
PROMPT @add_cin_candidat_to_planning.sql
PROMPT ========================================

