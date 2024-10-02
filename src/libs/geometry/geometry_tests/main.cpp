#include "geometry_test.h"

#include <QtTest/QtTest>

#include <memory>


int main(int argc, char** argv)
{
    int status = 0;
    auto ASSERT_TEST = [&status, argc, argv](std::unique_ptr<QObject> sut) {
        status |= QTest::qExec(sut.get(), argc, argv);
    };

    ASSERT_TEST(std::make_unique<GeometryTests>());
}
