#include <QDebug>
#include "stereogram.h"

QImage generateStereogram(const QImage &depthmap,
                          const QImage &_tile,
                          int observerToScreen,
                          int screenToBackground,
                          int betweenEyes,
                          const QPoint &_tileOffset)
{
    Q_ASSERT(observerToScreen > 0);
    Q_ASSERT(screenToBackground > 0);
    Q_ASSERT(betweenEyes > 0);
    Q_ASSERT(screenToBackground <= observerToScreen);

    int separationMax = (betweenEyes * screenToBackground) / (observerToScreen + screenToBackground);
    QImage tile = _tile;
    if (tile.width() != separationMax)
        tile = _tile.scaledToWidth(separationMax);
    int separationMin = 0.5 * separationMax; // we wont be comfortable squinting any further
    int depthMin = (observerToScreen * separationMin) / (betweenEyes - separationMin);
    QImage stereoImage(depthmap.size(), QImage::Format_RGB32);
    for (int y = 0; y < depthmap.height(); y++) {
        const QRgb *depthmapLine = reinterpret_cast<const QRgb*>(depthmap.scanLine(y));
        QVector<int> same(depthmap.width());
        for (int x = 0; x < depthmap.width(); x++) {
            same[x] = x;
        }
        for (int x = 0; x < depthmap.width(); x++) {
            // fit depth into a range of [depthMin, screenToBackground]
            double depthZeroToOne = ((255 - qGray(depthmapLine[x])) / 255.0);
            int depth = depthMin + (depthZeroToOne * (screenToBackground - depthMin));
            int separation = (betweenEyes * depth) / (observerToScreen + depth);
            Q_ASSERT(separation >= separationMin);
            Q_ASSERT(separation <= separationMax);
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
        QPoint tileOffset = _tileOffset;
        tileOffset.setY(-tileOffset.y());
        while (tileOffset.y() < 0)
            tileOffset.setY(tileOffset.y() + tile.height());
        while (tileOffset.x() < 0)
            tileOffset.setX(tileOffset.x() + tile.width());
        QRgb *stereogramLine = reinterpret_cast<QRgb*>(stereoImage.scanLine(y));
        const QRgb *tileLine = reinterpret_cast<const QRgb*>(tile.scanLine((y + tileOffset.y()) % tile.height()));
        for (int x = depthmap.width() - 1; x >= 0; x--) {
            if (same[x] == x) {
                stereogramLine[x] = tileLine[(x + tileOffset.x()) % tile.width()];
            } else {
                stereogramLine[x] = stereogramLine[same[x]];
            }
        }
    }

    return stereoImage;
}

Stereogram::Stereogram()
    : m_isDirty(false)
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

void Stereogram::setParameters(const StereogramParameters &parameters)
{
    if (m_parameters != parameters) {
        m_parameters = parameters;
        m_isDirty = true;
    }
}

void Stereogram::setTileOffset(const QPoint &tileOffset)
{
    if (m_tileOffset != tileOffset) {
        m_tileOffset = tileOffset;
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
                                          m_parameters.observerToScreenPx(),
                                          m_parameters.screenToBackgroundPx(),
                                          m_parameters.betweenEyesPx(),
                                          m_tileOffset);
    }
    return m_stereogram;
}
