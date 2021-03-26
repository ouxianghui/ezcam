#include "DrillEditorTableWidget.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QTextCodec>
#include <QHeaderView>
#include "SystemConfigManager.h"
#include "DrillEditorDiameterColumnDelegate.h"

DrillEditorTableWidget::DrillEditorTableWidget(QWidget *parent) : QTableWidget(parent)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    _menu = new QMenu(this);

    _actionAddDrill = new QAction(QIcon(":/icons/add_128.png"), tc->toUnicode("添加"), this);
    connect(_actionAddDrill, &QAction::triggered, this, &DrillEditorTableWidget::onActionAddDrill);

    _actionDelDrill = new QAction(QIcon(":/icons/del_128.png"), tc->toUnicode("删除"), this);
    connect(_actionDelDrill, &QAction::triggered, this, &DrillEditorTableWidget::onActionDelDrill);

    _menu->addAction(_actionAddDrill);
    _menu->addAction(_actionDelDrill);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    QStringList labels;

    labels << tc->toUnicode("刀具名称") << tc->toUnicode("直径");

    this->setColumnCount(labels.size());
    this->setHorizontalHeaderLabels(labels);

    // 直径那一列使用代理
    DrillEditorDiameterColumnDelegate* delegate = new DrillEditorDiameterColumnDelegate(this);
    this->setItemDelegateForColumn(1, delegate);

    init();

    connect(this, &DrillEditorTableWidget::itemChanged, this, &DrillEditorTableWidget::onItemChanged);
}

void DrillEditorTableWidget::init()
{
    int index = this->rowCount();

    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    QMap<QString, QVariant> map;
    smgr.getDrills(map);
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

void DrillEditorTableWidget::save2Settings()
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

    smgr.setDrills(map);
}

void DrillEditorTableWidget::contextMenuEvent(QContextMenuEvent* event)
{
    if (event != NULL)
    {
        QTableWidgetItem* item = this->itemAt(event->pos());
        if (item != NULL && item->isSelected())
        {
            _actionDelDrill->setEnabled(true);
        }
        else
        {
            _actionDelDrill->setDisabled(true);
        }

        _menu->exec(event->globalPos());
    }

    QTableWidget::contextMenuEvent(event);
}

void DrillEditorTableWidget::onActionAddDrill(bool /*checked*/)
{
    disconnect(this, &DrillEditorTableWidget::itemChanged, this, &DrillEditorTableWidget::onItemChanged);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    int index = this->rowCount();

    this->insertRow(index);

    int column = this->columnCount();

    for (int i = 0; i < column; ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(QTableWidgetItem::Type);
        item->setTextAlignment(Qt::AlignCenter);
        this->setItem(index, i, item);

        if (i == 0)
        {
            item->setText(tc->toUnicode("T"));
        }
        else if (i == 1)
        {
            item->setText(tc->toUnicode("0.0"));
        }
    }

    connect(this, &DrillEditorTableWidget::itemChanged, this, &DrillEditorTableWidget::onItemChanged);
}

void DrillEditorTableWidget::onActionDelDrill(bool /*checked*/)
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
    smgr.getDrills(map);

    map.remove(key);

    smgr.setDrills(map);

    this->removeRow(index);
}

void DrillEditorTableWidget::onItemChanged(QTableWidgetItem* /*item*/)
{
    save2Settings();
}

