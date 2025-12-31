#include "all.h"
#include "encodeutil.h"

int code_convert(
    const char *fromcode,
    const char *tocode,
    const char **inbuf,
    size_t *inbytesleft,
    char **outbuf,
    size_t *outbytesleft
)
{
    iconv_t cd = iconv_open(tocode, fromcode);

    if (cd == reinterpret_cast<iconv_t>(-1))
        return -1;

    if (
        iconv(
            cd,
            const_cast<char **>(inbuf),
            inbytesleft,
            outbuf,
            outbytesleft
        ) == static_cast<size_t>(-1)
    )
        return -1;

    iconv_close(cd);
    return 0;
}

int g2u(
    const char **inbuf,
    size_t *inbytesleft,
    char **outbuf,
    size_t *outbytesleft
)
{
    return code_convert("gbk", "utf-8", inbuf, inbytesleft, outbuf, outbytesleft);
}

int u2g(
    const char **inbuf,
    size_t *inbytesleft,
    char **outbuf,
    size_t *outbytesleft
)
{
    return code_convert("utf-8", "gbk", inbuf, inbytesleft, outbuf, outbytesleft);
}

size_t ConvertGBKToUtf8(
    char *outbuf,
    size_t outbytesleft,
    const char *inbuf,
    size_t inbytesleft
)
{
    memset(outbuf,0,outbytesleft);
    g2u(&inbuf, &inbytesleft, &outbuf, &outbytesleft);
    return --outbytesleft;
}

size_t ConvertGBKToUtf8(
    std::string &outbuf,
    const char *inbuf,
    size_t inbytesleft
)
{
    size_t len = 3 * inbytesleft;
    char *outb = new char[len], *tmpb = outb;
    g2u(&inbuf, &inbytesleft, &tmpb, &len);
    outbuf.clear();
    outbuf.assign(outb, --len);
    delete[] outb;
    outb = nullptr;
    return len;
}

size_t ConvertUtf8ToGBK(
    char *outbuf,
    size_t outbytesleft,
    const char *inbuf,
    size_t inbytesleft
)
{
    memset(outbuf,0,outbytesleft);
    u2g(&inbuf, &inbytesleft, &outbuf, &outbytesleft);
    return --outbytesleft;
}

size_t ConvertUtf8ToGBK(
    const char *inbuf,
    size_t inbytesleft,
    std::string &outbuf
)
{
    size_t len = 4 * inbytesleft;
    char *outb = new char[len], *tmpb = outb;
    u2g(&inbuf, &inbytesleft, &tmpb, &len);
    outbuf.clear();
    outbuf.assign(outb, --len);
    delete[] outb;
    outb = nullptr;
    return len;
}
