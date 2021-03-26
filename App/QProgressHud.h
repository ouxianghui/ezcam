#ifndef QPROGRESSHUD_H
#define QPROGRESSHUD_H

#include <QWidget>
#include <QList>
#include <QColor>
#include <QTimer>

namespace Ui {
class QProgressHUD;
}

class QProgressHUD : public QWidget
{
    Q_OBJECT

public:
    QProgressHUD(int nUpdateTime = 500, int nRadius = 3, int nSpace = 2, const QColor& pointColor = QColor(0, 0, 0, 127), QWidget *parent = 0);
    ~QProgressHUD();

    void setPointNum(int nPointNum = 6);
    void setUpdateTime(int nUpdateTime = 500);
    void setPointColor(const QColor& pointColor = QColor(128, 128, 128));
    void setRadius(int nRadius = 3);
    void setSpace(int nSpace = 2);
    void start();
    void stop();

protected slots:
    void onTimerTimeout();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::QProgressHUD *ui;
    QList<int> m_listPoint;
    int m_nPointNum;
    int m_nUpdateTime;
    int m_nRadius;
    int m_nSpace;
    QColor m_pointColor;
    QTimer* m_pTimer;
};

#endif // QPROGRESSHUD_H
