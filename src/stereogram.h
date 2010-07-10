#ifndef STEREOGRAM_H
#define STEREOGRAM_H

#include <QImage>
#include "stereogramparameters.h"

class Stereogram
{
public:
    Stereogram();
    void setDepthMap(const QImage &depthmap);
    void setTile(const QImage &tile);
    void setParameters(const StereogramParameters &parameters);
    void setTileOffset(const QPoint &tileOffset);
    QImage stereogram();

private:
    bool m_isDirty;
    QImage m_depthmap;
    QImage m_tile;
    StereogramParameters m_parameters;
    QPoint m_tileOffset;
    QImage m_stereogram;
};

#endif // STEREOGRAM_H
