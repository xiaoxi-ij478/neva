#ifndef SUDES_H_INCLUDED
#define SUDES_H_INCLUDED

class CSuDES
{
    public:
        CSuDES();
        virtual ~CSuDES();

        int Decrypts(char *buf, unsigned buflen) const;
        int Encrypts(char *buf, unsigned buflen) const;
        int SetIVBuf(const char *iv, unsigned ivlen);
        int SetKeyBuf(const char *key, unsigned keylen);

    private:
        char *ivbuf;
        char *keybuf;
};

#endif // SUDES_H_INCLUDED
