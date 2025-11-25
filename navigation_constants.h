#ifndef NAVIGATION_CONSTANTS_H
#define NAVIGATION_CONSTANTS_H

// Constantes pour les indices des pages dans le QStackedWidget
// Ordre correct dans mainwindow.cpp :
// Index 0: Candidat
// Index 1: Planning
// Index 2: Employee
// Index 3: Véhicule
// Index 4: Examen
// Index 5: Équipement

namespace Navigation {
    const int PAGE_CANDIDAT = 0;
    const int PAGE_PLANNING = 1;
    const int PAGE_EMPLOYEE = 2;
    const int PAGE_VEHICULE = 3;
    const int PAGE_EXAMEN = 4;
    const int PAGE_EQUIPEMENT = 5;
}

#endif // NAVIGATION_CONSTANTS_H

