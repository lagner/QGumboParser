#pragma once

#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "../QGumboParser/qgumbodocument.h"
#include "../QGumboParser/qgumbonode.h"
#include <QScopedPointer>

class tst_qgumboparsertest : public QObject
{
    Q_OBJECT
public:
    tst_qgumboparsertest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void interfaceTest();
    void getById();
    void getByTagName();
    void getAttributes();
    void getByClassName();
    void childNodes();
    void id();
    void classes();
    void outerTextTest();

private:
    QScopedPointer<QGumboDocument> validDocument;
};
