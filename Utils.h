#ifndef UTILS_H
#define UTILS_H

#include <QDebug>

//#define SHOW_THREAD_ID

#if defined (SHOW_THREAD_ID)
#include <QThread>

#define _DEBUG() (qDebug() << QThread::currentThreadId() << "-")
#else
#define _DEBUG() (qDebug())
#endif

#define DEBUG() (_DEBUG() << QString("%1::%2").arg(this->metaObject()->className()).arg(__FUNCTION__).toUtf8().data())

#endif // UTILS_H
