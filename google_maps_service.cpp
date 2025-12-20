#include "google_maps_service.h"
#include <QUrlQuery>
#include <QDebug>

GoogleMapsService::GoogleMapsService(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
}

GoogleMapsService::~GoogleMapsService()
{
}

void GoogleMapsService::setApiKey(const QString &key)
{
    m_apiKey = key;
}

// Geocoding - Convert address to coordinates
void GoogleMapsService::geocodeAddress(const QString &address)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API key not set");
        return;
    }
    
    if (address.isEmpty()) {
        emit errorOccurred("Address is empty");
        return;
    }
    
    QString url = buildGeocodeUrl(address);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &GoogleMapsService::onGeocodeReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GoogleMapsService::onNetworkError);
}

void GoogleMapsService::reverseGeocode(double latitude, double longitude)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API key not set");
        return;
    }
    
    QString url = buildReverseGeocodeUrl(latitude, longitude);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &GoogleMapsService::onReverseGeocodeReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GoogleMapsService::onNetworkError);
}

void GoogleMapsService::getStaticMap(double latitude, double longitude, int zoom, int width, int height, const QString &mapType)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API key not set");
        return;
    }
    
    QString url = buildStaticMapUrl(latitude, longitude, zoom, width, height, mapType);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &GoogleMapsService::onStaticMapReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GoogleMapsService::onNetworkError);
}

void GoogleMapsService::getStaticMapWithMarker(double latitude, double longitude, const QString &label, int zoom, int width, int height)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API key not set");
        return;
    }
    
    QString url = buildStaticMapUrl(latitude, longitude, zoom, width, height, "roadmap");
    
    // Add marker
    QString markerParam = QString("&markers=color:red%7Clabel:%1%7C%2,%3")
                              .arg(label.isEmpty() ? "" : label)
                              .arg(latitude, 0, 'f', 6)
                              .arg(longitude, 0, 'f', 6);
    url += markerParam;
    
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &GoogleMapsService::onStaticMapReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GoogleMapsService::onNetworkError);
}

void GoogleMapsService::getDistanceMatrix(const QString &origin, const QString &destination)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API key not set");
        return;
    }
    
    QString url = buildDistanceMatrixUrl(origin, destination);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &GoogleMapsService::onDistanceMatrixReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GoogleMapsService::onNetworkError);
}

void GoogleMapsService::getDirections(const QString &origin, const QString &destination, const QString &mode)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API key not set");
        return;
    }
    
    QString url = buildDirectionsUrl(origin, destination, mode);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &GoogleMapsService::onDirectionsReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GoogleMapsService::onNetworkError);
}

void GoogleMapsService::searchNearbyPlaces(double latitude, double longitude, int radius, const QString &type)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API key not set");
        return;
    }
    
    QString url = buildPlacesUrl(latitude, longitude, radius, type);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &GoogleMapsService::onPlacesReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GoogleMapsService::onNetworkError);
}

// Private slots
void GoogleMapsService::onGeocodeReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        
        QString status = obj["status"].toString();
        if (status == "OK") {
            QJsonArray results = obj["results"].toArray();
            if (!results.isEmpty()) {
                QJsonObject firstResult = results[0].toObject();
                QJsonObject geometry = firstResult["geometry"].toObject();
                QJsonObject location = geometry["location"].toObject();
                
                double lat = location["lat"].toDouble();
                double lng = location["lng"].toDouble();
                QString formattedAddress = firstResult["formatted_address"].toString();
                
                emit geocodeCompleted(lat, lng, formattedAddress);
            } else {
                emit errorOccurred("No results found");
            }
        } else {
            emit errorOccurred(QString("Geocoding failed: %1").arg(status));
        }
    } else {
        handleError(reply, "Geocoding");
    }
    
    reply->deleteLater();
}

void GoogleMapsService::onReverseGeocodeReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        
        QString status = obj["status"].toString();
        if (status == "OK") {
            QJsonArray results = obj["results"].toArray();
            if (!results.isEmpty()) {
                QJsonObject firstResult = results[0].toObject();
                QString address = firstResult["formatted_address"].toString();
                emit reverseGeocodeCompleted(address);
            } else {
                emit errorOccurred("No address found");
            }
        } else {
            emit errorOccurred(QString("Reverse geocoding failed: %1").arg(status));
        }
    } else {
        handleError(reply, "Reverse Geocoding");
    }
    
    reply->deleteLater();
}

void GoogleMapsService::onStaticMapReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QPixmap pixmap;
        if (pixmap.loadFromData(data)) {
            emit staticMapReady(pixmap);
        } else {
            emit errorOccurred("Failed to load map image");
        }
    } else {
        handleError(reply, "Static Map");
    }
    
    reply->deleteLater();
}

void GoogleMapsService::onDistanceMatrixReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        
        QString status = obj["status"].toString();
        if (status == "OK") {
            QJsonArray rows = obj["rows"].toArray();
            if (!rows.isEmpty()) {
                QJsonObject row = rows[0].toObject();
                QJsonArray elements = row["elements"].toArray();
                if (!elements.isEmpty()) {
                    QJsonObject element = elements[0].toObject();
                    QString elementStatus = element["status"].toString();
                    
                    if (elementStatus == "OK") {
                        QJsonObject distance = element["distance"].toObject();
                        QJsonObject duration = element["duration"].toObject();
                        
                        int distanceMeters = distance["value"].toInt();
                        QString distanceText = distance["text"].toString();
                        int durationSeconds = duration["value"].toInt();
                        QString durationText = duration["text"].toString();
                        
                        emit distanceMatrixReady(distanceMeters, durationSeconds, distanceText, durationText);
                    } else {
                        emit errorOccurred(QString("Distance calculation failed: %1").arg(elementStatus));
                    }
                }
            }
        } else {
            emit errorOccurred(QString("Distance Matrix request failed: %1").arg(status));
        }
    } else {
        handleError(reply, "Distance Matrix");
    }
    
    reply->deleteLater();
}

void GoogleMapsService::onDirectionsReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        
        QString status = obj["status"].toString();
        if (status == "OK") {
            QJsonArray routes = obj["routes"].toArray();
            if (!routes.isEmpty()) {
                QJsonObject route = routes[0].toObject();
                emit directionsReady(route);
            } else {
                emit errorOccurred("No routes found");
            }
        } else {
            emit errorOccurred(QString("Directions request failed: %1").arg(status));
        }
    } else {
        handleError(reply, "Directions");
    }
    
    reply->deleteLater();
}

void GoogleMapsService::onPlacesReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        
        QString status = obj["status"].toString();
        if (status == "OK") {
            QJsonArray results = obj["results"].toArray();
            emit placesFound(results);
        } else {
            emit errorOccurred(QString("Places search failed: %1").arg(status));
        }
    } else {
        handleError(reply, "Places Search");
    }
    
    reply->deleteLater();
}

void GoogleMapsService::onNetworkError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        qWarning() << "Network error:" << error << reply->errorString();
    }
}

// Helper methods
QString GoogleMapsService::buildGeocodeUrl(const QString &address)
{
    QString baseUrl = "https://maps.googleapis.com/maps/api/geocode/json";
    QUrl url(baseUrl);
    QUrlQuery query;
    query.addQueryItem("address", address);
    query.addQueryItem("key", m_apiKey);
    url.setQuery(query);
    return url.toString();
}

QString GoogleMapsService::buildReverseGeocodeUrl(double latitude, double longitude)
{
    QString baseUrl = "https://maps.googleapis.com/maps/api/geocode/json";
    QUrl url(baseUrl);
    QUrlQuery query;
    query.addQueryItem("latlng", QString("%1,%2").arg(latitude, 0, 'f', 6).arg(longitude, 0, 'f', 6));
    query.addQueryItem("key", m_apiKey);
    url.setQuery(query);
    return url.toString();
}

QString GoogleMapsService::buildStaticMapUrl(double latitude, double longitude, int zoom, int width, int height, const QString &mapType)
{
    QString baseUrl = "https://maps.googleapis.com/maps/api/staticmap";
    QUrl url(baseUrl);
    QUrlQuery query;
    query.addQueryItem("center", QString("%1,%2").arg(latitude, 0, 'f', 6).arg(longitude, 0, 'f', 6));
    query.addQueryItem("zoom", QString::number(zoom));
    query.addQueryItem("size", QString("%1x%2").arg(width).arg(height));
    query.addQueryItem("maptype", mapType);
    query.addQueryItem("key", m_apiKey);
    url.setQuery(query);
    return url.toString();
}

QString GoogleMapsService::buildDistanceMatrixUrl(const QString &origin, const QString &destination)
{
    QString baseUrl = "https://maps.googleapis.com/maps/api/distancematrix/json";
    QUrl url(baseUrl);
    QUrlQuery query;
    query.addQueryItem("origins", origin);
    query.addQueryItem("destinations", destination);
    query.addQueryItem("key", m_apiKey);
    url.setQuery(query);
    return url.toString();
}

QString GoogleMapsService::buildDirectionsUrl(const QString &origin, const QString &destination, const QString &mode)
{
    QString baseUrl = "https://maps.googleapis.com/maps/api/directions/json";
    QUrl url(baseUrl);
    QUrlQuery query;
    query.addQueryItem("origin", origin);
    query.addQueryItem("destination", destination);
    query.addQueryItem("mode", mode);
    query.addQueryItem("key", m_apiKey);
    url.setQuery(query);
    return url.toString();
}

QString GoogleMapsService::buildPlacesUrl(double latitude, double longitude, int radius, const QString &type)
{
    QString baseUrl = "https://maps.googleapis.com/maps/api/place/nearbysearch/json";
    QUrl url(baseUrl);
    QUrlQuery query;
    query.addQueryItem("location", QString("%1,%2").arg(latitude, 0, 'f', 6).arg(longitude, 0, 'f', 6));
    query.addQueryItem("radius", QString::number(radius));
    if (!type.isEmpty()) {
        query.addQueryItem("type", type);
    }
    query.addQueryItem("key", m_apiKey);
    url.setQuery(query);
    return url.toString();
}

void GoogleMapsService::handleError(QNetworkReply *reply, const QString &context)
{
    QString errorMsg = QString("%1 error: %2").arg(context).arg(reply->errorString());
    qWarning() << errorMsg;
    emit errorOccurred(errorMsg);
}
