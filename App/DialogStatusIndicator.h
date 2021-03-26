#ifndef DIALOGSTATUSINDICATOR_H
#define DIALOGSTATUSINDICATOR_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include "QProgressHud.h"

namespace Ui {
class DialogStatusIndicator;
}

class DialogStatusIndicator : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStatusIndicator(QWidget *parent = 0);
    ~DialogStatusIndicator();

    void setCurrentContext(const QString& strContext);

    void setPointNum(int nPointNum = 6);
    void setUpdateTime(int nUpdateTime = 500);
    void setPointColor(const QColor& pointColor = QColor(128, 128, 128));
    void setRadius(int nRadius = 3);
    void setSpace(int nSpace = 2);
    void start();
    void stop();

protected:
    void createWidgets();

private:
    Ui::DialogStatusIndicator *ui;
    QLabel* m_pLabelContext;
    QProgressHUD* m_pHud;
};

#endif // DIALOGSTATUSINDICATOR_H
