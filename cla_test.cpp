#include <QtTest>

#include "parser.h"

using namespace cla;

class cla_test : public QObject
{
    Q_OBJECT

private slots:
    void test_case1();

};

void cla_test::test_case1()
{
    parser p;

    int argc = 6;
    const char * argv[] = {"--key1", "value", "--key2", "value2", "pos1", "pos2"};

    p.addRequired("key1", 'k', "some description");
    p.addRequired("key2", 'c', "some description");

    p.parse(argc, argv);
    p.dump();

    QVERIFY(true);
}

QTEST_APPLESS_MAIN(cla_test)

#include "cla_test.moc"
