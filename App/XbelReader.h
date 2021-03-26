#ifndef XBELREADER_H
#define XBELREADER_H

#include <QIcon>
#include <QXmlStreamReader>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class XbelReader
{
public:
    XbelReader(QTreeWidget* pTreeWidget);
    ~XbelReader();

    bool read(QIODevice* pDevice);

    QString errorString() const;

private:
//! [2]
    void readXBEL();
    void readTitle(QTreeWidgetItem* pItem);
    void readFolder(QTreeWidgetItem* pItem);
    void readBookmark(QTreeWidgetItem* pItem);

    QTreeWidgetItem *createChildItem(QTreeWidgetItem* pItem);

    QXmlStreamReader m_xmlStreamReader;
    QTreeWidget* m_pTreeWidget;
//! [2]

    QIcon m_folderIcon;
    QIcon m_bookmarkIcon;
};

#endif // XBELREADER_H
