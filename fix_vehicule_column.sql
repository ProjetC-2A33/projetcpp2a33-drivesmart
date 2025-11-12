-- Script pour corriger la colonne VEHICULE dans la table PLANNING
-- Vérifie si VEHICULE ou MATRICULE_VEHICULE existe et ajoute/corrige selon le cas

SET SERVEROUTPUT ON;

PROMPT ========================================
PROMPT CORRECTION DE LA COLONNE VEHICULE
PROMPT ========================================
PROMPT

-- Vérifier l'état actuel des colonnes
DECLARE
    vehicule_exists NUMBER;
    matricule_exists NUMBER;
BEGIN
    -- Vérifier si VEHICULE existe
    SELECT COUNT(*) INTO vehicule_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'VEHICULE';
    
    -- Vérifier si MATRICULE_VEHICULE existe
    SELECT COUNT(*) INTO matricule_exists
    FROM USER_TAB_COLUMNS
    WHERE TABLE_NAME = 'PLANNING'
    AND UPPER(COLUMN_NAME) = 'MATRICULE_VEHICULE';
    
    DBMS_OUTPUT.PUT_LINE('État actuel:');
    DBMS_OUTPUT.PUT_LINE('  VEHICULE: ' || CASE WHEN vehicule_exists > 0 THEN 'EXISTE' ELSE 'N''EXISTE PAS' END);
    DBMS_OUTPUT.PUT_LINE('  MATRICULE_VEHICULE: ' || CASE WHEN matricule_exists > 0 THEN 'EXISTE' ELSE 'N''EXISTE PAS' END);
    DBMS_OUTPUT.PUT_LINE('');
    
    -- Si VEHICULE n'existe pas mais MATRICULE_VEHICULE existe, créer un synonyme ou ajouter VEHICULE
    IF vehicule_exists = 0 THEN
        IF matricule_exists > 0 THEN
            -- Option 1: Ajouter VEHICULE comme nouvelle colonne
            BEGIN
                EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD "VEHICULE" VARCHAR2(20)';
                DBMS_OUTPUT.PUT_LINE('✓ Colonne VEHICULE ajoutée');
                
                -- Option 2: Copier les données de MATRICULE_VEHICULE vers VEHICULE
                BEGIN
                    EXECUTE IMMEDIATE 'UPDATE PLANNING SET "VEHICULE" = MATRICULE_VEHICULE WHERE MATRICULE_VEHICULE IS NOT NULL';
                    DBMS_OUTPUT.PUT_LINE('✓ Données copiées de MATRICULE_VEHICULE vers VEHICULE');
                EXCEPTION
                    WHEN OTHERS THEN
                        DBMS_OUTPUT.PUT_LINE('ℹ Pas de données à copier ou erreur: ' || SQLERRM);
                END;
            EXCEPTION
                WHEN OTHERS THEN
                    DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de l''ajout de VEHICULE: ' || SQLERRM);
            END;
        ELSE
            -- Aucune des deux colonnes n'existe, créer VEHICULE
            BEGIN
                EXECUTE IMMEDIATE 'ALTER TABLE PLANNING ADD "VEHICULE" VARCHAR2(20)';
                DBMS_OUTPUT.PUT_LINE('✓ Colonne VEHICULE créée');
            EXCEPTION
                WHEN OTHERS THEN
                    DBMS_OUTPUT.PUT_LINE('✗ Erreur lors de la création de VEHICULE: ' || SQLERRM);
            END;
        END IF;
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
PROMPT La colonne VEHICULE a été ajoutée/corrigée.
PROMPT Vous pouvez maintenant utiliser l'application sans erreur.

