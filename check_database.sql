-- Script de vérification de la base de données DriveSmart
-- Ce script vérifie l'existence des tables et affiche leur structure

SET SERVEROUTPUT ON;

-- Vérifier l'existence de la table CONDIDAT
DECLARE
    v_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_count
    FROM user_tables
    WHERE table_name = 'CONDIDAT';
    
    IF v_count > 0 THEN
        DBMS_OUTPUT.PUT_LINE('✓ Table CONDIDAT existe');
    ELSE
        DBMS_OUTPUT.PUT_LINE('✗ Table CONDIDAT N''EXISTE PAS - Veuillez exécuter create_tables_safe.sql');
    END IF;
END;
/

-- Afficher la structure de la table CONDIDAT si elle existe
SELECT 
    column_name,
    data_type,
    data_length,
    nullable,
    data_default
FROM user_tab_columns
WHERE table_name = 'CONDIDAT'
ORDER BY column_id;

-- Vérifier les contraintes
SELECT 
    constraint_name,
    constraint_type,
    search_condition
FROM user_constraints
WHERE table_name = 'CONDIDAT';

-- Vérifier les autres tables
DECLARE
    v_count NUMBER;
BEGIN
    DBMS_OUTPUT.PUT_LINE('========================================');
    DBMS_OUTPUT.PUT_LINE('Vérification des tables:');
    DBMS_OUTPUT.PUT_LINE('========================================');
    
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'VEHICULE';
    DBMS_OUTPUT.PUT_LINE('VEHICULE: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ Manquante' END);
    
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'EXAMEN';
    DBMS_OUTPUT.PUT_LINE('EXAMEN: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ Manquante' END);
    
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'EMPLOYEE';
    DBMS_OUTPUT.PUT_LINE('EMPLOYEE: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ Manquante' END);
    
    SELECT COUNT(*) INTO v_count FROM user_tables WHERE table_name = 'PLANNING';
    DBMS_OUTPUT.PUT_LINE('PLANNING: ' || CASE WHEN v_count > 0 THEN '✓ Existe' ELSE '✗ Manquante' END);
END;
/

-- Compter les candidats existants
SELECT COUNT(*) AS nombre_candidats FROM CONDIDAT;

