#ifndef LOCAL_CALENDAR_H
#define LOCAL_CALENDAR_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/**
 * @class LocalCalendar
 * @brief Local database-based calendar system (no API key needed!)
 * 
 * Manages calendar events stored in Oracle database
 * 100% FREE - No external API required
 */
class LocalCalendar : public QObject
{
    Q_OBJECT

public:
    struct CalendarEvent {
        int eventId;
        int planningId;
        QString title;
        QDateTime start;
        QDateTime end;
        QString location;
        QString description;
        QString type;
        QString status;
    };

    explicit LocalCalendar(QObject *parent = nullptr);
    ~LocalCalendar();

    /**
     * @brief Create a calendar event from planning session
     * @return Event ID if successful, -1 if failed
     */
    int createEvent(int planningId, 
                    const QString &title,
                    const QDateTime &start,
                    const QDateTime &end,
                    const QString &location = "",
                    const QString &description = "",
                    const QString &type = "SEANCE");

    /**
     * @brief Update an existing calendar event
     */
    bool updateEvent(int eventId,
                     const QString &title,
                     const QDateTime &start,
                     const QDateTime &end,
                     const QString &location = "",
                     const QString &description = "");

    /**
     * @brief Delete a calendar event
     */
    bool deleteEvent(int eventId);

    /**
     * @brief Delete event by planning ID
     */
    bool deleteEventByPlanningId(int planningId);

    /**
     * @brief Get event by planning ID
     */
    CalendarEvent getEventByPlanningId(int planningId);

    /**
     * @brief Get all events for a specific date
     */
    QList<CalendarEvent> getEventsByDate(const QDate &date);

    /**
     * @brief Get all events in date range
     */
    QList<CalendarEvent> getEventsByDateRange(const QDate &startDate, const QDate &endDate);

    /**
     * @brief Check if planning has associated event
     */
    bool hasEvent(int planningId);

signals:
    void eventCreated(int eventId, const QString &title);
    void eventUpdated(int eventId);
    void eventDeleted(int eventId);
    void errorOccurred(const QString &error);

private:
    QString lastError;
};

#endif // LOCAL_CALENDAR_H
