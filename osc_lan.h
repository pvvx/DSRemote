#ifndef OSCLAN_H
#define OSCLAN_H

#include <QObject>
#include <QTcpSocket>

class osclan : public QObject
{
    Q_OBJECT
public:
    explicit osclan(QObject *parent = nullptr);
    int open(const char *);
    void close(void);
    int write(const char * data, int len);
    int read(char * data, int len);
    void flush(void);
signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *sock;
};

extern osclan ocs;

#endif // OSCLAN_H
