#ifndef ALGOTYPEDEF
#define ALGOTYPEDEF

#include <QPoint>
#include <QRect>
#include <string>

typedef struct Border
{
    int t;
    int r;
    int b;
    int l;
}Border;

typedef struct RectNode
{
    QRect rect;
    Border border;
}RectNode;

typedef struct InputRect
{
    int binIndex;
    QPoint pos;
    QSize size;
    QSize sizeCurrent;
    bool rotated;
    std::string barcodeNo;
    std::string name;

}InputRect;

#endif // ALGOTYPEDEF

