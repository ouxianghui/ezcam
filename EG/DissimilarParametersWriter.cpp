#include "DissimilarParametersWriter.h"
#include <QFile>
#include <QApplication>
#include <QException>

DissimilarParametersWriter::DissimilarParametersWriter()
{
}

DissimilarParametersWriter::~DissimilarParametersWriter()
{

}

bool DissimilarParametersWriter::write(int nID, const std::map<QString, QVariant>& paramsMap)
{
    try
    {
        QString strDir = QApplication::applicationDirPath();
        QString strFileName = QString("/dissimilar/%1.xml").arg(nID);
        QString strFilePathName = strDir + strFileName;

        QFile xmlFile(strFilePathName);
        if (xmlFile.exists())
        {
            xmlFile.remove();
        }

        if (xmlFile.open(QFile::ReadWrite | QFile::Text))
        {
            QXmlStreamWriter xmlStreamWriter;
            xmlStreamWriter.setAutoFormatting(true);
            xmlStreamWriter.setDevice(&xmlFile);
            xmlStreamWriter.writeStartDocument();
            xmlStreamWriter.writeDTD("<!DOCTYPE dissimilar>");
            xmlStreamWriter.writeStartElement("dissimilar");
            xmlStreamWriter.writeAttribute("version", "1.0");

            xmlStreamWriter.writeStartElement("element");
            xmlStreamWriter.writeTextElement("id", QString("%1").arg(nID));
            QString strParameters;
            std::map<QString, QVariant>::const_iterator it = paramsMap.begin();
            for (; it != paramsMap.end(); ++it)
            {
                QString strKey = it->first;
                QVariant var = it->second;
                QString strValue = var.toString();
                QString str;
                if (strParameters.isEmpty())
                {
                    str = QString("%1=%2").arg(strKey).arg(strValue);
                }
                else
                {
                    str = QString(";%1=%2").arg(strKey).arg(strValue);
                }
                strParameters.append(str);
            }

            xmlStreamWriter.writeTextElement("parameters", strParameters);
            xmlStreamWriter.writeEndElement();

            xmlStreamWriter.writeEndDocument();

            xmlFile.close();
            return true;
        }
        else
        {
            qWarning("dissimilar-library:%s",
                     QString("cannot read file %1:%2.")
                     .arg(strFilePathName)
                     .arg(xmlFile.errorString()).toStdString().c_str());
            return false;
        }
    }
    catch (QException& e)
    {
        qCritical("DissimilarParametersWriter::write() exception: %s", e.what());
        return false;
    }
}
