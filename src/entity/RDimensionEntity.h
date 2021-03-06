/**
 * Copyright (c) 2011-2016 by Andrew Mustun. All rights reserved.
 * 
 * This file is part of the QCAD project.
 *
 * QCAD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QCAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QCAD.
 */

#ifndef RDIMENSIONENTITY_H
#define RDIMENSIONENTITY_H

#include "entity_global.h"

#include "REntity.h"
#include "RDimensionData.h"

class RDocument;
class RExporter;

/**
 * Base class for dimension entity classes.
 *
 * \scriptable
 * \sharedPointerSupport
 * \ingroup entity
 */
class QCADENTITY_EXPORT RDimensionEntity: public REntity {

    Q_DECLARE_TR_FUNCTIONS(RDimensionEntity)

public:
    static RPropertyTypeId PropertyCustom;
    static RPropertyTypeId PropertyHandle;
    static RPropertyTypeId PropertyProtected;
    static RPropertyTypeId PropertyType;
    static RPropertyTypeId PropertyBlock;
    static RPropertyTypeId PropertyLayer;
    static RPropertyTypeId PropertyLinetype;
    static RPropertyTypeId PropertyLinetypeScale;
    static RPropertyTypeId PropertyLineweight;
    static RPropertyTypeId PropertyColor;
    static RPropertyTypeId PropertyDisplayedColor;
    static RPropertyTypeId PropertyDrawOrder;

    static RPropertyTypeId PropertyDefinitionPointX;
    static RPropertyTypeId PropertyDefinitionPointY;
    static RPropertyTypeId PropertyDefinitionPointZ;
    static RPropertyTypeId PropertyMiddleOfTextX;
    static RPropertyTypeId PropertyMiddleOfTextY;
    static RPropertyTypeId PropertyMiddleOfTextZ;
    static RPropertyTypeId PropertyText;
    static RPropertyTypeId PropertyUpperTolerance;
    static RPropertyTypeId PropertyLowerTolerance;

    static RPropertyTypeId PropertyLinearFactor;
    static RPropertyTypeId PropertyDimScale;
    static RPropertyTypeId PropertyDimBlockName;
    static RPropertyTypeId PropertyAutoTextPos;
    static RPropertyTypeId PropertyFontName;
    //static RPropertyTypeId PropertyHeight;
    //static RPropertyTypeId PropertyAngle;
    //static RPropertyTypeId PropertyLineSpacingFactor;
    //static RPropertyTypeId PropertyHAlign;
    //static RPropertyTypeId PropertyVAlign;

    static RPropertyTypeId PropertyAutoLabel;
    static RPropertyTypeId PropertyMeasuredValue;

public:
    RDimensionEntity(RDocument* document);
    virtual ~RDimensionEntity();

    static void init();

    static QSet<RPropertyTypeId> getStaticPropertyTypeIds() {
        return RPropertyTypeId::getPropertyTypeIds(typeid(RDimensionEntity));
    }

    virtual RS::EntityType getType() const {
        return RS::EntityDimension;
    }

    bool setProperty(RPropertyTypeId propertyTypeId, const QVariant& value,
        RTransaction* transaction=NULL);
    QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId& propertyTypeId,
            bool humanReadable = false, bool noAttributes = false);

    virtual void exportEntity(RExporter& e, bool preview=false, bool forceSelected=false) const;

    virtual RDimensionData& getData() = 0;

    virtual const RDimensionData& getData() const = 0;

    virtual bool isValid() const {
        return getData().isValid();
    }

    void setDefinitionPoint(const RVector& p) {
        getData().setDefinitionPoint(p);
    }

    RVector getDefinitionPoint() const {
        return getData().getDefinitionPoint();
    }

    void setText(const QString& t) {
        getData().setText(t);
    }

    QString getText() const {
        return getData().getText();
    }

    QString getMeasurement(bool resolveAutoMeasurement = true) const {
        return getData().getMeasurement(resolveAutoMeasurement);
    }

    RTextData getTextData() const {
        return getData().getTextData();
    }

    void setTextPosition(const RVector& p) {
        getData().setTextPosition(p);
    }

    RVector getTextPosition() const {
        return getData().getTextPosition();
    }

    void setFontName(const QString& fn) {
        getData().setFontName(fn);
    }

    QString getFontName() const {
        return getData().getFontName();
    }

    bool hasCustomTextPosition() const {
        return getData().hasCustomTextPosition();
    }

    void setCustomTextPosition(bool on) {
        getData().setCustomTextPosition(on);
    }

    RS::TextLineSpacingStyle getLineSpacingStyle() const {
        return getData().getLineSpacingStyle();
    }

    double getLineSpacingFactor() const {
        return getData().getLineSpacingFactor();
    }

    double getTextAngle() const {
        return getData().getTextAngle();
    }

    double getLinearFactor() const {
        return getData().getLinearFactor();
    }

    void setLinearFactor(double f) {
        getData().setLinearFactor(f);
    }

    double getDimScale() const {
        return getData().getDimScale();
    }

    void setDimScale(double f) {
        getData().setDimScale(f);
    }

    QString getDimBlockName() const {
        return getData().getDimBlockName();
    }

    bool hasDimensionBlockReference() const {
        return getData().hasDimensionBlockReference();
    }

protected:
    virtual void print(QDebug dbg) const;
};

Q_DECLARE_METATYPE(RDimensionEntity*)
Q_DECLARE_METATYPE(QSharedPointer<RDimensionEntity>)
Q_DECLARE_METATYPE(QSharedPointer<RDimensionEntity>*)

#endif
