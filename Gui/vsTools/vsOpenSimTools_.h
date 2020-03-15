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
    enum MessageType{
        Info,Good,Warning,Error
    };
    Q_ENUM(MessageType)
    vsOpenSimTools(QObject *parent = nullptr);
    static vsOpenSimTools *tools;
    void log(QString message,QString description="",MessageType messageType = MessageType::Info,bool logToConsole = false);
signals:
    void messageLogged(QString message);

private:
    QFile *logFile;
    QTextStream *logStream;
    QMap<MessageType,QString> messageColors;



};

#endif // VSOPENSIMTOOLS_H
