#include <QtWidgets>

#include "XbelReader.h"
#include "Global.h"

//! [0]
XbelReader::XbelReader(QTreeWidget* pTreeWidget)
    : m_pTreeWidget(pTreeWidget)
{
    QStyle* pStyle = m_pTreeWidget->style();

    m_folderIcon.addPixmap(pStyle->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    m_folderIcon.addPixmap(pStyle->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    m_bookmarkIcon.addPixmap(pStyle->standardPixmap(QStyle::SP_FileIcon));
}
//! [0]

XbelReader::~XbelReader()
{

}

//! [1]
bool XbelReader::read(QIODevice* pDevice)
{
    m_xmlStreamReader.setDevice(pDevice);

    if (m_xmlStreamReader.readNextStartElement()) {
        if (m_xmlStreamReader.name() == "xbel" && m_xmlStreamReader.attributes().value("version") == "1.0")
            readXBEL();
        else
            m_xmlStreamReader.raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
    }

    return !m_xmlStreamReader.error();
}
//! [1]

//! [2]
QString XbelReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(m_xmlStreamReader.errorString())
            .arg(m_xmlStreamReader.lineNumber())
            .arg(m_xmlStreamReader.columnNumber());
}
//! [2]

//! [3]
void XbelReader::readXBEL()
{
    Q_ASSERT(m_xmlStreamReader.isStartElement() && m_xmlStreamReader.name() == "xbel");

    while (m_xmlStreamReader.readNextStartElement()) {
        if (m_xmlStreamReader.name() == "folder")
            readFolder(0);
        else if (m_xmlStreamReader.name() == "bookmark")
            readBookmark(0);
        else
            m_xmlStreamReader.skipCurrentElement();
    }
}
//! [3]

//! [4]
void XbelReader::readTitle(QTreeWidgetItem* pItem)
{
    Q_ASSERT(m_xmlStreamReader.isStartElement() && m_xmlStreamReader.name() == "title");

    QString title = m_xmlStreamReader.readElementText();
    pItem->setText(0, title);
}
//! [4]


void XbelReader::readFolder(QTreeWidgetItem* pItem)
{
    Q_ASSERT(m_xmlStreamReader.isStartElement() && m_xmlStreamReader.name() == "folder");

    QTreeWidgetItem *pFolder = createChildItem(pItem);
    bool folded = (m_xmlStreamReader.attributes().value("folded") != "no");
    m_pTreeWidget->setItemExpanded(pFolder, !folded);

    while (m_xmlStreamReader.readNextStartElement()) {
        if (m_xmlStreamReader.name() == "title")
            readTitle(pFolder);
        else if (m_xmlStreamReader.name() == "folder")
            readFolder(pFolder);
        else if (m_xmlStreamReader.name() == "bookmark")
            readBookmark(pFolder);
        else
            m_xmlStreamReader.skipCurrentElement();
    }
}

void XbelReader::readBookmark(QTreeWidgetItem* pItem)
{
    Q_ASSERT(m_xmlStreamReader.isStartElement() && m_xmlStreamReader.name() == "bookmark");

    QTreeWidgetItem *pBookmark = createChildItem(pItem);
    pBookmark->setFlags(pBookmark->flags() | Qt::ItemIsEditable);
    pBookmark->setIcon(0, m_bookmarkIcon);
    pBookmark->setText(0, QObject::tr("Unknown title"));
    pBookmark->setText(1, m_xmlStreamReader.attributes().value("href").toString());

    while (m_xmlStreamReader.readNextStartElement())
    {
        if (m_xmlStreamReader.name() == "title")
            readTitle(pBookmark);
        else
            m_xmlStreamReader.skipCurrentElement();
    }
}

QTreeWidgetItem *XbelReader::createChildItem(QTreeWidgetItem* pItem)
{
    QTreeWidgetItem *pChildItem;
    if (pItem) {
        pChildItem = new QTreeWidgetItem(pItem);
    } else {
        pChildItem = new QTreeWidgetItem(m_pTreeWidget);
    }
    pChildItem->setData(0, XBEL_TARGET, m_xmlStreamReader.name().toString());
    return pChildItem;
}
