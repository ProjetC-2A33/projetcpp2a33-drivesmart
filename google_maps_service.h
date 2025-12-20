#ifndef GOOGLE_MAPS_SERVICE_H
#define GOOGLE_MAPS_SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QUrl>
#include <QPixmap>

class GoogleMapsService : public QObject
{
    Q_OBJECT

public:
    explicit GoogleMapsService(QObject *parent = nullptr);
    ~GoogleMapsService();

    // Configuration
    void setApiKey(const QString &key);
    
    // Geocoding - Convert address to coordinates
    void geocodeAddress(const QString &address);
    
    // Reverse Geocoding - Convert coordinates to address
    void reverseGeocode(double latitude, double longitude);
    
    // Static Map - Get a static map image
    void getStaticMap(double latitude, double longitude, 
                      int zoom = 15, 
                      int width = 600, 
                      int height = 400,
                      const QString &mapType = "roadmap");
    
    void getStaticMapWithMarker(double latitude, double longitude,
                                const QString &label = "",
                                int zoom = 15,
                                int width = 600,
                                int height = 400);
    
    // Distance Matrix - Calculate distance and duration between points
    void getDistanceMatrix(const QString &origin, const QString &destination);
    
    // Directions - Get route between two points
    void getDirections(const QString &origin, const QString &destination,
                       const QString &mode = "driving");
    
    // Places Search - Search for nearby places
    void searchNearbyPlaces(double latitude, double longitude, 
                           int radius = 1000,
                           const QString &type = "");
    
    // Getters
    QString getApiKey() const { return m_apiKey; }
    
signals:
    // Geocoding signals
    void geocodeCompleted(double latitude, double longitude, const QString &formattedAddress);
    void reverseGeocodeCompleted(const QString &address);
    
    // Map signals
    void staticMapReady(const QPixmap &mapImage);
    
    // Distance signals
    void distanceMatrixReady(int distanceMeters, int durationSeconds, const QString &distanceText, const QString &durationText);
    
    // Directions signals
    void directionsReady(const QJsonObject &route);
    
    // Places signals
    void placesFound(const QJsonArray &places);
    
    // Error signals
    void errorOccurred(const QString &error);
    
private slots:
    void onGeocodeReplyFinished();
    void onReverseGeocodeReplyFinished();
    void onStaticMapReplyFinished();
    void onDistanceMatrixReplyFinished();
    void onDirectionsReplyFinished();
    void onPlacesReplyFinished();
    void onNetworkError(QNetworkReply::NetworkError error);
    
private:
    QNetworkAccessManager *m_networkManager;
    QString m_apiKey;
    
    // Helper methods
    QString buildGeocodeUrl(const QString &address);
    QString buildReverseGeocodeUrl(double latitude, double longitude);
    QString buildStaticMapUrl(double latitude, double longitude, int zoom, int width, int height, const QString &mapType);
    QString buildDistanceMatrixUrl(const QString &origin, const QString &destination);
    QString buildDirectionsUrl(const QString &origin, const QString &destination, const QString &mode);
    QString buildPlacesUrl(double latitude, double longitude, int radius, const QString &type);
    
    void handleError(QNetworkReply *reply, const QString &context);
};

#endif // GOOGLE_MAPS_SERVICE_H
