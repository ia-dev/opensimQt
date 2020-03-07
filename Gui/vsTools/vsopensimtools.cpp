/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsOpenSimTools.h"

#include <QDebug>
#include <QTime>

vsOpenSimTools* vsOpenSimTools::tools = new vsOpenSimTools();

vsOpenSimTools::vsOpenSimTools(QObject *parent):QObject(parent),
    logFile(new QFile("OpenSimQtLog_.txt"))
{
    logFile->open(QFile::WriteOnly|QFile::Truncate);
    if(logFile->isOpen()) qDebug() << "it is open , the file";
    logStream = new QTextStream(logFile);
    *logStream << "Launching OpenSimQt ...";
    logStream->flush();
}
