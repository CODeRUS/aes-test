#include "aestest.h"

#include <QDebug>

#include <openssl/aes.h>

static void ctr128_inc(unsigned char *counter) {
    unsigned int  n=16;
    unsigned char c;

    do {
        --n;
        c = counter[n];
        ++c;
        counter[n] = c;
        if (c) return;
    } while (n);
}

static void ctr128_inc_aligned(unsigned char *counter) {
    size_t *data,c,n;
    const union { long one; char little; } is_endian = {1};

    if (is_endian.little) {
        ctr128_inc(counter);
        return;
    }

    data = (size_t *)counter;
    n = 16/sizeof(size_t);
    do {
        --n;
        c = data[n];
        ++c;
        data[n] = c;
        if (c) return;
    } while (n);
}

QByteArray AesTest::encryptWithCounter(const QByteArray &key, int counter, const QByteArray &plaintext)
{
    AES_KEY enc_key;
    AES_set_encrypt_key((const unsigned char*)key.constData(), 128, &enc_key);
    QByteArray out(plaintext.size(), '\0');
    QByteArray iv(16, '\0');
    //intToByteArray(iv, 0, counter);
    unsigned int ctr = 0;
    unsigned char ecount[16];
    memset(ecount, 0, 16);
    for (int i = 0; i < counter; i++) {
        AES_encrypt((const unsigned char*)iv.constData(), ecount, &enc_key);
        ctr128_inc_aligned((unsigned char*)iv.data());
    }
    AES_ctr128_encrypt((const unsigned char*)plaintext.constData(), (unsigned char*)out.data(),
                       plaintext.size(), &enc_key, (unsigned char*)iv.data(),
                       ecount, &ctr);
    return out;
}

AesTest::AesTest(QObject *parent) : QObject(parent)
{
    QByteArray source("This is a plaintext message.");
    QByteArray key128(16, 'z');

    for (int i = 0; i < 4096; i++) {
        QByteArray cipher(source.size(), '\0');
        cipher = encryptWithCounter(key128, 1, source);

        QByteArray plain(source.size(), '\0');
        plain = encryptWithCounter(key128, 1, cipher);

        if (plain != source) {
            qDebug() << "failed at" << i;
            qDebug() << "source:" << source.toHex();
            qDebug() << "cipher:" << cipher.toHex();
            qDebug() << "plain: " << plain.toHex();
        }
    }
}

void AesTest::intToByteArray(QByteArray &input, int offset, int value)
{
    input[offset + 3] = (char)(value % 256);
    input[offset + 2] = (char)((value >> 8) % 256);
    input[offset + 1] = (char)((value >> 16) % 256);
    input[offset]     = (char)((value >> 24) % 256);
}
