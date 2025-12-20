// Search, Sort, and Export functionality for Planning
// Add these methods to planning.cpp

void Planning::on_recherche_3_textChanged()
{
    currentSearchText = ui->recherche_3->toPlainText().trimmed();
    qDebug() << "Search text changed:" << currentSearchText;
    refreshTableWithFilters();
}

void Planning::on_tri_3_currentIndexChanged(int index)
{
    currentSortIndex = index;
    qDebug() << "Sort index changed:" << index;
    refreshTableWithFilters();
}

void Planning::refreshTableWithFilters()
{
    // Build query with search and sort
    QString queryStr = "SELECT P.ID_SEANCE, "
                      "TO_CHAR(P.DATE_SEANCE, 'DD/MM/YYYY') AS DATE_SEANCE, "
                      "P.NOM_CANDIDAT, "
                      "P.TYPE_SEANCE, "
                      "TO_CHAR(P.HEURE_DEBUT, 'HH24:MI') AS DEBUT, "
                      "TO_CHAR(P.HEURE_FIN, 'HH24:MI') AS FIN, "
                      "P.ADRESSE_CIRCUIT, "
                      "P.MATRICULE, "
                      "P.CIN_CONDIDAT, "
                      "P.CIN_EMPLOYEE "
                      "FROM PLANNING P ";
    
    // Add search filter
    bool hasWhere = false;
    if (!currentSearchText.isEmpty()) {
        queryStr += "WHERE (UPPER(P.NOM_CANDIDAT) LIKE :search "
                   "OR UPPER(P.TYPE_SEANCE) LIKE :search "
                   "OR UPPER(P.ADRESSE_CIRCUIT) LIKE :search "
                   "OR UPPER(P.MATRICULE) LIKE :search) ";
        hasWhere = true;
    }
    
    // Add sorting
    switch (currentSortIndex) {
        case 1: // Date (Plus récent)
            queryStr += "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC";
            break;
        case 2: // Date (Plus ancien)
            queryStr += "ORDER BY P.DATE_SEANCE ASC, P.HEURE_DEBUT ASC";
            break;
        case 3: // Candidat (A-Z)
            queryStr += "ORDER BY P.NOM_CANDIDAT ASC";
            break;
        case 4: // Candidat (Z-A)
            queryStr += "ORDER BY P.NOM_CANDIDAT DESC";
            break;
        case 5: // Type (Code/Conduit)
            queryStr += "ORDER BY P.TYPE_SEANCE ASC";
            break;
        case 6: // Heure (Plus tôt)
            queryStr += "ORDER BY P.HEURE_DEBUT ASC";
            break;
        default: // Default sorting
            queryStr += "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC";
            break;
    }
    
    QSqlQueryModel *model = new QSqlQueryModel();
    
    QSqlQuery query;
    query.prepare(queryStr);
    
    if (!currentSearchText.isEmpty()) {
        QString searchPattern = "%" + currentSearchText.toUpper() + "%";
        query.bindValue(":search", searchPattern);
    }
    
    if (!query.exec()) {
        qWarning() << "Error executing filtered query:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur", "Erreur lors de la recherche: " + query.lastError().text());
        delete model;
        return;
    }
    
    model->setQuery(query);
    
    int rows = model->rowCount();
    int cols = model->columnCount();
    
    ui->tab_3->setRowCount(rows);
    ui->tab_3->setColumnCount(cols + 1);
    
    QStringList headers;
    headers << "ID" << "Date séance" << "condidat" << "type" << "debut" << "fin" << "circuit" << "véhicule" << "CIN_CAND" << "CIN_EMP" << "Actions";
    ui->tab_3->setHorizontalHeaderLabels(headers);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QVariant v = model->data(model->index(r, c));
            ui->tab_3->setItem(r, c, new QTableWidgetItem(v.toString()));
        }
        bool ok = false;
        int id = model->data(model->index(r, 0)).toInt(&ok);
        ui->tab_3->setCellWidget(r, cols, createActionsCell(r, ok ? id : -1));
    }
    
    ui->tab_3->setColumnHidden(8, true);
    ui->tab_3->setColumnHidden(9, true);
    
    delete model;
    
    // Refresh calendar view
    refreshCalendarView();
    
    qDebug() << "Table refreshed with" << rows << "rows (search:" << currentSearchText << ", sort:" << currentSortIndex << ")";
}

void Planning::on_pushButton_8_clicked()
{
    qDebug() << "Export to PDF button clicked";
    
    // Ask user for save location
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter Planning en PDF",
                                                    QDir::homePath() + "/planning_" + QDate::currentDate().toString("yyyy-MM-dd") + ".pdf",
                                                    "PDF Files (*.pdf)");
    
    if (fileName.isEmpty()) {
        qDebug() << "Export cancelled by user";
        return;
    }
    
    // Ensure .pdf extension
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }
    
    // Create printer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape); // Landscape for wide table
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    
    // Create HTML document for PDF
    QString html = "<html><head><style>"
                  "body { font-family: Arial, sans-serif; }"
                  "h1 { color: #002157; text-align: center; margin-bottom: 20px; }"
                  "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                  "th { background-color: #ffce00; color: #002157; padding: 10px; text-align: left; border: 1px solid #ddd; font-weight: bold; }"
                  "td { padding: 8px; border: 1px solid #ddd; }"
                  "tr:nth-child(even) { background-color: #f9f9f9; }"
                  ".footer { margin-top: 30px; text-align: center; font-size: 10px; color: #666; }"
                  "</style></head><body>";
    
    html += "<h1>📋 Planning des Séances de Conduite - DriveSmart</h1>";
    html += "<p style='text-align:center;'><strong>Date d'export:</strong> " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>";
    
    // Get current table data
    QSqlQuery query;
    QString queryStr = "SELECT P.ID_SEANCE, "
                      "TO_CHAR(P.DATE_SEANCE, 'DD/MM/YYYY') AS DATE_SEANCE, "
                      "P.NOM_CANDIDAT, "
                      "P.TYPE_SEANCE, "
                      "TO_CHAR(P.HEURE_DEBUT, 'HH24:MI') AS DEBUT, "
                      "TO_CHAR(P.HEURE_FIN, 'HH24:MI') AS FIN, "
                      "P.ADRESSE_CIRCUIT, "
                      "P.MATRICULE "
                      "FROM PLANNING P ";
    
    // Apply current filters
    if (!currentSearchText.isEmpty()) {
        queryStr += "WHERE (UPPER(P.NOM_CANDIDAT) LIKE :search "
                   "OR UPPER(P.TYPE_SEANCE) LIKE :search "
                   "OR UPPER(P.ADRESSE_CIRCUIT) LIKE :search "
                   "OR UPPER(P.MATRICULE) LIKE :search) ";
    }
    
    // Apply current sorting
    switch (currentSortIndex) {
        case 1: queryStr += "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC"; break;
        case 2: queryStr += "ORDER BY P.DATE_SEANCE ASC, P.HEURE_DEBUT ASC"; break;
        case 3: queryStr += "ORDER BY P.NOM_CANDIDAT ASC"; break;
        case 4: queryStr += "ORDER BY P.NOM_CANDIDAT DESC"; break;
        case 5: queryStr += "ORDER BY P.TYPE_SEANCE ASC"; break;
        case 6: queryStr += "ORDER BY P.HEURE_DEBUT ASC"; break;
        default: queryStr += "ORDER BY P.DATE_SEANCE DESC, P.HEURE_DEBUT DESC"; break;
    }
    
    query.prepare(queryStr);
    
    if (!currentSearchText.isEmpty()) {
        QString searchPattern = "%" + currentSearchText.toUpper() + "%";
        query.bindValue(":search", searchPattern);
    }
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'export: " + query.lastError().text());
        return;
    }
    
    // Build table
    html += "<table>";
    html += "<tr><th>ID</th><th>Date</th><th>Candidat</th><th>Type</th><th>Début</th><th>Fin</th><th>Circuit</th><th>Véhicule</th></tr>";
    
    int rowCount = 0;
    while (query.next()) {
        rowCount++;
        html += "<tr>";
        html += "<td>" + query.value(0).toString() + "</td>";
        html += "<td>" + query.value(1).toString() + "</td>";
        html += "<td>" + query.value(2).toString() + "</td>";
        html += "<td>" + query.value(3).toString() + "</td>";
        html += "<td>" + query.value(4).toString() + "</td>";
        html += "<td>" + query.value(5).toString() + "</td>";
        html += "<td>" + query.value(6).toString() + "</td>";
        html += "<td>" + query.value(7).toString() + "</td>";
        html += "</tr>";
    }
    
    html += "</table>";
    html += "<div class='footer'>";
    html += "<p><strong>Total: " + QString::number(rowCount) + " séance(s)</strong></p>";
    html += "<p>Généré par DriveSmart - Système de Gestion d'Auto-École</p>";
    html += "</div>";
    html += "</body></html>";
    
    // Print to PDF
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
    
    qDebug() << "PDF exported successfully to:" << fileName;
    QMessageBox::information(this, "Succès", 
                            QString("Planning exporté avec succès!\n\nFichier: %1\nNombre de séances: %2")
                            .arg(fileName)
                            .arg(rowCount));
}
