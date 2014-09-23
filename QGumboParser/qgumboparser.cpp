#include <QString>
#include <QByteArray>
#include "qgumboparser.h"
#include "qgumbodocument.h"
#include "qgumbonode.h"
#include "gumbo/gumbo.h"


QGumboParser::QGumboParser(const char* rawUtf8String) {
    if (!rawUtf8String)
        throw std::logic_error("can't create parser without HTML");

    output_ = gumbo_parse(rawUtf8String);

    if (!output_)
        throw std::runtime_error("gumbo parser has returned nullptr");
}

QGumboParser::QGumboParser(const QByteArray& rawData) :
    QGumboParser(rawData.constData()) {
}

QGumboParser::QGumboParser(const QString& data) :
    QGumboParser(data.toUtf8().constData()) {
}

QGumboParser::~QGumboParser() {
    if (output_) {
        gumbo_destroy_output(&kGumboDefaultOptions, output_);
    }
}

QGumboDocument QGumboParser::document() const {
    Q_ASSERT(output_);
    return QGumboDocument();
}

QGumboNode QGumboParser::root() const {
    Q_ASSERT(output_);
    return QGumboNode(output_->root);
}
