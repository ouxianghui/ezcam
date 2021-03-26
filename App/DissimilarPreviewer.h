#ifndef DISSIMILARPREVIEWER_H
#define DISSIMILARPREVIEWER_H

#include <QFrame>
#include <QPaintEvent>
#include <map>
#include <QPixmap>

namespace Ui {
class DissimilarPreviewer;
}

class DissimilarPreviewer : public QFrame
{
    Q_OBJECT

public:
    explicit DissimilarPreviewer(QWidget* parent = 0);
    ~DissimilarPreviewer();

    void drawDissimilar(int nID, const std::map<QString, QVariant>& paramsMap);

protected:
    void paintEvent(QPaintEvent* event);

private:
    Ui::DissimilarPreviewer* ui;

    int m_nID;
    std::map<QString, QVariant> m_paramsMap;
    QImage m_bg;
};

#endif // DISSIMILARPREVIEWER_H
