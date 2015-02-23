#include "aestest.h"

#include <QDebug>

#include <openssl/aes.h>

AesTest::AesTest(QObject *parent) : QObject(parent)
{
    qDebug() << "simpleTest";

    QByteArray source("This is a plaintext message.");
    qDebug() << "SOURCE:" << source.toHex();

    for (int i = 0; i < 128; i++) {
        QByteArray key128(16, 'z');
        QByteArray cipher = getCiphertextV2(key128, i, source);
        QByteArray plaintext = getPlaintextV2(key128, i, cipher);
        if (plaintext != source) {
            qWarning() << "FAILED:" << i;
            qWarning() << "KEY:   " << key128.toHex();
            qWarning() << "CIPHER:" << cipher.toHex();
            qWarning() << "PLAIN: " << plaintext.toHex();
        }
    }
}

QByteArray AesTest::getCiphertextV2(const QByteArray &key, unsigned int counter, const QByteArray &plaintext)
{
    AES_KEY enc_key;
    AES_set_encrypt_key((const unsigned char*)key.constData(), key.size() * 8, &enc_key);
    QByteArray out(plaintext.size(), '\0');
    QByteArray iv(AES_BLOCK_SIZE, '\0');
    unsigned int ctr = counter;
    intToByteArray(iv, 0, ctr);
    unsigned char ecount[AES_BLOCK_SIZE];
    memset(ecount, 0, AES_BLOCK_SIZE);
    AES_ctr128_encrypt((unsigned char*)plaintext.constData(), (unsigned char*)out.data(),
                       plaintext.size(), &enc_key, (unsigned char*)iv.data(),
                       ecount, &ctr);
    return out;
}

QByteArray AesTest::getPlaintextV2(const QByteArray &key, unsigned int counter, const QByteArray &ciphertext)
{
    AES_KEY enc_key;
    AES_set_encrypt_key((const unsigned char*)key.constData(), key.size() * 8, &enc_key);
    QByteArray out(ciphertext.size(), '\0');
    QByteArray iv(AES_BLOCK_SIZE, '\0');
    unsigned int ctr = counter;
    intToByteArray(iv, 0, ctr);
    unsigned char ecount[AES_BLOCK_SIZE];
    memset(ecount, 0, AES_BLOCK_SIZE);
    AES_ctr128_encrypt((unsigned char*)ciphertext.constData(), (unsigned char*)out.data(),
                       ciphertext.size(), &enc_key, (unsigned char*)iv.data(),
                       ecount, &ctr);
    return out;
}

void AesTest::intToByteArray(QByteArray &input, int offset, int value)
{
    input[offset + 3] = (char)(value % 256);
    input[offset + 2] = (char)((value >> 8) % 256);
    input[offset + 1] = (char)((value >> 16) % 256);
    input[offset]     = (char)((value >> 24) % 256);
}

