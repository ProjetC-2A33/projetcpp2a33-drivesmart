-- Script pour corriger la structure de la table PLANNING
-- Ce script ajoute la colonne NOM_CANDIDAT si elle n'existe pas
-- IMPORTANT: Exécutez d'abord diagnostic_database.sql pour voir la structure actuelle

SET SERVEROUTPUT ON;

PROMPT ========================================
PROMPT CORRECTION DE LA TABLE PLANNING
PROMPT ========================================
PROMPT

-- Vérifier et ajouter la colonne NOM_CANDIDAT si elle n'existe pas
DECLARE
    col_exists NUMBER;
    col_exists_quoted NUMBER;
BEGIN
    -- Vérifier si la colonne existe sans guillemets (Oracle convertit en majuscules)
    SELECT COUNT(*) INTO col_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'NOM_CANDIDAT';
    
    -- Vérifier si la colonne existe avec guillemets (nom exact)
    SELECT COUNT(*) INTO col_exists_quoted
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND COLUMN_NAME = 'NOM_CANDIDAT';
    
    IF col_exists = 0 AND col_exists_quoted = 0 THEN
        -- La colonne n'existe pas, l'ajouter
        BEGIN
            -- Essayer d'abord sans guillemets (Oracle convertit en majuscules)
            EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD NOM_CANDIDAT VARCHAR2(200)';
            DBMS_OUTPUT.PUT_LINE('✓ Colonne NOM_CANDIDAT ajoutée avec succès (sans guillemets)');
        EXCEPTION
            WHEN OTHERS THEN
                -- Si ça échoue, essayer avec guillemets
                BEGIN
                    EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD "NOM_CANDIDAT" VARCHAR2(200)';
                    DBMS_OUTPUT.PUT_LINE('✓ Colonne "NOM_CANDIDAT" ajoutée avec succès (avec guillemets)');
                EXCEPTION
                    WHEN OTHERS THEN
                        DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de la colonne: ' || SQLERRM);
                END;
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Colonne NOM_CANDIDAT existe déjà');
    END IF;
END;
/
PROMPT

-- Vérifier et créer l'index si nécessaire
DECLARE
    idx_exists NUMBER;
BEGIN
    SELECT COUNT(*) INTO idx_exists
    FROM USER_INDEXES
    WHERE TABLE_NAME = 'PLANNING'
    AND INDEX_NAME = 'IDX_PLANNING_CANDIDAT';
    
    IF idx_exists = 0 THEN
        BEGIN
            EXECUTE IMMEDIATE 'CREATE INDEX IDX_PLANNING_CANDIDAT ON PLANNING(NOM_CANDIDAT)';
            DBMS_OUTPUT.PUT_LINE('✓ Index IDX_PLANNING_CANDIDAT créé avec succès');
        EXCEPTION
            WHEN OTHERS THEN
                BEGIN
                    EXECUTE IMMEDIATE 'CREATE INDEX IDX_PLANNING_CANDIDAT ON PLANNING("NOM_CANDIDAT")';
                    DBMS_OUTPUT.PUT_LINE('✓ Index IDX_PLANNING_CANDIDAT créé avec succès (avec guillemets)');
                EXCEPTION
                    WHEN OTHERS THEN
                        DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de la création de l''index: ' || SQLERRM);
                END;
        END;
    ELSE
        DBMS_OUTPUT.PUT_LINE('✓ Index IDX_PLANNING_CANDIDAT existe déjà');
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
PROMPT Si des erreurs persistent, vérifiez que:
PROMPT 1. La table PLANNING existe
PROMPT 2. Vous avez les droits ALTER TABLE
PROMPT 3. La colonne n'existe pas sous un autre nom
