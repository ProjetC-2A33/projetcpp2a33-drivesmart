#include "openstreetmap_service.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QUrl>
#include <QPainter>
#include <QDebug>
#include <QtMath>

OpenStreetMapService::OpenStreetMapService(QObject *parent)
    : QObject(parent),
      networkManager(new QNetworkAccessManager(this)),
      pendingLatitude(0.0),
      pendingLongitude(0.0),
      pendingZoom(15),
      pendingWidth(800),
      pendingHeight(600)
{
}

OpenStreetMapService::~OpenStreetMapService()
{
}

void OpenStreetMapService::geocodeAddress(const QString &address)
{
    if (address.isEmpty()) {
        emit errorOccurred("Address cannot be empty");
        return;
    }

    pendingAddress = address;

    // Nominatim geocoding API (FREE, no key required)
    // Documentation: https://nominatim.org/release-docs/develop/api/Search/
    QUrl url("https://nominatim.openstreetmap.org/search");
    QUrlQuery query;
    query.addQueryItem("q", address);
    query.addQueryItem("format", "json");
    query.addQueryItem("limit", "1");
    query.addQueryItem("addressdetails", "1");
    url.setQuery(query);

    QNetworkRequest request{url};
    
    // REQUIRED: User-Agent header (Nominatim usage policy)
    request.setRawHeader("User-Agent", "DriveSmart/1.0 (Educational Project)");
    
    qDebug() << "Geocoding address:" << address;
    qDebug() << "Request URL:" << url.toString();

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &OpenStreetMapService::handleGeocodeReply);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError) {
        emit errorOccurred("Network error: " + reply->errorString());
        reply->deleteLater();
    });
}

void OpenStreetMapService::handleGeocodeReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Geocoding failed: " + reply->errorString());
        return;
    }

    QByteArray responseData = reply->readAll();
    qDebug() << "Geocode response:" << responseData;

    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    if (!doc.isArray()) {
        emit errorOccurred("Invalid response format from Nominatim");
        return;
    }

    QJsonArray results = doc.array();
    if (results.isEmpty()) {
        emit errorOccurred("Address not found: " + pendingAddress);
        return;
    }

    QJsonObject firstResult = results[0].toObject();
    double lat = firstResult["lat"].toString().toDouble();
    double lon = firstResult["lon"].toString().toDouble();
    QString displayName = firstResult["display_name"].toString();

    qDebug() << "Geocoded:" << pendingAddress << "to" << lat << "," << lon;
    qDebug() << "Display name:" << displayName;

    emit geocodeCompleted(pendingAddress, lat, lon, displayName);
}

void OpenStreetMapService::getStaticMapWithMarker(double latitude, double longitude,
                                                   int zoom, int width, int height)
{
    if (latitude < -90 || latitude > 90 || longitude < -180 || longitude > 180) {
        emit errorOccurred("Invalid coordinates");
        return;
    }

    pendingLatitude = latitude;
    pendingLongitude = longitude;
    pendingZoom = qBound(1, zoom, 19);
    pendingWidth = width;
    pendingHeight = height;

    // Get OpenStreetMap tile URL
    QString tileUrl = getTileUrl(latitude, longitude, pendingZoom);
    
    qDebug() << "Fetching map tile:" << tileUrl;
    qDebug() << "Center:" << latitude << "," << longitude << "Zoom:" << pendingZoom;

    QNetworkRequest request{QUrl(tileUrl)};
    request.setRawHeader("User-Agent", "DriveSmart/1.0 (Educational Project)");

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &OpenStreetMapService::handleMapTileReply);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError) {
        emit errorOccurred("Map tile error: " + reply->errorString());
        reply->deleteLater();
    });
}

void OpenStreetMapService::handleMapTileReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Failed to load map: " + reply->errorString());
        return;
    }

    QByteArray imageData = reply->readAll();
    QPixmap mapImage;
    
    if (!mapImage.loadFromData(imageData)) {
        emit errorOccurred("Failed to parse map image");
        return;
    }

    // Scale to requested size
    mapImage = mapImage.scaled(pendingWidth, pendingHeight, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    
    // Add marker at center
    int markerX = pendingWidth / 2;
    int markerY = pendingHeight / 2;
    QPixmap finalMap = addMarkerToMap(mapImage, markerX, markerY);

    qDebug() << "Map ready:" << finalMap.size();

    emit staticMapReady(finalMap);
}

void OpenStreetMapService::reverseGeocode(double latitude, double longitude)
{
    if (latitude < -90 || latitude > 90 || longitude < -180 || longitude > 180) {
        emit errorOccurred("Invalid coordinates");
        return;
    }

    pendingLatitude = latitude;
    pendingLongitude = longitude;

    // Nominatim reverse geocoding
    QUrl url("https://nominatim.openstreetmap.org/reverse");
    QUrlQuery query;
    query.addQueryItem("lat", QString::number(latitude, 'f', 6));
    query.addQueryItem("lon", QString::number(longitude, 'f', 6));
    query.addQueryItem("format", "json");
    url.setQuery(query);

    QNetworkRequest request{url};
    request.setRawHeader("User-Agent", "DriveSmart/1.0 (Educational Project)");

    qDebug() << "Reverse geocoding:" << latitude << "," << longitude;

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &OpenStreetMapService::handleReverseGeocodeReply);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError) {
        emit errorOccurred("Reverse geocode error: " + reply->errorString());
        reply->deleteLater();
    });
}

void OpenStreetMapService::handleReverseGeocodeReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Reverse geocoding failed: " + reply->errorString());
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    
    if (!doc.isObject()) {
        emit errorOccurred("Invalid reverse geocode response");
        return;
    }

    QJsonObject result = doc.object();
    QString address = result["display_name"].toString();

    qDebug() << "Reverse geocoded to:" << address;

    emit reverseGeocodeCompleted(pendingLatitude, pendingLongitude, address);
}

QString OpenStreetMapService::getTileUrl(double latitude, double longitude, int zoom)
{
    // Convert lat/lon to tile coordinates
    int tileX, tileY, pixelX, pixelY;
    latLonToTileXY(latitude, longitude, zoom, tileX, tileY, pixelX, pixelY);

    // OpenStreetMap tile server
    // Format: https://tile.openstreetmap.org/{zoom}/{x}/{y}.png
    return QString("https://tile.openstreetmap.org/%1/%2/%3.png")
        .arg(zoom)
        .arg(tileX)
        .arg(tileY);
}

void OpenStreetMapService::latLonToTileXY(double lat, double lon, int zoom, 
                                          int &tileX, int &tileY, 
                                          int &pixelX, int &pixelY)
{
    // Web Mercator projection
    double latRad = qDegreesToRadians(lat);
    double n = qPow(2.0, zoom);
    
    tileX = static_cast<int>((lon + 180.0) / 360.0 * n);
    tileY = static_cast<int>((1.0 - qLn(qTan(latRad) + 1.0 / qCos(latRad)) / M_PI) / 2.0 * n);
    
    // Pixel position within tile (for more accurate marker placement)
    double xFrac = ((lon + 180.0) / 360.0 * n) - tileX;
    double yFrac = ((1.0 - qLn(qTan(latRad) + 1.0 / qCos(latRad)) / M_PI) / 2.0 * n) - tileY;
    
    pixelX = static_cast<int>(xFrac * 256);
    pixelY = static_cast<int>(yFrac * 256);
}

QPixmap OpenStreetMapService::addMarkerToMap(const QPixmap &mapImage, int markerX, int markerY)
{
    QPixmap result = mapImage.copy();
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw red marker pin
    int markerSize = 30;
    int markerRadius = markerSize / 2;

    // Pin shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 80));
    painter.drawEllipse(QPoint(markerX + 2, markerY + markerSize - 5), 
                        markerRadius / 2, markerRadius / 4);

    // Pin body (red circle)
    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(QColor(220, 50, 50));
    painter.drawEllipse(QPoint(markerX, markerY - markerRadius / 2), 
                        markerRadius, markerRadius);

    // Pin tip (triangle pointing down)
    QPolygon triangle;
    triangle << QPoint(markerX - 8, markerY)
             << QPoint(markerX + 8, markerY)
             << QPoint(markerX, markerY + 15);
    painter.drawPolygon(triangle);

    // Center dot
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPoint(markerX, markerY - markerRadius / 2), 5, 5);

    painter.end();

    return result;
}
