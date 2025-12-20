// Calendar functionality methods for Planning class
// Append these methods to planning.cpp

void Planning::onCalendarDateClicked(const QDate &date)
{
    qDebug() << "Calendar date clicked:" << date.toString("dd/MM/yyyy");
    
    // Get events for this date from database
    QSqlQuery query;
    query.prepare("SELECT p.ID_SEANCE, p.DATE_SEANCE, p.CONDIDAT, p.TYPE_SEANCE, "
                  "p.HEURE_DEBUT, p.HEURE_FIN, p.CIRCUIT, "
                  "c.NOM || ' ' || c.PRENOM as CANDIDAT_NOM, "
                  "e.NOM || ' ' || e.PRENOM as MONITEUR_NOM, "
                  "v.MATRICULE as VEHICULE "
                  "FROM PLANNING p "
                  "LEFT JOIN CONDIDAT c ON p.CONDIDAT = c.CIN "
                  "LEFT JOIN EMPLOYES e ON p.NOM_EMPLOYE = e.CIN "
                  "LEFT JOIN VEHICULE v ON p.VEHICULE = v.MATRICULE "
                  "WHERE p.DATE_SEANCE = :date "
                  "ORDER BY p.HEURE_DEBUT");
    query.bindValue(":date", date);
    
    if (!query.exec()) {
        qWarning() << "Error fetching events for date:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur", "Impossible de charger les événements pour cette date.");
        return;
    }
    
    // Build message with all events for this date
    QString message = QString("📅 <b>Événements pour %1</b><br><br>")
        .arg(date.toString("dddd dd MMMM yyyy"));
    
    int eventCount = 0;
    while (query.next()) {
        eventCount++;
        QString candidat = query.value("CANDIDAT_NOM").toString();
        QString type = query.value("TYPE_SEANCE").toString();
        QTime debut = query.value("HEURE_DEBUT").toTime();
        QTime fin = query.value("HEURE_FIN").toTime();
        QString circuit = query.value("CIRCUIT").toString();
        QString moniteur = query.value("MONITEUR_NOM").toString();
        QString vehicule = query.value("VEHICULE").toString();
        
        message += QString("<div style='background-color:#f0f0f0; padding:10px; margin:5px 0; border-left:4px solid #4CAF50; border-radius:5px;'>"
                          "<b>🕐 %1 - %2</b><br>"
                          "👤 Candidat: <b>%3</b><br>"
                          "📝 Type: <b>%4</b><br>"
                          "📍 Circuit: %5<br>")
            .arg(debut.toString("HH:mm"))
            .arg(fin.toString("HH:mm"))
            .arg(candidat)
            .arg(type)
            .arg(circuit);
        
        if (!moniteur.isEmpty()) {
            message += QString("👨‍🏫 Moniteur: %1<br>").arg(moniteur);
        }
        if (!vehicule.isEmpty()) {
            message += QString("🚗 Véhicule: %1<br>").arg(vehicule);
        }
        
        message += "</div>";
    }
    
    if (eventCount == 0) {
        message += "<p style='color:#666; text-align:center;'>Aucune séance planifiée pour cette date.</p>";
    } else {
        message += QString("<br><p style='text-align:center; color:#4CAF50;'><b>Total: %1 séance(s)</b></p>").arg(eventCount);
    }
    
    // Show message box with events
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Calendrier des Séances");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void Planning::highlightDatesWithEvents()
{
    if (!ui->calendarWidget) {
        return;
    }
    
    qDebug() << "Highlighting dates with events...";
    
    // Clear all date formats first
    ui->calendarWidget->setDateTextFormat(QDate(), QTextCharFormat());
    
    // Get all dates with events from database
    QSqlQuery query;
    query.prepare("SELECT DISTINCT DATE_SEANCE, COUNT(*) as EVENT_COUNT "
                  "FROM PLANNING "
                  "WHERE DATE_SEANCE IS NOT NULL "
                  "GROUP BY DATE_SEANCE");
    
    if (!query.exec()) {
        qWarning() << "Error fetching dates with events:" << query.lastError().text();
        return;
    }
    
    int totalHighlighted = 0;
    while (query.next()) {
        QDate eventDate = query.value("DATE_SEANCE").toDate();
        int eventCount = query.value("EVENT_COUNT").toInt();
        
        if (eventDate.isValid()) {
            QTextCharFormat format;
            
            // Different colors based on number of events
            if (eventCount >= 3) {
                // Many events - dark green
                format.setBackground(QColor(56, 142, 60));
                format.setForeground(Qt::white);
            } else if (eventCount >= 2) {
                // Multiple events - medium green
                format.setBackground(QColor(76, 175, 80));
                format.setForeground(Qt::white);
            } else {
                // Single event - light green
                format.setBackground(QColor(129, 199, 132));
                format.setForeground(Qt::black);
            }
            
            format.setFontWeight(QFont::Bold);
            ui->calendarWidget->setDateTextFormat(eventDate, format);
            totalHighlighted++;
        }
    }
    
    qDebug() << "Highlighted" << totalHighlighted << "dates with events";
    
    // Highlight today's date with special color
    QTextCharFormat todayFormat;
    todayFormat.setBackground(QColor(255, 206, 0, 100)); // Light yellow
    todayFormat.setForeground(QColor(0, 33, 87)); // Dark blue
    todayFormat.setFontWeight(QFont::Bold);
    ui->calendarWidget->setDateTextFormat(QDate::currentDate(), todayFormat);
}

void Planning::refreshCalendarView()
{
    if (!ui->calendarWidget) {
        return;
    }
    
    qDebug() << "Refreshing calendar view...";
    
    // Re-highlight dates with events
    highlightDatesWithEvents();
    
    // Update calendar widget
    ui->calendarWidget->update();
    ui->calendarWidget->repaint();
}
