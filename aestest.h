#ifndef AESTEST_H
#define AESTEST_H

#include <QObject>

class AesTest : public QObject
{
    Q_OBJECT
public:
    explicit AesTest(QObject *parent = 0);

private:
    QByteArray getCiphertextV2(const QByteArray &key, unsigned int counter, const QByteArray &plaintext);
    QByteArray getPlaintextV2(const QByteArray &key, unsigned int counter, const QByteArray &ciphertext);
    void intToByteArray(QByteArray &input, int offset, int value);
};

#endif // AESTEST_H
