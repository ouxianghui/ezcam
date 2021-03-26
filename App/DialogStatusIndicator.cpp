#include "DialogStatusIndicator.h"
#include "ui_DialogStatusIndicator.h"
#include <QTextCodec>


DialogStatusIndicator::DialogStatusIndicator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStatusIndicator)
{
    ui->setupUi(this);

    QTextCodec *tc = QTextCodec::codecForName("GBK");
    this->setWindowTitle(tc->toUnicode("任务状态指示器"));
    createWidgets();
}

DialogStatusIndicator::~DialogStatusIndicator()
{
    delete ui;
}

void DialogStatusIndicator::createWidgets()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    this->setLayout(pLayout);

    m_pLabelContext = new QLabel(this);
    pLayout->addWidget(m_pLabelContext, 1);

    m_pHud = new QProgressHUD(100, 4, 2, QColor(0, 0, 0), this);
    m_pHud->setVisible(true);
    pLayout->addWidget(m_pHud, 1);
}

void DialogStatusIndicator::setCurrentContext(const QString& strContext)
{
    m_pLabelContext->setText(strContext);
}

void DialogStatusIndicator::setPointNum(int nPointNum)
{
    m_pHud->setPointNum(nPointNum);
}

void DialogStatusIndicator::setUpdateTime(int nUpdateTime)
{
    m_pHud->setUpdateTime(nUpdateTime);
}

void DialogStatusIndicator::setPointColor(const QColor& pointColor)
{
    m_pHud->setPointColor(pointColor);
}

void DialogStatusIndicator::setRadius(int nRadius)
{
    m_pHud->setRadius(nRadius);
}

void DialogStatusIndicator::setSpace(int nSpace)
{
    m_pHud->setSpace(nSpace);
}

void DialogStatusIndicator::start()
{
    m_pHud->start();
}

void DialogStatusIndicator::stop()
{
    m_pHud->stop();
}
