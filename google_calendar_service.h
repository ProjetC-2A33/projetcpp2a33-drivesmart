#ifndef GOOGLE_CALENDAR_SERVICE_H
#define GOOGLE_CALENDAR_SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QString>
#include <QUrl>

class GoogleCalendarService : public QObject
{
    Q_OBJECT

public:
    explicit GoogleCalendarService(QObject *parent = nullptr);
    ~GoogleCalendarService();

    // Configuration
    void setApiKey(const QString &key);
    void setCalendarId(const QString &id);
    
    // Calendar operations
    void createEvent(const QString &summary, 
                     const QString &description,
                     const QDateTime &startTime,
                     const QDateTime &endTime,
                     const QString &location,
                     const QString &attendeeEmail = "");
    
    void updateEvent(const QString &eventId,
                     const QString &summary,
                     const QString &description,
                     const QString &location,
                     const QDateTime &startTime,
                     const QDateTime &endTime);
    
    void deleteEvent(const QString &eventId);
    
    void checkAvailability(const QDateTime &startTime, 
                          const QDateTime &endTime,
                          const QString &resourceEmail = "");
    
    void listEvents(const QDateTime &timeMin, const QDateTime &timeMax);
    
    // Getters
    QString getLastEventId() const { return lastCreatedEventId; }
    QString getLastCreatedEventId() const { return lastCreatedEventId; }
    bool isConfigured() const { return !apiKey.isEmpty() && !calendarId.isEmpty(); }

signals:
    void eventCreated(QString eventId, QString summary);
    void eventUpdated(QString eventId);
    void eventDeleted(QString eventId);
    void availabilityChecked(bool isAvailable, QJsonArray conflicts);
    void eventsListed(QJsonArray events);
    void errorOccurred(QString errorMessage);
    void networkError(QString errorDetails);

private slots:
    void onEventCreated(QNetworkReply *reply);
    void onEventUpdated(QNetworkReply *reply);
    void onEventDeleted(QNetworkReply *reply);
    void onAvailabilityChecked(QNetworkReply *reply);
    void onEventsListed(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString apiKey;
    QString calendarId;
    QString lastCreatedEventId;
    
    // Helper methods
    QString buildEventJson(const QString &summary,
                          const QString &description,
                          const QDateTime &startTime,
                          const QDateTime &endTime,
                          const QString &location,
                          const QString &attendeeEmail = "");
    
    QNetworkRequest createRequest(const QString &endpoint);
    void processNetworkReply(QNetworkReply *reply, const QString &operation);
};

#endif // GOOGLE_CALENDAR_SERVICE_H
