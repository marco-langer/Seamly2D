#include "qhash_extensions_tests.h"

#include "core_utils/qhash_extensions.h"

void QHashExtensionsTests::eraseIfTest()
{
    QHash<QString, int> data{
        { "first", 1 }, { "second", 2 }, { "third", 3 }, { "another second", 2 }
    };

    erase_if(data, [](const auto& keyAndValue) { return keyAndValue.second == 2; });

    const QHash<QString, int> expectedData{ { "first", 1 }, { "third", 3 } };

    QCOMPARE(data, expectedData);
}
