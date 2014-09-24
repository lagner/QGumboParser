#include <QDebug>
#include "tst_qgumboparsertest.h"
#include "../QGumboParser/qgumboattribute.h"

namespace {
const QString validHtml = R"~(
        <!DOCTYPE html><html><head></head>
        <body><div id="wrapper">
        <header>
        <div class="header_container"><p id="target">Some text for testing</p></div>
        </header>
        <article class="left article">
        <div class="content"><a href="http://example.com">link</a></div>
        </article>
        <footer></footer>
        </div></body></html>
        )~";
}

tst_qgumboparsertest::tst_qgumboparsertest()
{
}

void tst_qgumboparsertest::initTestCase() {
    try {
        validDocument.reset(new QGumboDocument(QGumboDocument::parse(validHtml)));
    } catch (...) {
        validDocument.reset();
    }

    QVERIFY2(validDocument, "QGumboDocument can't handle valid html");
}

void tst_qgumboparsertest::cleanupTestCase() {
    validDocument.reset();
}

void tst_qgumboparsertest::parse()
{
}

void tst_qgumboparsertest::getById()
{
    try {
        QVERIFY(validDocument);

        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        QGumboNodes nodes = root.getElementById(QStringLiteral("target"));
        QVERIFY(nodes.size() == 1);

        QGumboNode& node = nodes.front();
        QVERIFY(!!node);

        QString value = node.innerText();
        QCOMPARE(value, QStringLiteral("Some text for testing"));

    } catch (const std::exception& ex) {
        QString mes("exception was thrown: ");
        mes.append(ex.what());
        QVERIFY2(false, mes.toUtf8().constData());
    }
}

void tst_qgumboparsertest::getByTagName() {
    try {
        QVERIFY(validDocument);

        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        QGumboNodes nodes = root.getElementsByTagName(HtmlTag::HEADER);
        QCOMPARE(nodes.size(), size_t(1));

        nodes = root.getElementsByTagName(HtmlTag::DIV);
        QCOMPARE(nodes.size(), size_t(3));

        nodes = root.getElementsByTagName(HtmlTag::A);
        QCOMPARE(nodes.size(), size_t(1));

    } catch (const std::exception& ex) {
        QString mes("exception was thrown: ");
        mes.append(ex.what());
        QVERIFY2(false, mes.toUtf8().constData());
    }
}

void tst_qgumboparsertest::getAttributes() {
    try {
        QVERIFY(validDocument);

        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        {
            QGumboNodes nodes = root.getElementsByTagName(HtmlTag::A);
            QCOMPARE(nodes.size(), size_t(1));

            QGumboNode& node = nodes.front();
            QVERIFY(!!node);

            QGumboAttributes attrs = node.allAttributes();
            QCOMPARE(attrs.size(), size_t(1));

            QGumboAttribute& attr = attrs.front();
            QCOMPARE(attr.name(), QStringLiteral("href"));
            QCOMPARE(attr.value(), QStringLiteral("http://example.com"));
        }
        {
            QGumboNodes nodes = root.getElementsByTagName(HtmlTag::P);
            QCOMPARE(nodes.size(), size_t(1));

            QGumboNode node = nodes.front();
            QVERIFY(!!node);

            QGumboAttributes attrs = node.allAttributes();
            QCOMPARE(attrs.size(), size_t(1));

            QGumboAttribute& attr = attrs.front();

            QCOMPARE(attr.name(), QStringLiteral("id"));
            QCOMPARE(attr.value(), QStringLiteral("target"));
        }

    } catch (const std::exception& ex) {
        QString mes("exception was thrown: ");
        mes.append(ex.what());
        QVERIFY2(false, mes.toUtf8().constData());
    }
}

QTEST_MAIN(tst_qgumboparsertest)
