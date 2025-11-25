// QR Code generator using web API
// Falls back to simple pattern if API is unavailable

#ifndef QRCODEGEN_H
#define QRCODEGEN_H

#include <QImage>
#include <QString>
#include <QPainter>
#include <QByteArray>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QUrlQuery>
#include <QTimer>
#include <QHash>
#include <vector>
#include <string>

// QR Code generator class
class QRCodeGenerator {
public:
    static QImage generateQRCode(const QString &data, int size = 300) {
        // Try to use web API first for real QR codes
        qDebug() << "Génération du QR code pour:" << data.left(50) << "...";
        QImage qrImage = generateQRCodeFromAPI(data, size);
        
        if (!qrImage.isNull()) {
            qDebug() << "QR Code généré avec succès, taille:" << qrImage.size();
            return qrImage;
        }
        
        // Try alternative API if first one fails
        qDebug() << "Première API a échoué, tentative avec API alternative...";
        qrImage = generateQRCodeFromAlternativeAPI(data, size);
        
        if (!qrImage.isNull()) {
            qDebug() << "QR Code généré avec succès via API alternative";
            return qrImage;
        }
        
        // Fallback to simple pattern if all APIs fail
        qDebug() << "Toutes les APIs ont échoué, utilisation du pattern de secours (non scannable)";
        return generateSimpleQRPattern(data, size);
    }

private:
    static QImage generateQRCodeFromAPI(const QString &data, int size) {
        // Utiliser l'API QR Server avec encodage URL correct
        QNetworkAccessManager manager;
        QEventLoop loop;
        
        // Encoder les données en UTF-8 puis en pourcentage pour l'URL
        QByteArray utf8Bytes = data.toUtf8();
        QByteArray encodedBytes = utf8Bytes.toPercentEncoding();
        QString encodedData = QString::fromLatin1(encodedBytes);
        
        // Construire l'URL de manière simple et directe
        QString urlString = QString("https://api.qrserver.com/v1/create-qr-code/?size=%1x%1&data=%2&format=png&ecc=M&margin=1")
            .arg(size)
            .arg(encodedData);
        
        QUrl url(urlString);
        
        qDebug() << "Génération QR Code - Taille données:" << data.length() << "caractères";
        qDebug() << "URL complète (premiers 500 caractères):" << urlString.left(500);
        
        QNetworkRequest request(url);
        request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
        request.setRawHeader("Accept", "image/png");
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        
        QNetworkReply *reply = manager.get(request);
        
        // Attendre la réponse avec timeout
        QTimer timeout;
        timeout.setSingleShot(true);
        timeout.setInterval(20000); // 20 secondes timeout
        QObject::connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        
        timeout.start();
        loop.exec();
        
        QImage result;
        if (timeout.isActive()) {
            timeout.stop();
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray imageData = reply->readAll();
                qDebug() << "Réponse API reçue, taille:" << imageData.size() << "bytes";
                
                // Vérifier que c'est bien une image PNG (commence par les bytes PNG)
                if (imageData.size() > 100 && imageData.startsWith("\x89PNG")) {
                    QImage image;
                    if (image.loadFromData(imageData, "PNG")) {
                        result = image;
                        qDebug() << "QR Code généré avec succès! Dimensions:" << result.size();
                    } else {
                        qDebug() << "Erreur: Impossible de charger l'image PNG depuis les données";
                    }
                } else {
                    qDebug() << "Erreur: Réponse ne semble pas être une image PNG valide";
                    qDebug() << "Premiers bytes:" << imageData.left(50).toHex();
                }
            } else {
                qDebug() << "Erreur réseau:" << reply->error() << "-" << reply->errorString();
                qDebug() << "Code HTTP:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            }
        } else {
            qDebug() << "Timeout: L'API n'a pas répondu dans les 20 secondes";
        }
        
        delete reply;
        return result;
    }
    
    static QImage generateQRCodeFromAlternativeAPI(const QString &data, int size) {
        // Alternative: Utiliser une autre API ou méthode
        // Essayer avec QuickChart ou une autre API
        QNetworkAccessManager manager;
        QEventLoop loop;
        
        // Encoder les données
        QByteArray utf8Bytes = data.toUtf8();
        QByteArray encodedBytes = utf8Bytes.toPercentEncoding();
        QString encodedData = QString::fromLatin1(encodedBytes);
        
        // Utiliser l'API QR Server avec correction d'erreur élevée
        QString urlString = QString("https://api.qrserver.com/v1/create-qr-code/?size=%1x%1&data=%2&format=png&ecc=H&margin=2")
            .arg(size)
            .arg(encodedData);
        
        QUrl url(urlString);
        QNetworkRequest request(url);
        request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
        request.setRawHeader("Accept", "image/png");
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        
        QNetworkReply *reply = manager.get(request);
        
        QTimer timeout;
        timeout.setSingleShot(true);
        timeout.setInterval(20000);
        QObject::connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        
        timeout.start();
        loop.exec();
        
        QImage result;
        if (timeout.isActive()) {
            timeout.stop();
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray imageData = reply->readAll();
                if (imageData.size() > 100 && imageData.startsWith("\x89PNG")) {
                    QImage image;
                    if (image.loadFromData(imageData, "PNG")) {
                        result = image;
                        qDebug() << "QR Code généré avec succès via API alternative (ECC:H)";
                    }
                }
            }
        }
        
        delete reply;
        return result;
    }
    
    static QImage generateSimpleQRPattern(const QString &data, int size) {
        // Create a simple QR-like pattern as fallback
        QImage image(size, size, QImage::Format_ARGB32);
        image.fill(Qt::white);
        
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, false);
        
        // Draw corner markers (QR code finder patterns)
        int margin = 20;
        int moduleSize = 8;
        
        // Top-left corner (7x7 finder pattern)
        drawFinderPattern(&painter, margin, margin, moduleSize);
        
        // Top-right corner
        drawFinderPattern(&painter, size - margin - moduleSize * 7, margin, moduleSize);
        
        // Bottom-left corner
        drawFinderPattern(&painter, margin, size - margin - moduleSize * 7, moduleSize);
        
        // Draw data pattern based on text hash
        QByteArray dataBytes = data.toUtf8();
        uint hash = qHash(data);
        
        int dataStartX = margin + moduleSize * 9;
        int dataStartY = margin + moduleSize * 9;
        int x = dataStartX;
        int y = dataStartY;
        
        // Generate pseudo-random pattern based on data hash
        for (int i = 0; i < (size - 2 * margin - moduleSize * 18) / moduleSize; ++i) {
            for (int j = 0; j < (size - 2 * margin - moduleSize * 18) / moduleSize; ++j) {
                uint patternHash = hash + i * 31 + j * 17;
                if (patternHash % 3 == 0) { // 33% black modules
                    painter.fillRect(x, y, moduleSize, moduleSize, Qt::black);
                }
                x += moduleSize;
            }
            x = dataStartX;
            y += moduleSize;
        }
        
        painter.end();
        return image;
    }
    
    static void drawFinderPattern(QPainter *painter, int x, int y, int moduleSize) {
        // Draw 7x7 finder pattern
        // Outer square (black)
        painter->fillRect(x, y, moduleSize * 7, moduleSize * 7, Qt::black);
        // Inner square (white)
        painter->fillRect(x + moduleSize, y + moduleSize, moduleSize * 5, moduleSize * 5, Qt::white);
        // Center square (black)
        painter->fillRect(x + moduleSize * 2, y + moduleSize * 2, moduleSize * 3, moduleSize * 3, Qt::black);
    }
};

#endif // QRCODEGEN_H

