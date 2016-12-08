#include <cstddef>
#include <cstdlib>
#include <string>

#include "TreeMap.h"
#include "../CODEine-master/benchmark.h"
#include "HashMap.h"

namespace {

    template<typename K, typename V>
    using Map = aisdi::TreeMap<K, V>;

    template<class T>
    void benchmarking(int numberEle) {
        T tree;
        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_int_distribution<int> distr(0, numberEle);

        for (int i = 0, val, val2; i < numberEle; i++) {
            val = distr(eng);
            val2 = distr(eng);
            tree[val] = val2;
        }
    }


    void perfomTest() {
        Map<int, std::string> map;
        map[1] = "TODO";
    }

} // namespace

int main(int argc, char **argv) {
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
    for (std::size_t i = 0; i < repeatCount; ++i)
        perfomTest();

    bmk::benchmark<> bm;

    bm.run("TreeMap", 10, benchmarking<aisdi::TreeMap<int, int>>, "number of elements",
           {10, 30, 60, 100, 200, 300, 600, 1000, 2000, 3000, 6000, 10000, 30000, 60000, 100000, 200000});
    bm.run("HashMap", 10, benchmarking<aisdi::HashMap<int, int>>, "number of elements",
           {10, 30, 60, 100, 200, 300, 600, 1000, 2000, 3000, 6000, 10000, 30000, 60000, 100000, 200000});
    bm.serialize("Randomly inserting ints", "TreevsVectorInserting.txt");

}
