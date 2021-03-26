#include <QtWidgets>

#include "XbelWriter.h"
#include "Global.h"

//! [0]
XbelWriter::XbelWriter(QTreeWidget* pTreeWidget)
    : m_pTreeWidget(pTreeWidget)
{
    m_xmlStreamWriter.setAutoFormatting(true);
}
//! [0]

XbelWriter::~XbelWriter()
{

}

//! [1]
bool XbelWriter::writeFile(QIODevice* pDevice)
{
    m_xmlStreamWriter.setDevice(pDevice);

    m_xmlStreamWriter.writeStartDocument();
    m_xmlStreamWriter.writeDTD("<!DOCTYPE xbel>");
    m_xmlStreamWriter.writeStartElement("xbel");
    m_xmlStreamWriter.writeAttribute("version", "1.0");
    for (int i = 0; i < m_pTreeWidget->topLevelItemCount(); ++i)
        writeItem(m_pTreeWidget->topLevelItem(i));

    m_xmlStreamWriter.writeEndDocument();
    return true;
}
//! [1]

//! [2]
void XbelWriter::writeItem(QTreeWidgetItem* pItem)
{
    QString tagName = pItem->data(0, XBEL_TARGET).toString();
    if (tagName == "folder")
    {
        bool folded = !m_pTreeWidget->isItemExpanded(pItem);
        m_xmlStreamWriter.writeStartElement(tagName);
        m_xmlStreamWriter.writeAttribute("folded", folded ? "yes" : "no");
        m_xmlStreamWriter.writeTextElement("title", pItem->text(0));
        for (int i = 0; i < pItem->childCount(); ++i)
            writeItem(pItem->child(i));
        m_xmlStreamWriter.writeEndElement();
    }
    else if (tagName == "bookmark")
    {
        m_xmlStreamWriter.writeStartElement(tagName);
        if (!pItem->text(1).isEmpty())
            m_xmlStreamWriter.writeAttribute("href", pItem->text(1));
        m_xmlStreamWriter.writeTextElement("title", pItem->text(0));
        m_xmlStreamWriter.writeEndElement();
    }
}
//! [2]

