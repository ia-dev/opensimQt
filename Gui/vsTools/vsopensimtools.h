/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSOPENSIMTOOLS_H
#define VSOPENSIMTOOLS_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QWidget>

class vsOpenSimTools : public QObject
{
    Q_OBJECT
public:
    vsOpenSimTools(QObject *parent = nullptr);
    void logToFile(QString message);
    static vsOpenSimTools *tools;

private:
    QFile *logFile;
    QTextStream *logStream;



};

#endif // VSOPENSIMTOOLS_H
