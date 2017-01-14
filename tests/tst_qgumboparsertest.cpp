#include <QDebug>
#include "tst_qgumboparsertest.h"
#include "../QGumboParser/qgumboattribute.h"

namespace {
const QString validHtml = R"~(
        <!DOCTYPE html><html>
        <head>
            <title>test html text</title>
            <meta charset="UTF-8">
        </head>
        <body><div id="wraPPer">
                <header>
                    <div class="header_container"><p id="target">Some text for testing</p></div>
                    <div class="Left date">date</div>
                </header>
                <article class="left article"><h2>Head</h2>
                    <div class="content"><a href="http://example.com" id="imp">link</a></div>
                </article>
                <footer></footer>
        </div></body></html>
        )~";
}

tst_qgumboparsertest::tst_qgumboparsertest()
{
}

void tst_qgumboparsertest::initTestCase()
{
    try {
        validDocument.reset(new QGumboDocument(QGumboDocument::parse(validHtml)));
    } catch (...) {
        validDocument.reset();
    }

    QVERIFY2(validDocument, "QGumboDocument can't handle valid html");
}

void tst_qgumboparsertest::cleanupTestCase()
{
    validDocument.reset();
}

void tst_qgumboparsertest::interfaceTest()
{
    QVERIFY(validDocument);

    QGumboNode root = validDocument->rootNode();
    QVERIFY(!!root);

    // assinment
    QGumboNode other = root;
    Q_UNUSED(other);

    QGumboNode copied(root);
    QGumboNode moved(std::move(root));
}

void tst_qgumboparsertest::getById()
{
    try {
        QVERIFY(validDocument);

        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        QGumboNodes nodes = root.getElementById(QStringLiteral("taRGet"));
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

void tst_qgumboparsertest::getByTagName()
{
    try {
        QVERIFY(validDocument);

        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        QGumboNodes nodes = root.getElementsByTagName(HtmlTag::HEADER);
        QCOMPARE(nodes.size(), size_t(1));

        nodes = root.getElementsByTagName(HtmlTag::DIV);
        QCOMPARE(nodes.size(), size_t(4));

        nodes = root.getElementsByTagName(HtmlTag::A);
        QCOMPARE(nodes.size(), size_t(1));

    } catch (const std::exception& ex) {
        QString mes("exception was thrown: ");
        mes.append(ex.what());
        QVERIFY2(false, mes.toUtf8().constData());
    }
}

void tst_qgumboparsertest::getAttributes()
{
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
            QCOMPARE(attrs.size(), size_t(2));

            QCOMPARE(attrs.front().name(), QStringLiteral("href"));
            QCOMPARE(attrs.front().value(), QStringLiteral("http://example.com"));
            QCOMPARE(attrs.back().name(), QStringLiteral("id"));
            QCOMPARE(attrs.back().value(), QStringLiteral("imp"));
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

void tst_qgumboparsertest::getByClassName()
{
    try {
        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        {
            QGumboNodes nodes = root.getElementsByClassName(QStringLiteral("article"));
            QCOMPARE(nodes.size(), size_t(1));
        }
        {
            QGumboNodes nodes = root.getElementsByClassName(QStringLiteral("left"));
            QCOMPARE(nodes.size(), size_t(2));

            QCOMPARE(nodes.back().tag(), HtmlTag::ARTICLE);
            QCOMPARE(nodes.front().tagName(), QStringLiteral("div"));
        }
    } catch (const std::exception& ex) {
        QString mes("exception was thrown: ");
        mes.append(ex.what());
        QVERIFY2(false, mes.toUtf8().constData());
    }
}

void tst_qgumboparsertest::childNodes()
{
    try {
        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        QGumboNodes nodes = root.getElementById(QStringLiteral("wrapper"));
        QCOMPARE(nodes.size(), size_t(1));

        QGumboNodes childs = nodes.front().childNodes();
        QCOMPARE(childs.size(), size_t(7));

        QCOMPARE(nodes.front().childElementCount(), 3);

        QGumboNodes children = nodes.front().children();
        QCOMPARE(children.size(), size_t(3));

        HtmlTag tag;

        auto finder = [&tag] (const QGumboNode& node) {
            return node.tag() == tag;
        };

        tag = HtmlTag::FOOTER;

        auto it = std::find_if(children.begin(), children.end(), finder);
        QVERIFY(it != children.end());

        tag = HtmlTag::ARTICLE;

        it = std::find_if(children.begin(), children.end(), finder);
        QVERIFY(it != children.end());

        tag = HtmlTag::HEADER;

        it = std::find_if(children.begin(), children.end(), finder);
        QVERIFY(it != children.end());

    } catch (const std::exception& ex) {
        QString mes("exception was thrown: ");
        mes.append(ex.what());
        QVERIFY2(false, mes.toUtf8().constData());
    }
}

void tst_qgumboparsertest::id()
{
    try {
        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        QCOMPARE(root.id(), QStringLiteral(""));

        QGumboNodes nodes = root.getElementsByTagName(HtmlTag::A);
        QCOMPARE(nodes.size(), size_t(1));

        QGumboNode& node = nodes.front();
        QVERIFY(!!node);

        QCOMPARE(node.id(), QStringLiteral("imp"));

    } catch (const std::exception& ex) {
        QString mes("exception was thrown: ");
        mes.append(ex.what());
        QVERIFY2(false, mes.toUtf8().constData());
    }
}

void tst_qgumboparsertest::classes()
{
    try {
        QGumboNode root = validDocument->rootNode();
        QVERIFY(!!root);

        QGumboNodes nodes = root.getElementsByTagName(HtmlTag::ARTICLE);
        QCOMPARE(nodes.size(), size_t(1));

        QStringList classes = nodes.front().classList();
        QCOMPARE(classes.size(), 2);
        QCOMPARE(classes.front(), QStringLiteral("left"));
        QCOMPARE(classes.back(), QStringLiteral("article"));

    } catch (const std::exception& ex) {
        QString mes("exception was thrown: ");
        mes.append(ex.what());
        QVERIFY2(false, mes.toUtf8().constData());
    }
}

void tst_qgumboparsertest::outerHtmlTest()
{
    QVERIFY(validDocument);

    QGumboNode root = validDocument->rootNode();
    QVERIFY(!!root);

    QGumboNodes nodes = root.getElementById(QStringLiteral("taRGet"));
    QCOMPARE(nodes.size(), size_t(1));
    QGumboNode& node = nodes.front();

    QCOMPARE(node.outerHtml(),
             QStringLiteral(R"~(<p id="target">Some text for testing</p>)~"));

    QGumboNodes items = root.getElementsByTagName(HtmlTag::HEAD);
    QCOMPARE(items.size(), size_t(1));
    QGumboNode head = items.front();

    const auto& ref = QStringLiteral(R"~(<head>
            <title>test html text</title>
            <meta charset="UTF-8">
        </head>)~");
    QCOMPARE(head.outerHtml(), ref);
}

QTEST_MAIN(tst_qgumboparsertest)
