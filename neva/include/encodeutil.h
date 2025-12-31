#ifndef ENCODEUTIL_H_INCLUDED
#define ENCODEUTIL_H_INCLUDED

extern int code_convert(
    const char *fromcode,
    const char *tocode,
    const char **inbuf,
    size_t *inbytesleft,
    char **outbuf,
    size_t *outbytesleft
);
extern int g2u(
    const char **inbuf,
    size_t *inbytesleft,
    char **outbuf,
    size_t *outbytesleft
);
extern int u2g(
    const char **inbuf,
    size_t *inbytesleft,
    char **outbuf,
    size_t *outbytesleft
);
// the original implementation does not include this function
extern size_t ConvertGBKToUtf8(
    char *outbuf,
    size_t outbytesleft,
    const char *inbuf,
    size_t inbytesleft
);
extern size_t ConvertGBKToUtf8(
    std::string &outbuf,
    const char *inbuf,
    size_t inbytesleft
);
extern size_t ConvertUtf8ToGBK(
    char *outbuf,
    size_t outbytesleft,
    const char *inbuf,
    size_t inbytesleft
);
extern size_t ConvertUtf8ToGBK(
    const char *inbuf,
    size_t inbytesleft,
    std::string &outbuf
);

#endif // ENCODEUTIL_H_INCLUDED
