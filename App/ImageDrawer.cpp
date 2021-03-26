#include "ImageDrawer.h"
#include "ui_ImageDrawer.h"
#include <QPainter>

ImageDrawer::ImageDrawer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ImageDrawer)
{
    ui->setupUi(this);
}

ImageDrawer::~ImageDrawer()
{
    delete ui;
}

void ImageDrawer::drawImage(const QImage& image)
{
    m_image = image;
    update();
}

void ImageDrawer::paintEvent(QPaintEvent* event)
{
    if (!m_image.isNull())
    {
        QPainter painter(this);

        // 抗锯齿
        painter.setRenderHint(QPainter::Antialiasing, true);

        QRect rc = painter.viewport();

        double dImageW = m_image.width();
        double dImageH = m_image.height();

        double dDrawerW = rc.width();
        double dDrawerH = rc.height();

        double dImageWHScale = dImageW/dImageH;
        double dDrawerWHScale = dDrawerW/dDrawerH;

        double dRealW = 0.0f;
        double dRealH = 0.0f;

        double dRealX = 0.0f;
        double dRealY = 0.0f;

        // 以画布的高为基准
        if (dImageWHScale < dDrawerWHScale)
        {
            dRealH = dDrawerH;
            dRealW = dRealH*dImageWHScale;

            dRealX = (dDrawerW-dRealW)/2.f;
            dRealY = 0.0f;
        }
        // 以画布的宽为基准
        else
        {
            dRealW = dDrawerW;
            dRealH = dRealW/dImageWHScale;

            dRealX = 0.0f;
            dRealY = (dDrawerH-dRealH)/2.0f;
        }

        // 往中心收缩4个像素，空出边缘绘图
        dRealX += 4.0;
        dRealY += 4.0;
        dRealW -= 8.0;
        dRealH -= 8.0;

        //m_image.scaled(QSize(dRealW, dRealH));
        painter.drawImage(QRectF(dRealX, dRealY, dRealW, dRealH), m_image);
    }

    QFrame::paintEvent(event);
}
