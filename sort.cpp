#include <iostream>
#include <vector>
#include <utility>

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <random>
#include <string>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
int RUBIKON = 16;

using namespace std;

mt19937 rnd(0);

template <typename T>
inline void mySwap(T& a, T& b) {
    T temp =  move(a);
    a =  move(b);
    b =  move(temp);
}

template <typename T, typename Compare>
void insertionSort(T* left, T* right, Compare comp)
{
    if (left == right) return;

    for (T* i = left + 1; i != right; ++i) {
        T temp =  move(*i);
        T* j = i;

        while (j > left && comp(temp, *(j - 1))) {
            *j =  move(*(j - 1));
            --j;
        }

        *j =  move(temp);
    }
}

template <typename T, typename Compare>
T* getMiddleValue(T* a, T* b, T* c, Compare comp)
{
    if (comp(*a, *b)) {
        if (comp(*b, *c)) return b;
        if (comp(*a, *c)) return c;
        return a;
    } else {
        if (comp(*a, *c)) return a;
        if (comp(*b, *c)) return c;
        return b;
    }
}

template <typename T, typename Compare>
T* partition(T* left, T* right, T* pivot, Compare comp) {
    mySwap(*pivot, *(right - 1));
    T* store = left;

    for (T* p = left; p != right - 1; ++p){
        if (comp(*p, *(right - 1))) {
            mySwap(*p, *store);
            ++store;
        }
    }

    mySwap(*store, *(right - 1));
    return store;
}

template <typename T, typename Compare>
void mySort(T* left, T* right, Compare comp)
{
    while (right - left > 1) {
        if (right - left <= RUBIKON) {
            insertionSort(left, right, comp);
            return;
        }

        T* middle = left + (right - left) / 2;
        T* pivot = getMiddleValue(left, middle, right - 1, comp);
        T* p = partition(left, right, pivot, comp);

        if (p - left < right - p - 1) {
            mySort(left, p, comp);
            left = p + 1;
        } else {
            mySort(p + 1, right, comp);
            right = p;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


class QuickSortBasicTest : public ::testing::Test {
protected:
    void SetUp() override {
        srand(time(nullptr));
    }
};



TEST_F(QuickSortBasicTest, SingleElement) {
    int arr[] = { 42 };
    mySort(arr, arr + 1, less<int>());
    EXPECT_EQ(arr[0], 42);
}

TEST_F(QuickSortBasicTest, TwoElementsSorted) {
    int arr[] = { 1, 2 };
    mySort(arr, arr + 2, less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST_F(QuickSortBasicTest, TwoElementsUnsorted) {
    int arr[] = { 2, 1 };
    mySort(arr, arr + 2, less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST_F(QuickSortBasicTest, ThreeElements) {
    int arr[] = { 3, 1, 2 };
    mySort(arr, arr + 3, less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST_F(QuickSortBasicTest, AlreadySorted) {
    int arr[] = { 1, 2, 3, 4, 5 };
    mySort(arr, arr + 5, less<int>());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

TEST_F(QuickSortBasicTest, ReverseSorted) {
    int arr[] = { 5, 4, 3, 2, 1 };
    mySort(arr, arr + 5, less<int>());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

TEST_F(QuickSortBasicTest, WithDuplicates) {
    int arr[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3 };
    int expected[] = { 1, 1, 2, 3, 3, 4, 5, 5, 6, 9 };
    mySort(arr, arr + 10, less<int>());
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

class QuickSortLargeTest : public ::testing::Test {
protected:
    void SetUp() override {
        srand(time(nullptr));
    }
};

TEST_F(QuickSortLargeTest, LargeRandomArray) {
    const int N = 1000;
    vector<int> arr(N);

    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 20001 - 10000;
    }

    vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());

    mySort(arr.data(), arr.data() + N, less<int>());

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
    }
}

TEST_F(QuickSortLargeTest, VeryLargeArray) {
    const int N = 10000;
    vector<int> arr(N);

    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 100001 - 50000;
    }

    vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());

    mySort(arr.data(), arr.data() + N, less<int>());

    for (int i = 0; i < N - 1; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]) << "Not sorted at index " << i;
    }
}

TEST_F(QuickSortLargeTest, AllSameElements) {
    const int N = 100;
    vector<int> arr(N, 42);

    mySort(arr.data(), arr.data() + N, less<int>());

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], 42);
    }
}

TEST_F(QuickSortLargeTest, LargeReverseSorted) {
    const int N = 500;
    vector<int> arr(N);

    for (int i = 0; i < N; ++i) {
        arr[i] = N - i;
    }

    mySort(arr.data(), arr.data() + N, less<int>());

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

class QuickSortTypesTest : public ::testing::Test {};

TEST_F(QuickSortTypesTest, DoubleArray) {
    double arr[] = { 3.14, 1.41, 2.71, 0.57, 1.73 };
    double expected[] = { 0.57, 1.41, 1.73, 2.71, 3.14 };

    mySort(arr, arr + 5, less<double>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_DOUBLE_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, FloatArray) {
    float arr[] = { 3.14f, 1.41f, 2.71f, 0.57f, 1.73f };
    float expected[] = { 0.57f, 1.41f, 1.73f, 2.71f, 3.14f };

    mySort(arr, arr + 5, less<float>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_FLOAT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, StringArray) {
    string arr[] = { "banana", "apple", "cherry", "date", "elderberry" };
    string expected[] = { "apple", "banana", "cherry", "date", "elderberry" };

    mySort(arr, arr + 5, less< string>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, CustomStruct) {
    struct Person {
        string name;
        int age;

        bool operator<(const Person& other) const {
            return age < other.age;
        }
    };

    Person people[] = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35},
        {"David", 20}
    };

    mySort(people, people + 4, less<Person>());

    EXPECT_EQ(people[0].name, "David");
    EXPECT_EQ(people[0].age, 20);
    EXPECT_EQ(people[1].name, "Bob");
    EXPECT_EQ(people[1].age, 25);
    EXPECT_EQ(people[2].name, "Alice");
    EXPECT_EQ(people[2].age, 30);
    EXPECT_EQ(people[3].name, "Charlie");
    EXPECT_EQ(people[3].age, 35);
}

TEST_F(QuickSortTypesTest, CustomComparator) {
    int arr[] = { 5, 2, 8, 1, 9 };
    int expected[] = { 9, 8, 5, 2, 1 };

    mySort(arr, arr + 5, greater<int>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, LambdaComparator) {
    vector< string> arr = { "banana", "apple", "Cherry", "Date" };
    vector< string> expected = { "apple", "banana", "Cherry", "Date" };

    auto case_insensitive = [](const  string& a, const  string& b) {
        string a_lower, b_lower;
        transform(a.begin(), a.end(), back_inserter(a_lower), ::tolower);
        transform(b.begin(), b.end(), back_inserter(b_lower), ::tolower);
        return a_lower < b_lower;
        };

    mySort(arr.data(), arr.data() + arr.size(), case_insensitive);

    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

class QuickSortArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        srand(time(nullptr));
    }
};



class QuickSortCornerCasesTest : public ::testing::Test {};

TEST_F(QuickSortCornerCasesTest, MinMaxValues) {
    int arr[] = { INT_MAX, INT_MIN, 0, -1, 1 };
    int expected[] = { INT_MIN, -1, 0, 1, INT_MAX };

    mySort(arr, arr + 5, less<int>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortCornerCasesTest, ManySwapsNeeded) {
    const int N = 100;
    vector<int> arr(N);

    for (int i = 0; i < N / 2; ++i) {
        arr[2 * i] = N - i - 1;
        arr[2 * i + 1] = i;
    }

    vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());

    mySort(arr.data(), arr.data() + N, less<int>());

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
    }
}

TEST_F(QuickSortCornerCasesTest, InsertionSortThreshold) {
    const int N = RUBIKON;
    vector<int> arr(N);

    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 101;
    }

    vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());

    mySort(arr.data(), arr.data() + N, less<int>());

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
    }
}

TEST_F(QuickSortCornerCasesTest, JustAboveInsertionSortThreshold) {
    const int N = RUBIKON + 1;
    vector<int> arr(N);

    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 101;
    }

    vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());

    mySort(arr.data(), arr.data() + N, less<int>());

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
    }
}

TEST_F(QuickSortCornerCasesTest, NotStableSort) {
    struct Item {
        int key;
        int value;

        bool operator<(const Item& other) const {
            return key < other.key;
        }
    };

    Item items[] = {
        {1, 100},
        {2, 200},
        {1, 300},
        {3, 400},
        {2, 500}
    };

    mySort(items, items + 5, less<Item>());

    for (int i = 0; i < 4; ++i) {
        EXPECT_LE(items[i].key, items[i + 1].key);
    }
}

TEST_F(QuickSortCornerCasesTest, middleCheck) {
    int a = 1, b = 2, c = 3;
    int* med = getMiddleValue(&a, &b, &c, less<int>());
    EXPECT_EQ(*med, 2);

    a = 3; b = 2; c = 1;
    med = getMiddleValue(&a, &b, &c, less<int>());
    EXPECT_EQ(*med, 2);

    a = 2; b = 3; c = 1;
    med = getMiddleValue(&a, &b, &c, less<int>());
    EXPECT_EQ(*med, 2);

    a = 2; b = 1; c = 3;
    med = getMiddleValue(&a, &b, &c, less<int>());
    EXPECT_EQ(*med, 2);

    a = 3; b = 1; c = 2;
    med = getMiddleValue(&a, &b, &c, less<int>());
    EXPECT_EQ(*med, 2);

    a = 1; b = 3; c = 2;
    med = getMiddleValue(&a, &b, &c, less<int>());
    EXPECT_EQ(*med, 2);
}

TEST_F(QuickSortCornerCasesTest, PartitionCheck) {
    int arr[] = { 5, 3, 8, 1, 2, 7, 4, 6 };
    int* pivot = &arr[3];

    int* p = partition(arr, arr + 8, pivot, less<int>());
    for (int i = 0; i < p - arr; ++i) {
        EXPECT_LE(arr[i], *p);
    }
    for (int i = p - arr + 1; i < 8; ++i) {
        EXPECT_GE(arr[i], *p);
    }
}

TEST_F(QuickSortCornerCasesTest, PerformanceTest) {
    const int N = 100000;
    vector<int> arr(N);

    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 1000001;
    }

    EXPECT_NO_THROW({
        mySort(arr.data(), arr.data() + N,  less<int>());
        });

    for (int i = 0; i < N - 1; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]) << "Not sorted at index " << i;
    }
}




void findRubikon() {
    ofstream file1("insertion.txt");
    ofstream file2("quicksort.txt");
    for (int i = 1; i < 100; i++) {
        
        int* a = new int [i]; for (int j = 0; j < i; j++) a[j] = rnd();
        RUBIKON = i + 1;
        auto start = std::chrono::steady_clock::now();
        mySort(a, a + i, [](int a, int b) { return a < b; });
        auto end = std::chrono::steady_clock::now();
        auto res = end - start;
        cout << res.count();
        file1 << res.count() << '\n';
        
        RUBIKON = i + 1;
        start = std::chrono::steady_clock::now();
        mySort(a, a + i, [](int a, int b) { return a < b; });
        end = std::chrono::steady_clock::now();
        res = end - start;

        file2 << res.count() << '\n';
        delete[] a;
    }
    file1.close();
    file2.close();
}
int main(int argc, char** argv) {
    const int n = 10;
    int k = 7;

    int a[10];
    for (int i = 0; i < n; i++) a[i] = i * k % n;

    cout << "Массив:\n";
    for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];

    mySort(a, a + n, [](int a, int b) { return a < b; });

    cout << "Сортированный массив:\n";
    for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];

    findRubikon();


    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
    return 0;
}
