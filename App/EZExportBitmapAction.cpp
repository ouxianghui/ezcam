#include "EZExportBitmapAction.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include <QMessageBox>
#include <QImageWriter>
#include <QFileDialog>
#include <QTextCodec>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "RGraphicsView.h"
#include "RGraphicsSceneQt.h"
#include "RGraphicsViewImage.h"
#include "SystemConfigManager.h"
#include "BitmapExportDialog.h"

EZExportBitmapAction::EZExportBitmapAction(RGuiAction* pGuiAction)
    : RActionAdapter(pGuiAction)
{

}

EZExportBitmapAction::~EZExportBitmapAction()
{

}

void EZExportBitmapAction::beginEvent()
{
    BitmapExportDialog dlg;

    if (dlg.exec() == QDialog::Accepted)
    {
        QString bmpFileName = this->getFileName();

        if (bmpFileName.isNull() || bmpFileName.isEmpty())
        {
            this->terminate();
            return;
        }

        // make sure nothing is selected:
        documentInterface->deselectAll();
        RGraphicsView* view = documentInterface->getLastKnownViewWithFocus();
        RGraphicsScene* scene = view->getScene();

        RGraphicsViewImage* pGraphicsView = new RGraphicsViewImage();
        pGraphicsView->setScene((RGraphicsSceneQt*)scene, false);

        pGraphicsView->setPaintOrigin(true);
        pGraphicsView->setTextHeightThresholdOverride(0);
        pGraphicsView->setAntialiasing(true);

        pGraphicsView->setColorMode(RGraphicsView::FullColor);
        //pGraphicsView->setColorMode(RGraphicsView::GrayScale);

        if (dlg.getBlackBackgroundRadioButton()->isChecked())
        {
            pGraphicsView->setBackgroundColor(QColor(0, 0, 0));
        }
        else if (dlg.getWriteBackgroundRadioButton()->isChecked())
        {
            pGraphicsView->setBackgroundColor(QColor(255, 255, 255));
        }

        double dWidth = dlg.getWidthLineEdit()->getValue();
        double dHeight = dlg.getHeightLineEdit()->getValue();

        int nMargin = 10;

        QString strMargin = dlg.getMarginComboBox()->currentText();
        if (!strMargin.isEmpty() && !strMargin.isNull())
        {
            bool ok = false;
            nMargin = strMargin.toInt(&ok);
            if (!ok)
            {
                nMargin = 10;
            }
        }
        else
        {
            nMargin = 10;
        }

        QString strResolution = dlg.getResolutionComboBox()->currentText();
        if (strResolution != "auto")
        {
            bool ok = false;
            int nResolution = strResolution.toInt(&ok);
            if (!ok)
            {
                this->terminate();
                return;
            }

            dlg.getWidthLineEdit()->setValue(nResolution*dWidth);
            dlg.getHeightLineEdit()->setValue(nResolution*dHeight);

            RBox bb = documentInterface->getDocument().getBoundingBox(true, true);
            if (bb.isValid())
            {
                dWidth = RMath::mround(bb.getWidth()*nResolution + 2*nMargin);
                dHeight = RMath::mround(bb.getHeight()*nResolution + 2*nMargin);
            }
        }

        pGraphicsView->resizeImage(dWidth, dHeight);

        pGraphicsView->autoZoom(nMargin, true);

        scene->regenerate();

        // export file
        QImage buffer = pGraphicsView->getBuffer();

        scene->unregisterView(pGraphicsView);

        QImageWriter iw(bmpFileName);
        QString ext = QFileInfo(bmpFileName).suffix().toLower();
        if (ext == "png")
        {
            iw.setQuality(9);
        }
        else if (ext == "jpg" || ext == "jpeg")
        {
            iw.setQuality(100);
        }
        else if (ext == "tif" || ext == "tiff")
        {
            iw.setCompression(1);
        }
        else if (ext == "bmp")
        {
            iw.setCompression(1);
        }

        if (!iw.write(buffer))
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");
            QString strMsg;
            strMsg.append(tc->toUnicode("导出位图失败 : "));
            strMsg.append(iw.errorString());
            QMessageBox::warning(NULL, tc->toUnicode("警告"), strMsg, QMessageBox::Ok);
        }
    }

    this->terminate();
}

QString EZExportBitmapAction::getFileName()
{
    QList<QByteArray> formats = QImageWriter::supportedImageFormats();
    QStringList filters;

    for (int i = 0; i < formats.length(); ++i)
    {
        QByteArray format = formats[i];
        QByteArray formatAlt = "";
        if (format == "jpg" || format == "tif")
        {
            continue;
        }

        if (format == "jpeg")
        {
            formatAlt = "jpg";
        }
        else if (format == "tiff")
        {
            formatAlt = "tif";
        }

        QByteArray filter = format.toUpper() + " " + "Files" + " (";
        if (formatAlt.length() != 0)
        {
            filter += "*." + formatAlt + " ";
        }
        filter += "*." + format;
        filter += ")";

        filters.push_back(filter);
    }

    return getSaveFileName(NULL, "Export as Bitmap", filters);
}

QString EZExportBitmapAction::getSaveFileName(QWidget* parentWidget, const QString& caption, const QStringList& filterStrings)
{
    QFileDialog fileDialog(parentWidget);
    fileDialog.setWindowTitle(caption);

    // use native dialog:
    fileDialog.setOption(QFileDialog::DontUseNativeDialog, false);

    // overwrite is handled after extension has been added:
    fileDialog.setOption(QFileDialog::DontConfirmOverwrite, true);

    SystemConfigManager& sMgr = SystemConfigManager::GetInstance();
    QString strDir = sMgr.getRecentlyUsedExportBitmapFileDir();

    QFileInfo fiDir(strDir);

    fileDialog.setDirectory(fiDir.absolutePath());
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    fileDialog.setNameFilters(filterStrings);

    fileDialog.selectFile(fiDir.completeBaseName());

    if (fiDir.suffix().length() != 0)
    {
        // preselect first name filter that matches current extension:
        for (int i = 0; i < filterStrings.length(); ++i)
        {
            if (filterStrings[i].contains("*." + fiDir.suffix().toLower()))
            {
                fileDialog.selectNameFilter(filterStrings[i]);
                break;
            }
        }
    }

    fileDialog.setLabelText(QFileDialog::FileType, "Format:");

    QString fileToSave;

    bool done = false;

    while (!done)
    {
        done = true;

        if (!fileDialog.exec())
        {
            return "";
        }

        fileToSave = fileDialog.selectedFiles()[0];

        QFileInfo inf(fileToSave);
        sMgr.setRecentlyUsedDExportBitmapFileDir(inf.path());

        //QString selectedNameFilter = fileDialog.selectedNameFilter();
        // file, possibly without suffix:
        QFileInfo fi(fileToSave);
        /*
        if (fi.suffix().length() == 0)
        {
            QString suffix = "";
            QRegularExpression regex("/(*(.[^ )]*)/i");
            QRegularExpressionMatch match = regex.match(selectedNameFilter);
            if (match.hasMatch())
            {
                suffix = match.captured(1);
            }

            if (suffix.length() != 0)
            {
                fileToSave += suffix;
            }
        }
        */

        // file with suffix:
        fi.setFile(fileToSave);
        if (fi.exists())
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");
            QMessageBox::StandardButtons ret = QMessageBox::warning(NULL,
                                                                    tc->toUnicode("覆盖现有文件？"),
                                                                    QString(tc->toUnicode("文件 '%1' 已经存在。 是否覆盖？")).arg(fileToSave),
                                                                    QMessageBox::Yes,
                                                                    QMessageBox::No);
            if (ret != QMessageBox::Yes)
            {
                done = false;
            }
        }
    }

    return fileToSave;
}
