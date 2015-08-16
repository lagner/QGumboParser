#ifndef QGUMBOPARSER_H
#define QGUMBOPARSER_H

#include "gumbo-parser/src/gumbo.h"

struct GumboInternalOutput;
class QByteArray;
class QString;
class QGumboDocument;
class QGumboNode;

class QGumboParser {
public:
    QGumboParser(const char* rawUtf8String);
    QGumboParser(const QByteArray& rawData);
    QGumboParser(const QString& data);

    ~QGumboParser();

    QGumboDocument document() const;

    QGumboNode root() const;

    GumboOutput* output() const {
        return output_;
    }

private:
    GumboInternalOutput* output_;
};

#endif // QGUMBOPARSER_H
