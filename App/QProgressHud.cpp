#include "QProgressHud.h"
#include "ui_qprogresshud.h"
#include <QPainter>

QProgressHUD::QProgressHUD(int nUpdateTime, int nRadius, int nSpace, const QColor& pointColor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QProgressHUD),
    m_nUpdateTime(nUpdateTime),
    m_nRadius(nRadius),
    m_nSpace(nSpace),
    m_pointColor(pointColor)
{
    ui->setupUi(this);

    m_nPointNum = 0;
    m_listPoint.clear();

    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(nUpdateTime);

    connect(m_pTimer, &QTimer::timeout, this, &QProgressHUD::onTimerTimeout);
    m_pTimer->start();

    QRect rc = this->rect();
    m_nPointNum = rc.width()/((m_nRadius+m_nSpace)*2);

    int nAvg = 255/m_nPointNum;
    for (int i = m_nPointNum-1; i >= 0; --i)
    {
        m_listPoint.push_front(i*nAvg);
    }
}

QProgressHUD::~QProgressHUD()
{
    delete ui;
}

void QProgressHUD::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect rc = this->rect();

    //用户希望的矩形的个数
    int nNumUser = m_listPoint.size();

    //控件的尺寸至少能容纳下一个矩形才画点
    if (nNumUser > 0 && rc.height() >= (m_nRadius+m_nSpace)*2 && rc.width() >= (m_nRadius+m_nSpace)*2)
    {
        //圆心为控件高度的一半的位置
        qreal dCenterY = rc.height()/2;

        //当前控件的尺寸能画几个矩形
        int nNumWidget = rc.width()/((m_nRadius+m_nSpace)*2);

        //判断一下，实际上能画几个矩形
        int nNum = nNumWidget > nNumUser ? nNumUser : nNumWidget;

        //开始画圆
        for (int i = 0; i < nNum; ++i)
        {
            m_pointColor.setAlpha(m_listPoint.at(i));
            QBrush brush(m_pointColor);
            painter.setBrush(brush);
            painter.drawRect((m_nRadius+m_nSpace)*2*i, dCenterY-m_nRadius, m_nRadius*2, m_nRadius*2);
        }
    }


    QWidget::paintEvent(event);
}

void QProgressHUD::setPointNum(int nPointNum)
{
    m_nPointNum = nPointNum;
}

void QProgressHUD::setUpdateTime(int nUpdateTime)
{
    m_nUpdateTime = nUpdateTime;
}

void QProgressHUD::setPointColor(const QColor& pointColor)
{
    m_pointColor = pointColor;
}

void QProgressHUD::setRadius(int nRadius)
{
    m_nRadius = nRadius;
}

void QProgressHUD::setSpace(int nSpace)
{
    m_nSpace = nSpace;
}

void QProgressHUD::start()
{
    if (!m_pTimer->isActive())
    {
        m_pTimer->start();
    }
}

void QProgressHUD::stop()
{
    if (m_pTimer->isActive())
    {
        m_pTimer->stop();
    }
}

void QProgressHUD::onTimerTimeout()
{
//    int nNum = m_listPoint.size();
    int nValue = m_listPoint.takeLast();
    m_listPoint.push_front(nValue);

    update();
}
