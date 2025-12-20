#include "google_calendar_service.h"
#include <QDebug>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GoogleCalendarService::GoogleCalendarService(QObject *parent)
    : QObject(parent),
      networkManager(new QNetworkAccessManager(this)),
      apiKey(""),
      calendarId("primary"),
      lastCreatedEventId("")
{
    qDebug() << "GoogleCalendarService initialized";
}

GoogleCalendarService::~GoogleCalendarService()
{
    qDebug() << "GoogleCalendarService destroyed";
}

void GoogleCalendarService::setApiKey(const QString &key)
{
    apiKey = key;
    qDebug() << "Google Calendar API key configured";
}

void GoogleCalendarService::setCalendarId(const QString &id)
{
    calendarId = id;
    qDebug() << "Calendar ID set to:" << id;
}

QNetworkRequest GoogleCalendarService::createRequest(const QString &endpoint)
{
    QString baseUrl = "https://www.googleapis.com/calendar/v3";
    QUrl url(baseUrl + endpoint);
    
    QUrlQuery query;
    query.addQueryItem("key", apiKey);
    url.setQuery(query);
    
    qDebug() << "Creating request for URL:" << url.toString();
    qDebug() << "API Key configured:" << (!apiKey.isEmpty() ? "Yes" : "No");
    qDebug() << "API Key length:" << apiKey.length();
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    return request;
}

QString GoogleCalendarService::buildEventJson(const QString &summary,
                                              const QString &description,
                                              const QDateTime &startTime,
                                              const QDateTime &endTime,
                                              const QString &location,
                                              const QString &attendeeEmail)
{
    QJsonObject event;
    event["summary"] = summary;
    event["description"] = description;
    event["location"] = location;
    
    // Start time
    QJsonObject start;
    start["dateTime"] = startTime.toString(Qt::ISODate);
    start["timeZone"] = "Africa/Tunis";
    event["start"] = start;
    
    // End time
    QJsonObject end;
    end["dateTime"] = endTime.toString(Qt::ISODate);
    end["timeZone"] = "Africa/Tunis";
    event["end"] = end;
    
    // Attendees (optional)
    if (!attendeeEmail.isEmpty()) {
        QJsonArray attendees;
        QJsonObject attendee;
        attendee["email"] = attendeeEmail;
        attendees.append(attendee);
        event["attendees"] = attendees;
    }
    
    // Reminders
    QJsonObject reminders;
    reminders["useDefault"] = false;
    QJsonArray overrides;
    
    QJsonObject reminder1;
    reminder1["method"] = "email";
    reminder1["minutes"] = 24 * 60; // 24 hours before
    overrides.append(reminder1);
    
    QJsonObject reminder2;
    reminder2["method"] = "popup";
    reminder2["minutes"] = 60; // 1 hour before
    overrides.append(reminder2);
    
    reminders["overrides"] = overrides;
    event["reminders"] = reminders;
    
    QJsonDocument doc(event);
    return doc.toJson(QJsonDocument::Compact);
}

void GoogleCalendarService::createEvent(const QString &summary,
                                       const QString &description,
                                       const QDateTime &startTime,
                                       const QDateTime &endTime,
                                       const QString &location,
                                       const QString &attendeeEmail)
{
    if (!isConfigured()) {
        emit errorOccurred("Google Calendar not configured. Please set API key.");
        return;
    }
    
    qDebug() << "Creating calendar event:" << summary;
    
    QString endpoint = QString("/calendars/%1/events").arg(calendarId);
    QNetworkRequest request = createRequest(endpoint);
    
    QString jsonData = buildEventJson(summary, description, startTime, endTime, location, attendeeEmail);
    
    QNetworkReply *reply = networkManager->post(request, jsonData.toUtf8());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onEventCreated(reply);
        reply->deleteLater();
    });
}

void GoogleCalendarService::updateEvent(const QString &eventId,
                                       const QString &summary,
                                       const QString &description,
                                        const QString &location,
                                        const QDateTime &startTime,
                                        const QDateTime &endTime)
{
    if (!isConfigured()) {
        emit errorOccurred("Google Calendar not configured. Please set API key.");
        return;
    }
    
    if (eventId.isEmpty()) {
        emit errorOccurred("Event ID is required for update.");
        return;
    }
    
    qDebug() << "Updating calendar event:" << eventId;
    
    QString endpoint = QString("/calendars/%1/events/%2").arg(calendarId, eventId);
    QNetworkRequest request = createRequest(endpoint);
    
    QString jsonData = buildEventJson(summary, description, startTime, endTime, location);
    
    QNetworkReply *reply = networkManager->put(request, jsonData.toUtf8());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onEventUpdated(reply);
        reply->deleteLater();
    });
}

void GoogleCalendarService::deleteEvent(const QString &eventId)
{
    if (!isConfigured()) {
        emit errorOccurred("Google Calendar not configured. Please set API key.");
        return;
    }
    
    if (eventId.isEmpty()) {
        emit errorOccurred("Event ID is required for deletion.");
        return;
    }
    
    qDebug() << "Deleting calendar event:" << eventId;
    
    QString endpoint = QString("/calendars/%1/events/%2").arg(calendarId, eventId);
    QNetworkRequest request = createRequest(endpoint);
    
    QNetworkReply *reply = networkManager->deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onEventDeleted(reply);
        reply->deleteLater();
    });
}

void GoogleCalendarService::checkAvailability(const QDateTime &startTime,
                                              const QDateTime &endTime,
                                              const QString &resourceEmail)
{    Q_UNUSED(resourceEmail);    if (!isConfigured()) {
        emit errorOccurred("Google Calendar not configured. Please set API key.");
        return;
    }
    
    qDebug() << "Checking availability from" << startTime << "to" << endTime;
    
    QString endpoint = QString("/calendars/%1/events").arg(calendarId);
    QUrl url("https://www.googleapis.com/calendar/v3" + endpoint);
    
    QUrlQuery query;
    query.addQueryItem("key", apiKey);
    query.addQueryItem("timeMin", startTime.toString(Qt::ISODate));
    query.addQueryItem("timeMax", endTime.toString(Qt::ISODate));
    query.addQueryItem("singleEvents", "true");
    query.addQueryItem("orderBy", "startTime");
    url.setQuery(query);
    
    QNetworkRequest request(url);
    
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onAvailabilityChecked(reply);
        reply->deleteLater();
    });
}

void GoogleCalendarService::listEvents(const QDateTime &timeMin, const QDateTime &timeMax)
{
    if (!isConfigured()) {
        emit errorOccurred("Google Calendar not configured. Please set API key.");
        return;
    }
    
    qDebug() << "Listing events from" << timeMin << "to" << timeMax;
    
    QString endpoint = QString("/calendars/%1/events").arg(calendarId);
    QUrl url("https://www.googleapis.com/calendar/v3" + endpoint);
    
    QUrlQuery query;
    query.addQueryItem("key", apiKey);
    query.addQueryItem("timeMin", timeMin.toString(Qt::ISODate));
    query.addQueryItem("timeMax", timeMax.toString(Qt::ISODate));
    query.addQueryItem("singleEvents", "true");
    query.addQueryItem("orderBy", "startTime");
    query.addQueryItem("maxResults", "50");
    url.setQuery(query);
    
    QNetworkRequest request(url);
    
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onEventsListed(reply);
        reply->deleteLater();
    });
}

void GoogleCalendarService::onEventCreated(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = "Failed to create event: " + reply->errorString();
        qDebug() << errorMsg;        qWarning() << "HTTP Status Code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qWarning() << "Response body:" << reply->readAll();        emit errorOccurred(errorMsg);
        return;
    }
    
    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject obj = doc.object();
    
    lastCreatedEventId = obj["id"].toString();
    QString summary = obj["summary"].toString();
    
    qDebug() << "Event created successfully. ID:" << lastCreatedEventId;
    emit eventCreated(lastCreatedEventId, summary);
}

void GoogleCalendarService::onEventUpdated(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = "Failed to update event: " + reply->errorString();
        qDebug() << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }
    
    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject obj = doc.object();
    
    QString eventId = obj["id"].toString();
    
    qDebug() << "Event updated successfully. ID:" << eventId;
    emit eventUpdated(eventId);
}

void GoogleCalendarService::onEventDeleted(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = "Failed to delete event: " + reply->errorString();
        qDebug() << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }
    
    qDebug() << "Event deleted successfully";
    emit eventDeleted("");
}

void GoogleCalendarService::onAvailabilityChecked(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = "Failed to check availability: " + reply->errorString();
        qDebug() << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }
    
    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject obj = doc.object();
    QJsonArray items = obj["items"].toArray();
    
    bool isAvailable = items.isEmpty();
    
    qDebug() << "Availability check: " << (isAvailable ? "Available" : "Conflicts found");
    qDebug() << "Found" << items.size() << "conflicting events";
    
    emit availabilityChecked(isAvailable, items);
}

void GoogleCalendarService::onEventsListed(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = "Failed to list events: " + reply->errorString();
        qDebug() << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }
    
    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject obj = doc.object();
    QJsonArray items = obj["items"].toArray();
    
    qDebug() << "Listed" << items.size() << "events";
    emit eventsListed(items);
}
