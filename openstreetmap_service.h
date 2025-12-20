#ifndef OPENSTREETMAPSERVICE_H
#define OPENSTREETMAPSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QPixmap>

/**
 * @class OpenStreetMapService
 * @brief Service class for OpenStreetMap and Nominatim geocoding
 * 
 * 100% FREE - No API key required!
 * Uses:
 * - Nominatim API for geocoding (address to coordinates)
 * - OpenStreetMap tiles for map display
 * 
 * Usage Policy:
 * - Maximum 1 request per second
 * - Must provide User-Agent header
 * - Fair use for non-commercial educational projects
 */
class OpenStreetMapService : public QObject
{
    Q_OBJECT

public:
    explicit OpenStreetMapService(QObject *parent = nullptr);
    ~OpenStreetMapService();

    /**
     * @brief Geocode an address to get latitude/longitude coordinates
     * @param address The address to geocode (e.g., "Avenue Habib Bourguiba, Tunis")
     * 
     * Uses Nominatim API: https://nominatim.openstreetmap.org/
     * Results emitted via geocodeCompleted signal
     */
    void geocodeAddress(const QString &address);

    /**
     * @brief Get a static map image centered on coordinates with a marker
     * @param latitude Latitude of the center point
     * @param longitude Longitude of the center point
     * @param zoom Zoom level (1-19, default 15)
     * @param width Map width in pixels (default 800)
     * @param height Map height in pixels (default 600)
     * 
     * Downloads OpenStreetMap tile and adds a marker
     * Results emitted via staticMapReady signal
     */
    void getStaticMapWithMarker(double latitude, double longitude,
                                int zoom = 15,
                                int width = 800,
                                int height = 600);

    /**
     * @brief Reverse geocode coordinates to get an address
     * @param latitude Latitude
     * @param longitude Longitude
     * 
     * Results emitted via reverseGeocodeCompleted signal
     */
    void reverseGeocode(double latitude, double longitude);

signals:
    /**
     * @brief Emitted when geocoding is complete
     * @param address Original address queried
     * @param latitude Resulting latitude
     * @param longitude Resulting longitude
     * @param displayName Full formatted address from Nominatim
     */
    void geocodeCompleted(const QString &address, double latitude, double longitude, const QString &displayName);

    /**
     * @brief Emitted when static map image is ready
     * @param mapImage QPixmap containing the map with marker
     */
    void staticMapReady(const QPixmap &mapImage);

    /**
     * @brief Emitted when reverse geocoding is complete
     * @param latitude Original latitude
     * @param longitude Original longitude
     * @param address Resulting address
     */
    void reverseGeocodeCompleted(double latitude, double longitude, const QString &address);

    /**
     * @brief Emitted when any error occurs
     * @param errorMessage Description of the error
     */
    void errorOccurred(const QString &errorMessage);

private slots:
    void handleGeocodeReply();
    void handleReverseGeocodeReply();
    void handleMapTileReply();

private:
    QNetworkAccessManager *networkManager;
    
    // Store pending request context
    QString pendingAddress;
    double pendingLatitude;
    double pendingLongitude;
    int pendingZoom;
    int pendingWidth;
    int pendingHeight;

    /**
     * @brief Get OpenStreetMap tile URL for given coordinates and zoom
     * @return URL to tile image
     */
    QString getTileUrl(double latitude, double longitude, int zoom);

    /**
     * @brief Add a red marker to the map image
     * @param mapImage Original map image
     * @param markerX X position of marker
     * @param markerY Y position of marker
     * @return Map image with marker added
     */
    QPixmap addMarkerToMap(const QPixmap &mapImage, int markerX, int markerY);

    /**
     * @brief Convert lat/lon to tile coordinates
     */
    void latLonToTileXY(double lat, double lon, int zoom, int &tileX, int &tileY, int &pixelX, int &pixelY);
};

#endif // OPENSTREETMAPSERVICE_H
