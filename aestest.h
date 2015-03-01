#ifndef AESTEST_H
#define AESTEST_H

#include <QObject>

class AesTest : public QObject
{
    Q_OBJECT
public:
    explicit AesTest(QObject *parent = 0);

private:
    QByteArray encryptWithCounter(const QByteArray &key, int counter, const QByteArray &plaintext);
    void intToByteArray(QByteArray &input, int offset, int value);
};

#endif // AESTEST_H
