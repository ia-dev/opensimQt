/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsOpenSimTools_.h"

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

    messageColors.insert(MessageType::Info,"#000000");
    messageColors.insert(MessageType::Good,"#55B83B");
    messageColors.insert(MessageType::Warning,"#ffff00");
    messageColors.insert(MessageType::Error,"#ff0000");
}

void vsOpenSimTools::log(QString message, QString description,MessageType messageType,bool logToConsole)
{
    QString typeText = QVariant::fromValue(messageType).toString();
    QString messageLine = (description==""?"":(description+"::")) + message;
    *logStream << messageLine  ;
    logStream->flush();
    QString consoleLine ="<p><span style='color:" + messageColors.value(messageType,"#000000") +"'>#" + typeText +"::</span> "
                          +  messageLine + "</p>";
    if(logToConsole)
        emit messageLogged(consoleLine);


}
