// QR Code generator using web API
// Falls back to simple pattern if API is unavailable

#ifndef QRCODEGEN_H
#define QRCODEGEN_H

#include <QImage>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QPainter>
#include <vector>
#include <string>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include "vendor/qrcodegen.hpp"

// QR Code generator class
class QRCodeGenerator {
public:
    static QImage generateQRCode(const QString &data, int size = 600) {
        std::string text = data.toUtf8().toStdString();
        qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text.c_str(), qrcodegen::QrCode::Ecc::HIGH);
        int modules = qr.getSize();
        int marginModules = 8;
        int ppm = std::max(6, std::min(16, size / (modules + 2 * marginModules)));
        int marginPx = marginModules * ppm;
        int imgSize = modules * ppm + 2 * marginPx;
        QImage image(imgSize, imgSize, QImage::Format_ARGB32);
        image.fill(Qt::white);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        for (int y = 0; y < modules; y++) {
            for (int x = 0; x < modules; x++) {
                if (qr.getModule(x, y)) {
                    painter.fillRect(marginPx + x * ppm, marginPx + y * ppm, ppm, ppm, Qt::black);
                }
            }
        }
        painter.end();
        return image;
    }
};

#endif // QRCODEGEN_H

