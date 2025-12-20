#include "local_calendar.h"

LocalCalendar::LocalCalendar(QObject *parent) : QObject(parent)
{
    qDebug() << "LocalCalendar initialized (100% FREE - No API key needed!)";
}

LocalCalendar::~LocalCalendar()
{
}

int LocalCalendar::createEvent(int planningId, 
                                const QString &title,
                                const QDateTime &start,
                                const QDateTime &end,
                                const QString &location,
                                const QString &description,
                                const QString &type)
{
    QSqlQuery query;
    query.prepare("INSERT INTO CALENDAR_EVENTS "
                  "(PLANNING_ID, EVENT_TITLE, EVENT_START, EVENT_END, "
                  "EVENT_LOCATION, EVENT_DESCRIPTION, EVENT_TYPE, EVENT_STATUS) "
                  "VALUES (:planning_id, :title, :start, :end, :location, :description, :type, 'CONFIRMED')");
    
    query.bindValue(":planning_id", planningId);
    query.bindValue(":title", title);
    query.bindValue(":start", start);
    query.bindValue(":end", end);
    query.bindValue(":location", location);
    query.bindValue(":description", description);
    query.bindValue(":type", type);
    
    if (!query.exec()) {
        lastError = query.lastError().text();
        qWarning() << "Failed to create calendar event:" << lastError;
        emit errorOccurred("Erreur lors de la création de l'événement: " + lastError);
        return -1;
    }
    
    // Get the generated event ID
    query.prepare("SELECT EVENT_ID FROM CALENDAR_EVENTS WHERE PLANNING_ID = :planning_id ORDER BY CREATED_AT DESC FETCH FIRST 1 ROW ONLY");
    query.bindValue(":planning_id", planningId);
    
    int eventId = -1;
    if (query.exec() && query.next()) {
        eventId = query.value(0).toInt();
        qDebug() << "Calendar event created successfully! ID:" << eventId;
        emit eventCreated(eventId, title);
    }
    
    return eventId;
}

bool LocalCalendar::updateEvent(int eventId,
                                 const QString &title,
                                 const QDateTime &start,
                                 const QDateTime &end,
                                 const QString &location,
                                 const QString &description)
{
    QSqlQuery query;
    query.prepare("UPDATE CALENDAR_EVENTS SET "
                  "EVENT_TITLE = :title, "
                  "EVENT_START = :start, "
                  "EVENT_END = :end, "
                  "EVENT_LOCATION = :location, "
                  "EVENT_DESCRIPTION = :description "
                  "WHERE EVENT_ID = :event_id");
    
    query.bindValue(":event_id", eventId);
    query.bindValue(":title", title);
    query.bindValue(":start", start);
    query.bindValue(":end", end);
    query.bindValue(":location", location);
    query.bindValue(":description", description);
    
    if (!query.exec()) {
        lastError = query.lastError().text();
        qWarning() << "Failed to update calendar event:" << lastError;
        emit errorOccurred("Erreur lors de la mise à jour: " + lastError);
        return false;
    }
    
    qDebug() << "Calendar event updated successfully! ID:" << eventId;
    emit eventUpdated(eventId);
    return true;
}

bool LocalCalendar::deleteEvent(int eventId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CALENDAR_EVENTS WHERE EVENT_ID = :event_id");
    query.bindValue(":event_id", eventId);
    
    if (!query.exec()) {
        lastError = query.lastError().text();
        qWarning() << "Failed to delete calendar event:" << lastError;
        emit errorOccurred("Erreur lors de la suppression: " + lastError);
        return false;
    }
    
    qDebug() << "Calendar event deleted successfully! ID:" << eventId;
    emit eventDeleted(eventId);
    return true;
}

bool LocalCalendar::deleteEventByPlanningId(int planningId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CALENDAR_EVENTS WHERE PLANNING_ID = :planning_id");
    query.bindValue(":planning_id", planningId);
    
    if (!query.exec()) {
        lastError = query.lastError().text();
        qWarning() << "Failed to delete calendar event by planning ID:" << lastError;
        return false;
    }
    
    qDebug() << "Calendar event deleted for planning ID:" << planningId;
    return true;
}

LocalCalendar::CalendarEvent LocalCalendar::getEventByPlanningId(int planningId)
{
    CalendarEvent event;
    event.eventId = -1;
    
    QSqlQuery query;
    query.prepare("SELECT EVENT_ID, PLANNING_ID, EVENT_TITLE, EVENT_START, EVENT_END, "
                  "EVENT_LOCATION, EVENT_DESCRIPTION, EVENT_TYPE, EVENT_STATUS "
                  "FROM CALENDAR_EVENTS WHERE PLANNING_ID = :planning_id");
    query.bindValue(":planning_id", planningId);
    
    if (query.exec() && query.next()) {
        event.eventId = query.value(0).toInt();
        event.planningId = query.value(1).toInt();
        event.title = query.value(2).toString();
        event.start = query.value(3).toDateTime();
        event.end = query.value(4).toDateTime();
        event.location = query.value(5).toString();
        event.description = query.value(6).toString();
        event.type = query.value(7).toString();
        event.status = query.value(8).toString();
    }
    
    return event;
}

QList<LocalCalendar::CalendarEvent> LocalCalendar::getEventsByDate(const QDate &date)
{
    QList<CalendarEvent> events;
    
    QSqlQuery query;
    query.prepare("SELECT EVENT_ID, PLANNING_ID, EVENT_TITLE, EVENT_START, EVENT_END, "
                  "EVENT_LOCATION, EVENT_DESCRIPTION, EVENT_TYPE, EVENT_STATUS "
                  "FROM CALENDAR_EVENTS "
                  "WHERE TRUNC(EVENT_START) = :date "
                  "ORDER BY EVENT_START");
    query.bindValue(":date", date);
    
    if (query.exec()) {
        while (query.next()) {
            CalendarEvent event;
            event.eventId = query.value(0).toInt();
            event.planningId = query.value(1).toInt();
            event.title = query.value(2).toString();
            event.start = query.value(3).toDateTime();
            event.end = query.value(4).toDateTime();
            event.location = query.value(5).toString();
            event.description = query.value(6).toString();
            event.type = query.value(7).toString();
            event.status = query.value(8).toString();
            events.append(event);
        }
    }
    
    return events;
}

QList<LocalCalendar::CalendarEvent> LocalCalendar::getEventsByDateRange(const QDate &startDate, const QDate &endDate)
{
    QList<CalendarEvent> events;
    
    QSqlQuery query;
    query.prepare("SELECT EVENT_ID, PLANNING_ID, EVENT_TITLE, EVENT_START, EVENT_END, "
                  "EVENT_LOCATION, EVENT_DESCRIPTION, EVENT_TYPE, EVENT_STATUS "
                  "FROM CALENDAR_EVENTS "
                  "WHERE EVENT_START BETWEEN :start_date AND :end_date "
                  "ORDER BY EVENT_START");
    query.bindValue(":start_date", QDateTime(startDate, QTime(0, 0, 0)));
    query.bindValue(":end_date", QDateTime(endDate.addDays(1), QTime(0, 0, 0)));
    
    if (query.exec()) {
        while (query.next()) {
            CalendarEvent event;
            event.eventId = query.value(0).toInt();
            event.planningId = query.value(1).toInt();
            event.title = query.value(2).toString();
            event.start = query.value(3).toDateTime();
            event.end = query.value(4).toDateTime();
            event.location = query.value(5).toString();
            event.description = query.value(6).toString();
            event.type = query.value(7).toString();
            event.status = query.value(8).toString();
            events.append(event);
        }
    }
    
    return events;
}

bool LocalCalendar::hasEvent(int planningId)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM CALENDAR_EVENTS WHERE PLANNING_ID = :planning_id");
    query.bindValue(":planning_id", planningId);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    
    return false;
}
