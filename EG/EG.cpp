#include "EG.h"
#include "RLine.h"
#include "RCircle.h"
#include "RMemoryStorage.h"
#include "RSpatialIndexNavel.h"
#include "RDocumentInterface.h"
#include "RDocument.h"
#include "RClipboardOperation.h"
#include "RCopyOperation.h"
#include "RPasteOperation.h"
#include "ProjectConfigManager.h"
#include "RUnit.h"

EG::EG()
{
}


RArc EG::getArc2PR(RVector point1,
                   RVector point2,
                   double radius,
                   bool reversed,
                   bool alternativeSolution)
{
    if (!point1.isValid() || !point2.isValid()) {
        return RArc();
    }

    if (radius <= 0.0 || radius > 1.0e6) {
        return RArc();
    }

    if (point1.equalsFuzzy(point2)) {
        return RArc();
    }

    RVector center;
    RCircle circle1(point1, radius);
    RCircle circle2(point2, radius);

    QList<RVector> ips = circle1.getIntersectionPoints(circle2, false);

    if (ips.length() == 2) {
        RVector ipRight, ipLeft;
        RLine line(point1, point2);
        if (line.getSideOfPoint(ips[0]) == RS::RightHand) {
            ipRight = ips[0];
            ipLeft = ips[1];
        }
        else {
            ipLeft = ips[0];
            ipRight = ips[1];
        }

        // XOR:
        if ((reversed == true) != (alternativeSolution == true)) {
            center = ipRight;
        }
        else {
            center = ipLeft;
        }
    }
    else {
        // center in the middle:
        RVector v;
        v.setPolar(radius, point1.getAngleTo(point2));
        center = point1 + v;
        point2 = point1 + (v * 2.0);
    }

    double angle1 = center.getAngleTo(point1);
    double angle2 = center.getAngleTo(point2);

    return RArc(center, radius, angle1, angle2, reversed);
}

bool EG::copyShapes2Document(const QString& fileName, const RVector& pt, double rotationDegrees, bool flipH, bool flipV, RDocument& dst)
{
    RMemoryStorage* storage = new RMemoryStorage();
    RSpatialIndexNavel* spatialIndex = new RSpatialIndexNavel();
    RDocument* document = new RDocument(*storage, *spatialIndex);

   RDocumentInterface* srcDi = new RDocumentInterface(*document);

   // import a DXF file into the drawing:
   if (srcDi->importFile(fileName, "") != RDocumentInterface::IoErrorNoError)
   {
       return false;
   }

   // 复制到剪切板
   srcDi->selectAll();

   RVector offset = srcDi->getDocument().getSelectionBox().getCenter().getNegated();

   RCopyOperation* copy = new RCopyOperation(offset, srcDi->getDocument());

   RDocumentInterface& clipBoard = srcDi->getClipboard();
   clipBoard.getDocument().setUnit(srcDi->getDocument().getUnit());
   clipBoard.applyOperation(copy);
   clipBoard.getDocument().setUnit(srcDi->getDocument().getUnit());
   clipBoard.autoZoom();

   // 粘贴到目标文档
   double scale = 1.0;
   double rotation = RMath::deg2rad(rotationDegrees);
   bool toCurrentLayer = false;
   bool overwriteLayers = false;
   bool overwriteBlocks = false;

   RPasteOperation* paste = new RPasteOperation(clipBoard.getDocument());
   paste->setText("");
   paste->setOffset(pt);
   paste->setScale(scale);
   paste->setRotation(rotation);
   paste->setFlipVertical(flipV);
   paste->setFlipHorizontal(flipH);
   paste->setToCurrentLayer(toCurrentLayer);
   paste->setOverwriteLayers(overwriteLayers);
   paste->setOverwriteBlocks(overwriteBlocks);
   //double unitFactor = RUnit::convert(1.0, clipBoard.getDocument().getUnit(), dstDi->getDocument().getUnit());
   //boundary = paste->getBoundary(unitFactor);

   paste->apply(dst);

   delete srcDi;
   srcDi = NULL;

    return true;
}

bool EG::copyShapes2Document(const QString& fileName, const RVector& pt, double rotationDegrees, bool flipH, bool flipV, RDocumentInterface* dst)
{
    return copyShapes2Document(fileName, pt, rotationDegrees, flipH, flipV, dst->getDocument());
}
