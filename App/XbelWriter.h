#ifndef XBELWRITER_H
#define XBELWRITER_H

#include <QXmlStreamWriter>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class XbelWriter
{
public:
    XbelWriter(QTreeWidget* pTreeWidget);
    ~XbelWriter();

    bool writeFile(QIODevice* pDevice);

private:
    void writeItem(QTreeWidgetItem* pItem);
    QXmlStreamWriter m_xmlStreamWriter;
    QTreeWidget* m_pTreeWidget;

};

#endif // XBELWRITER_H
