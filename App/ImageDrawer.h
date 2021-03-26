#ifndef IMAGEDRAWER_H
#define IMAGEDRAWER_H

#include <QFrame>
#include <QImage>

namespace Ui {
class ImageDrawer;
}

class ImageDrawer : public QFrame
{
    Q_OBJECT

public:
    explicit ImageDrawer(QWidget *parent = 0);
    ~ImageDrawer();

    void drawImage(const QImage& image);

protected:
    void paintEvent(QPaintEvent* event);

private:
    Ui::ImageDrawer *ui;
    QImage m_image;
};

#endif // IMAGEDRAWER_H
