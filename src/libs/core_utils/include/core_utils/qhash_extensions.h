#ifndef QHASH_EXTENSIONS_H
#define QHASH_EXTENSIONS_H

#include <QHash>

#if QT_VERSION < QT_VERSION_CHECK(6, 1, 0)
template <typename Key, typename T, typename Predicate>
qsizetype erase_if(QHash<Key, T>& hash, Predicate pred)
{
    auto first{ hash.begin() };
    auto last{ hash.end() };
    qsizetype numErased{};

    while (first != last) {
        if (pred(std::pair<const Key&, T&>{ first.key(), first.value() })) {
            first = hash.erase(first);
            ++numErased;
        } else {
            ++first;
        }
    }

    return numErased;
}
#endif

#endif   // QHASH_EXTENSIONS_H
