#include <QtTest>

#include "cla/parser.h"

using namespace cla;

class cla_test : public QObject
{
    Q_OBJECT

private slots:
    void optionalArgTest();
    void requiredArgTest();
};

void cla_test::optionalArgTest()
{
    int argc = 6;
    const char * argv[] = {"--string", "test_value2", "--int", "42", "--double", "42.42"};

    std::string stringValue;
    int intValue;
    double doubleValue;

    parser()
            .addOptional("string", 's', stringValue, "some description")
            .addOptional("int", 'i', intValue,"some description")
            .addOptional("double", 'd', doubleValue, "some description")
            .parse(argc, argv);

    QCOMPARE(stringValue, "test_value2");
    QCOMPARE(intValue, 42);
    QCOMPARE(doubleValue, 42.42);
}

void cla_test::requiredArgTest()
{
    int argc = 6;
    const char * argv[] = {"--string", "test_value2", "--int", "42", "--double", "42.42"};

    std::string stringValue;
    int intValue;
    double doubleValue;

    parser()
            .addRequired("string", 's', stringValue, "some description")
            .addRequired("int", 'i', intValue,"some description")
            .addRequired("double", 'd', doubleValue, "some description")
            .parse(argc, argv);

    QCOMPARE(stringValue, "test_value2");
    QCOMPARE(intValue, 42);
    QCOMPARE(doubleValue, 42.42);
}

QTEST_APPLESS_MAIN(cla_test)

#include "cla_test.moc"
