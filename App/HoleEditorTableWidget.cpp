#include "HoleEditorTableWidget.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QTextCodec>
#include <QHeaderView>
#include "SystemConfigManager.h"

HoleEditorTableWidget::HoleEditorTableWidget(QWidget *parent) : QTableWidget(parent)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    _menu = new QMenu(this);

    _actionAddHole = new QAction(QIcon(":/icons/add_128.png"), tc->toUnicode("添加"), this);
    connect(_actionAddHole, &QAction::triggered, this, &HoleEditorTableWidget::onActionAddHole);

    _actionDelHole = new QAction(QIcon(":/icons/del_128.png"), tc->toUnicode("删除"), this);
    connect(_actionDelHole, &QAction::triggered, this, &HoleEditorTableWidget::onActionDelHole);

    _menu->addAction(_actionAddHole);
    _menu->addAction(_actionDelHole);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    QStringList labels;

    labels << tc->toUnicode("直径")
           << tc->toUnicode("深度");

    this->setColumnCount(labels.size());
    this->setHorizontalHeaderLabels(labels);

    init();

    connect(this, &HoleEditorTableWidget::itemChanged, this, &HoleEditorTableWidget::onItemChanged);
}

void HoleEditorTableWidget::init()
{
    int index = this->rowCount();

    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    QMap<QString, QVariant> map;
    smgr.getHoles(map);
    for (QMap<QString, QVariant>::iterator it = map.begin(); it != map.end(); ++it)
    {
        this->insertRow(index);

        int column = this->columnCount();

        for (int i = 0; i < column; ++i)
        {
            QTableWidgetItem* item = new QTableWidgetItem(QTableWidgetItem::Type);
            item->setTextAlignment(Qt::AlignCenter);
            this->setItem(index, i, item);

            if (i == 0)
            {
                item->setText(it.key());
            }
            else if (i == 1)
            {
                item->setText(it.value().toString());
            }
        }

        ++index;
    }
}

void HoleEditorTableWidget::save2Settings()
{
    int rowNum = this->rowCount();

    int columnNum = this->columnCount();

    QMap<QString, QVariant> map;

    for (int j = 0; j < rowNum; ++j)
    {
        QString key;
        QVariant value;
        for (int i = 0; i < columnNum; ++i)
        {
            QTableWidgetItem* item = this->item(j, i);

            if (i == 0)
            {
                key = item->text();
            }
            else if (i == 1)
            {
                value = item->text();
            }
        }

        map[key] = value;
    }

    SystemConfigManager& smgr = SystemConfigManager::GetInstance();

    smgr.setHoles(map);
}

void HoleEditorTableWidget::contextMenuEvent(QContextMenuEvent* event)
{
    if (event != NULL)
    {
        QTableWidgetItem* item = this->itemAt(event->pos());
        if (item != NULL && item->isSelected())
        {
            _actionDelHole->setEnabled(true);
        }
        else
        {
            _actionDelHole->setDisabled(true);
        }

        _menu->exec(event->globalPos());
    }

    QTableWidget::contextMenuEvent(event);
}

void HoleEditorTableWidget::onActionAddHole(bool /*checked*/)
{
    disconnect(this, &HoleEditorTableWidget::itemChanged, this, &HoleEditorTableWidget::onItemChanged);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    int index = this->rowCount();

    this->insertRow(index);

    int column = this->columnCount();

    for (int i = 0; i < column; ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(QTableWidgetItem::Type);
        item->setTextAlignment(Qt::AlignCenter);
        this->setItem(index, i, item);

        item->setText(tc->toUnicode("0.0"));
    }

    connect(this, &HoleEditorTableWidget::itemChanged, this, &HoleEditorTableWidget::onItemChanged);
}

void HoleEditorTableWidget::onActionDelHole(bool /*checked*/)
{
    int index = this->currentRow();

    int column = this->columnCount();

    QString key;
    for (int i = 0; i < column; ++i)
    {
        QTableWidgetItem* item = this->item(index, i);

        if (i == 0)
        {
            key = item->text();
            break;
        }
    }

    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    QMap<QString, QVariant> map;
    smgr.getHoles(map);

    map.remove(key);

    smgr.setHoles(map);

    this->removeRow(index);
}

void HoleEditorTableWidget::onItemChanged(QTableWidgetItem* /*item*/)
{
    save2Settings();
}

