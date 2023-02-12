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
    const char * argv[] = {"--string", "test_value2", "--int", "42", "--double", "42.42"};

    std::string stringValue;
    int intValue;
    double doubleValue;

    try  {
        parser()
            .addOptional("string", 's', stringValue, "some description")
            .addOptional("int", 'i', intValue,"some description")
            .addOptional("double", 'd', doubleValue, "some description")
            .parse(argc, argv)
            .dump();
    }
    catch ( std::exception & e )
    {
        std::cout << "error" << e.what();
    };

    QCOMPARE(stringValue, "test_value2");
    QCOMPARE(intValue, 42);
    QCOMPARE(doubleValue, 42.42);
}

QTEST_APPLESS_MAIN(cla_test)

#include "cla_test.moc"
