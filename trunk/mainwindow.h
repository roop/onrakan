#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "settings.h"
#include "stereogram.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadDepthMap(const QString &file);
    void loadDepthMap(const QPixmap &pixmap);
    void loadTile(const QString &file);
    void loadTile(const QPixmap &pixmap);

protected:
    void changeEvent(QEvent *e);

public slots:
    void depthMapDialog();
    void tileDialog();
    void loadRandomDotTile();
    void generateStereogram();
    void optionsDialog();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_arrangementScene, *m_stereogramScene;
    QGraphicsPixmapItem *m_depthmapItem, *m_tileItem, *m_stereogramItem;
    Settings *m_settings;
    Stereogram m_stereogram;
};

#endif // MAINWINDOW_H
