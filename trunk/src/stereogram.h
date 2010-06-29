#ifndef STEREOGRAM_H
#define STEREOGRAM_H

#include <QImage>

class Settings;

class Stereogram
{
public:
    Stereogram();
    void setDepthMap(const QImage &depthmap);
    void setTile(const QImage &tile);
    void setObserverToScreen(int pixels);
    void setScreenToBackground(int pixels);
    void setBetweenEyes(int pixels);
    void setTileOffset(const QPoint &tileOffset);
    QImage stereogram();

private:
    bool m_isDirty;
    QImage m_depthmap;
    QImage m_tile;
    int m_observerToScreen, m_screenToBackground, m_betweenEyes;
    QPoint m_tileOffset;
    QImage m_stereogram;
};

#endif // STEREOGRAM_H
