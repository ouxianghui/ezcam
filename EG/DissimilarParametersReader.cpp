#include "DissimilarParametersReader.h"
#include <QApplication>
#include <QFile>
#include <QStringList>
#include <QException>

DissimilarParametersReader::DissimilarParametersReader()
{
}

DissimilarParametersReader::~DissimilarParametersReader()
{

}

bool DissimilarParametersReader::read(int nID, std::map<QString, QVariant>& paramsMap)
{
    try
    {
        QString strDir = QApplication::applicationDirPath();
        QString strFileName = QString("/dissimilar/%1.xml").arg(nID);
        QString strFilePathName = strDir + strFileName;

        QFile xmlFile(strFilePathName);
        if (!xmlFile.exists())
        {
            return false;
        }

        if (xmlFile.open(QFile::ReadOnly | QFile::Text))
        {
            QXmlStreamReader xmlStreamReader;
            xmlStreamReader.setDevice(&xmlFile);

            if (xmlStreamReader.readNextStartElement())
            {
                QStringRef str = xmlStreamReader.name();
                if (xmlStreamReader.name() == "dissimilar" && xmlStreamReader.attributes().value("version") == "1.0")
                {
                    while (xmlStreamReader.readNextStartElement())
                    {
                        //QStringRef str = xmlStreamReader.name();
                        if (xmlStreamReader.name() == "id")
                        {
                            QString strID = xmlStreamReader.readElementText();
                            if (strID.isEmpty())
                            {
                                return false;
                            }

                            bool ok = false;
                            int nValue = -1;
                            nValue = strID.toInt(&ok);
                            if (nID != nValue)
                            {
                                return false;
                            }
                        }
                        else if (xmlStreamReader.name() == "parameters")
                        {
                            QString strParameters = xmlStreamReader.readElementText();
                            if (strParameters.isEmpty())
                            {
                                return false;
                            }

                            paramsMap.clear();

                            QStringList sl = strParameters.split(';');
                            for (int i = 0; i < sl.size(); ++i)
                            {
                                QString str = sl.at(i);
                                if (!str.isEmpty())
                                {
                                    QStringList slParam = str.split('=');
                                    if (slParam.size() >= 2)
                                    {
                                        QString strKey = slParam.at(0);
                                        QString strValue = slParam.at(1);
                                        bool ok = false;
                                        strValue.toDouble(&ok);
                                        if (ok)
                                        {
                                            paramsMap[strKey] = strValue;
                                        }
                                        else
                                        {
                                            return false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    xmlStreamReader.raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
                    return false;
                }
            }
            else
            {
                return false;
            }

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
        qCritical("DissimilarParametersReader::read() exception: %s", e.what());
        return false;
    }
}

