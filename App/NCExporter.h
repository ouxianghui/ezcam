#ifndef NCEXPORTER_H
#define NCEXPORTER_H

#include <QObject>

class NCExporter : public QObject
{
    Q_OBJECT
public:
    explicit NCExporter(QObject *parent = 0);
    ~NCExporter();

signals:

public slots:
};

#endif // NCEXPORTER_H
