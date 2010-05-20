#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsdialog.h"

#include <QDebug>
#include <QImage>
#include <QSize>
#include <QDir>
#include <QFileDialog>
#include <QApplication>

static QImage stereogram(const QImage &depthMap, const QImage &tile, int tileoffset = 0, int *xDelta = 0)
{
    int tileWidth = qMin(depthMap.width(), tile.width());
    int tileHeight = tile.height();
    tileoffset = tileoffset % tileWidth;
    if (tileoffset >= 0)
        tileoffset = tileoffset - tileWidth;
    Q_ASSERT(tileoffset < 0);
    int depthMapOffset = -tileoffset;
    QSize stereoImageSize(depthMap.width() + depthMapOffset, depthMap.height());
    QImage stereoImage(stereoImageSize, QImage::Format_RGB32);

    // fill the first tile
    int x = 0;
    for ( ; x < tileWidth; x++) {
        for (int y = 0; y < stereoImage.height(); y++) {
            QRgb pix = tile.pixel(x, y % tileHeight);
            stereoImage.setPixel(x, y, pix);
        }
    }

    // fill other tiles using the previous tile
    for ( ; x < stereoImage.width(); x++) {
        for (int y = 0; y < stereoImage.height(); y++) {
            int depthPixel = depthMap.pixel(x - depthMapOffset, y);
            int depthGray = (qRed(depthPixel) + qGreen(depthPixel) + qBlue(depthPixel)) / 3;
            int depth = depthGray % (tileWidth / 4);
            QRgb pix = stereoImage.pixel(x - tileWidth + depth, y);
            stereoImage.setPixel(x, y, pix);
        }
    }
    if (xDelta)
        *xDelta = depthMapOffset;

    return stereoImage;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_arrangementScene(new QGraphicsScene)
    , m_stereogramScene(new QGraphicsScene)
    , m_depthmapItem(new QGraphicsPixmapItem)
    , m_tileItem(new QGraphicsPixmapItem)
    , m_stereogramItem(new QGraphicsPixmapItem)
    , m_settings(0)
{
    ui->setupUi(this);
    m_settings = new Settings(this);
    m_arrangementScene->addItem(m_depthmapItem);
    m_arrangementScene->addItem(m_tileItem);
    m_tileItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_stereogramScene->addItem(m_stereogramItem);
    ui->arrangementGraphicsView->setScene(m_arrangementScene);
    ui->stereogramGraphicsView->setScene(m_stereogramScene);
    // loadDepthMap(":/images/default_depth_map.png");
    // loadRandomDotTile();
    QString depthMapFile = "/home/roop/tmp/boxes.png";
    QString tileFile = "/home/roop/tile.png";
    if (QApplication::arguments().size() > 1)
        depthMapFile = QApplication::arguments().at(1);
    if (QApplication::arguments().size() > 2)
        tileFile = QApplication::arguments().at(2);
    loadDepthMap(depthMapFile);
    loadTile(tileFile);

    connect(ui->action_LoadDepthMap, SIGNAL(triggered()), SLOT(depthMapDialog()));
    connect(ui->action_LoadTileFromFile, SIGNAL(triggered()), SLOT(tileDialog()));
    connect(ui->action_LoadRandomDotTile, SIGNAL(triggered()), SLOT(loadRandomDotTile()));
    connect(ui->action_Options, SIGNAL(triggered()), SLOT(optionsDialog()));

    connect(ui->generateStereogramButton, SIGNAL(clicked()), SLOT(generateStereogram()));
}

void MainWindow::depthMapDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                          tr("Choose depth map image"),
                          QDir::currentPath(),
                          tr("Image Files (*.png *.jpg *.jpeg *.bmp *.PNG *.JPG *.JPEG *.BMP)"));
    if (!fileName.isEmpty())
        loadDepthMap(fileName);
}

void MainWindow::tileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                          tr("Choose tile image"),
                          QDir::currentPath(),
                          tr("Image Files (*.png *.jpg *.jpeg *.bmp *.PNG *.JPG *.JPEG *.BMP)"));
    if (!fileName.isEmpty())
        loadTile(fileName);
}

void MainWindow::loadDepthMap(const QString &file)
{
    QPixmap pixmap(file);
    if (pixmap.isNull())
        qDebug() << "Could not load depth map from file:" << file;
    loadDepthMap(pixmap);
}

void MainWindow::loadDepthMap(const QPixmap &pixmap)
{
    m_depthmapItem->setPixmap(pixmap);
    m_arrangementScene->setSceneRect(pixmap.rect());
    m_stereogramScene->setSceneRect(pixmap.rect());
}

void MainWindow::loadTile(const QString &file)
{
    QPixmap pixmap(file);
    if (pixmap.isNull())
        qDebug() << "Could not load tile from file:" << file;
    loadTile(pixmap);
}

void MainWindow::loadTile(const QPixmap &pixmap)
{
    m_tileItem->setPixmap(pixmap);
    m_tileItem->setPos(m_depthmapItem->pos());
}

void MainWindow::loadRandomDotTile()
{
    QRectF depthMapBr = m_depthmapItem->boundingRect();
    int height = depthMapBr.height();
    int width = qMin(qMax(0xff, height / 3), int(depthMapBr.width()) / 5);
    // 0xff is the gradient range in the depth map
    QImage img(QSize(width, height), QImage::Format_RGB32);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            img.setPixel(x, y, rand() % 0xffffff);
        }
    }
    loadTile(QPixmap::fromImage(img));
}

void MainWindow::generateStereogram()
{
    QImage depthMap = m_depthmapItem->pixmap().toImage();
    QImage tile = m_tileItem->pixmap().toImage();
    int tileoffset = int(m_tileItem->pos().x() - m_depthmapItem->pos().x());
    int xDelta = 0;
    m_stereogramItem->setPixmap(QPixmap::fromImage(stereogram(depthMap, tile, tileoffset, &xDelta)));
    m_stereogramItem->setPos(-xDelta, 0);
}

void MainWindow::optionsDialog()
{
    OptionsDialog dialog(m_settings, this);
    bool ok = dialog.exec();
    if (ok)
        qDebug() << "Gotta redo the stereogram";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_arrangementScene;
    delete m_stereogramScene;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
