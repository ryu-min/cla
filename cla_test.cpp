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

    int argc = 6;
    const char * argv[] = {"--key1", "value", "--key2", "value2", "pos1", "pos2"};

    parser()
            .addRequired("key1", 'k', "some description")
            .addRequired("key2", 'c', "some description")
            .addPositional("some another pos description")
            .addPositional("some another pos description")
            .parse(argc, argv)
            .dump();

    QVERIFY(true);
}

QTEST_APPLESS_MAIN(cla_test)

#include "cla_test.moc"
