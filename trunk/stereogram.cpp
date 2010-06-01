#include <QDebug>
#include "stereogram.h"

QImage generateStereogram(const QImage &depthmap,
                          const QImage &tile,
                          int observerToScreen,
                          int screenToBackground,
                          int betweenEyes)
{
    Q_ASSERT(observerToScreen > 0);
    Q_ASSERT(screenToBackground > 0);
    Q_ASSERT(betweenEyes > 0);
    Q_ASSERT(screenToBackground <= observerToScreen);

    QImage stereoImage(depthmap.size(), QImage::Format_RGB32);
    for (int y = 0; y < depthmap.height(); y++) {
        const QRgb *depthmapLine = reinterpret_cast<const QRgb*>(depthmap.scanLine(y));
        QVector<int> same(depthmap.width());
        for (int x = 0; x < depthmap.width(); x++) {
            same[x] = x;
        }
        for (int x = 0; x < depthmap.width(); x++) {
            // fit depth into a range of [0, screenToBackground]
            int depth = ((255 - qGray(depthmapLine[x])) / 255) * screenToBackground;
            int separation = (betweenEyes * depth) / (depth + observerToScreen);
            int left = x - separation / 2;
            int right = left + separation;
            Q_ASSERT(left <= right);
            if (left < 0 || right > (depthmap.width() - 1))
                continue;
            // The following for loop just makes sure that left and right get the same
            // pixel values without disturbing any existing sameness-links.
            for (int k = same[left];
                 k != left && k != right;
                 k = same[left]) {
                if (k < right) {
                    left = k;
                } else {
                    left = right;
                    right = k;;
                }
            }
            same[left] = right;
        }
        // write the stereogram
        QRgb *stereogramLine = reinterpret_cast<QRgb*>(stereoImage.scanLine(y));
        const QRgb *tileLine = reinterpret_cast<const QRgb*>(tile.scanLine(y % tile.height()));
        for (int x = depthmap.width() - 1; x >= 0; x--) {
            if (same[x] == x) {
                stereogramLine[x] = tileLine[x % tile.width()];
            } else {
                stereogramLine[x] = stereogramLine[same[x]];
            }
        }
    }

    return stereoImage;
}

Stereogram::Stereogram()
    : m_isDirty(false)
    , m_observerToScreen(1000)
    , m_screenToBackground(1000)
    , m_betweenEyes(100)
{
}

void Stereogram::setDepthMap(const QImage &depthmap)
{
    if (m_depthmap != depthmap) {
        m_depthmap = depthmap;
        m_isDirty = true;
    }
}

void Stereogram::setTile(const QImage &tile)
{
    if (m_tile != tile) {
        m_tile = tile;
        m_isDirty = true;
    }
}

void Stereogram::setObserverToScreen(int pixels)
{
    if (m_observerToScreen != pixels) {
        m_observerToScreen = pixels;
        m_isDirty = true;
    }
}

void Stereogram::setScreenToBackground(int pixels)
{
    if (m_screenToBackground != pixels) {
        m_screenToBackground = pixels;
        m_isDirty = true;
    }
}

void Stereogram::setBetweenEyes(int pixels)
{
    if (m_betweenEyes != pixels) {
        m_betweenEyes = pixels;
        m_isDirty = true;
    }
}

QImage Stereogram::stereogram()
{
    if (m_depthmap.isNull() || m_tile.isNull())
        return QImage();
    if (m_isDirty) {
        m_stereogram = generateStereogram(m_depthmap,
                                          m_tile,
                                          m_observerToScreen,
                                          m_screenToBackground,
                                          m_betweenEyes);
    }
    return m_stereogram;
}
