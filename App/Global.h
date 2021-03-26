#ifndef GLOBAL
#define GLOBAL
#include <Qt>
#include <string>
#include "CuttingSolutionTable.h"
#include "DataSt.h"
#include "RColor.h"

const int ITEM_TYPE = Qt::UserRole + 1;

// For WorkspaceTreeWidget
const int ITEM_TYPE_ROOT = Qt::UserRole + 2;
const int ITEM_TYPE_MATERIAL_LIST = Qt::UserRole + 3;
const int ITEM_TYPE_MATERIAL_GROUP = Qt::UserRole + 4;
const int ITEM_TYPE_CUTTING_SOLUTION = Qt::UserRole + 5;

const int MATERIAL_LIST_NAME = Qt::UserRole + 6;
const int MATERIAL_LIST_ID = Qt::UserRole + 7;

const int MATERIAL_GROUP_NAME = Qt::UserRole + 8;
const int MATERIAL_GROUP_ID = Qt::UserRole + 9;

const int CUTTING_SOLUTION_NAME = Qt::UserRole + 10;
const int CUTTING_SOLUTION_ID = Qt::UserRole + 11;

const int SOURCE_BOARD_ID = Qt::UserRole + 12;

// For Xbel
const int XBEL_TARGET = Qt::UserRole + 13;

// for MaterialTableWidget
const int MATERIAL_TABLE_DISSIMILAR_ITEM = Qt::UserRole + 14;
const int MATERIAL_TABLE_WIDTH_ITEM = Qt::UserRole + 15;
const int MATERIAL_TABLE_HEIGHT_ITEM = Qt::UserRole + 16;
const int MATERIAL_TABLE_BARCODE_ITEM = Qt::UserRole + 17;
const int MATERIAL_TABLE_FINISHED_PRODUCT_NAME_ITEM = Qt::UserRole + 18;
const int MATERIAL_TABLE_PRODUCT_BOARD_ID = Qt::UserRole + 19;

// For DialogPrintBoards
const int ITEM_TYPE_SOURCE_BOARD = Qt::UserRole + 20;
const int SOURCE_BOARD_INDEX_ITEM = Qt::UserRole + 21;
const int SOURCE_BOARD_ID_ITEM = Qt::UserRole + 22;
const int ITEM_TYPE_BOARD = Qt::UserRole + 23;
const int BOARD_ID_ITEM = Qt::UserRole + 24;
const int BOARD_ORDERS_NO_ITEM = Qt::UserRole + 25;
const int BOARD_BARCODE_ITEM = Qt::UserRole + 26;
const int BOARD_MATERIAL_TYPE_ITEM = Qt::UserRole + 27;
const int BOARD_PRODUCT_NAME_ITEM = Qt::UserRole + 28;
const int BOARD_WIDTH_ITEM = Qt::UserRole + 29;
const int BOARD_HEIGHT_ITEM = Qt::UserRole + 30;
const int BOARD_THICKNESS_ITEM = Qt::UserRole + 31;
const int BOARD_NUM_ITEM = Qt::UserRole + 32;
const int BOARD_EDGE_WEIGHT_ITEM = Qt::UserRole + 33;
const int BOARD_FINISHED_PRODUCT_NAME_ITEM = Qt::UserRole + 34;
const int BOARD_FINISHED_PRODUCT_WIDTH_ITEM = Qt::UserRole + 35;
const int BOARD_FINISHED_PRODUCT_HEIGHT_ITEM = Qt::UserRole + 36;
const int BOARD_FINISHED_PRODUCT_THICKNESS_ITEM = Qt::UserRole + 37;
const int BOARD_DISSIMILAR_SHAPE_ITEM = Qt::UserRole + 38;
const int BOARD_ROTATE_ITEM = Qt::UserRole + 39;
const int BOARD_CLIENT_INFORMATION_ITEM = Qt::UserRole + 40;
const int BOARD_FRANCHIES_STORE_ITEM = Qt::UserRole + 41;
const int BOARD_INDEX_ITEM = Qt::UserRole + 42;

// For ShapeEditorTableWidget
const int ENTITY_ID = Qt::UserRole + 43;

const RColor HOLE_COLOR = RColor(255, 0, 0);
const RColor SLOT_COLOR = RColor(255, 0, 0);

#endif // GLOBAL

