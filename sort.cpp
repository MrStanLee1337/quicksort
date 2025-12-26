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
int RUBIKON = 6;

using namespace std;

mt19937 rnd(0);

template <typename T>
inline void mySwap(T& a, T& b) {
    T temp =  move(a);
    a =  move(b);
    b =  move(temp);
}

template <typename T, typename Cmp>
void insertionSort(T* left, T* right, Cmp comp)
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

template <typename T, typename Cmp>
T* getMiddleValue(T* a, T* b, T* c, Cmp comp)
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


template <typename T, typename Cmp>
T* partition(T* left, T* right, Cmp comp) {

    T* middle = left + (right - left) / 2;
    T* pivot = getMiddleValue(left, middle, right - 1, comp);
    mySwap(*pivot, *left);
    T* p = left;
    T* i = left;
    T* j = right;

    while (true) {
        do {
            ++i;
        } while (i < right && comp(*i, *p));
        do {
            --j;
        } while (j > left && comp(*p, *j));
        if (i >= j) break;
        mySwap(*i, *j);
    }
    swap(*p, *j);
    return j;
}

template <typename T, typename Cmp>
void mySort(T* left, T* right, Cmp comp)//right excluded
{
    while(right - left > RUBIKON){
        T* p = partition(left, right, comp);
        size_t leftSize = p - left;
        size_t rightSize = right - (p + 1);
        if (leftSize < rightSize) {
            mySort(left, p, comp);
            left = p + 1; 
        }
        else {
            mySort(p + 1, right, comp);
            right = p;
        }
    }

    insertionSort(left, right, comp);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


class QuickSortBasicTest : public ::testing::Test {
protected:
    void SetUp() override {
        srand(time(nullptr));
    }
};



TEST_F(QuickSortBasicTest, singleElement) {
    int arr[] = { 42 };
    mySort(arr, arr + 1, less<int>());
    EXPECT_EQ(arr[0], 42);
}

TEST_F(QuickSortBasicTest, twoElementsSorted) {
    int arr[] = { 1, 2 };
    mySort(arr, arr + 2, less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST_F(QuickSortBasicTest, twoElementsUnsorted) {
    int arr[] = { 2, 1 };
    mySort(arr, arr + 2, less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST_F(QuickSortBasicTest, threeElements) {
    int arr[] = { 3, 1, 2 };
    mySort(arr, arr + 3, less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST_F(QuickSortBasicTest, alreadySorted) {
    int arr[] = { 1, 2, 3, 4, 5 };
    mySort(arr, arr + 5, less<int>());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

TEST_F(QuickSortBasicTest, reverseSorted) {
    int arr[] = { 5, 4, 3, 2, 1 };
    mySort(arr, arr + 5, less<int>());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

TEST_F(QuickSortBasicTest, withDuplicates) {
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

TEST_F(QuickSortLargeTest, largeRandomArray) {
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

TEST_F(QuickSortLargeTest, veryLargeArray) {
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

TEST_F(QuickSortLargeTest, allSameElements) {
    const int N = 100;
    vector<int> arr(N, 42);

    mySort(arr.data(), arr.data() + N, less<int>());

    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], 42);
    }
}

TEST_F(QuickSortLargeTest, largeReverseSorted) {
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

TEST_F(QuickSortTypesTest, doubleArray) {
    double arr[] = { 3.14, 1.41, 2.71, 0.57, 1.73 };
    double expected[] = { 0.57, 1.41, 1.73, 2.71, 3.14 };

    mySort(arr, arr + 5, less<double>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_DOUBLE_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, floatArray) {
    float arr[] = { 3.14f, 1.41f, 2.71f, 0.57f, 1.73f };
    float expected[] = { 0.57f, 1.41f, 1.73f, 2.71f, 3.14f };

    mySort(arr, arr + 5, less<float>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_FLOAT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, stringArray) {
    string arr[] = { "banana", "apple", "cherry", "date", "elderberry" };
    string expected[] = { "apple", "banana", "cherry", "date", "elderberry" };

    mySort(arr, arr + 5, less< string>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, customStruct) {
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

TEST_F(QuickSortTypesTest, customComparator) {
    int arr[] = { 5, 2, 8, 1, 9 };
    int expected[] = { 9, 8, 5, 2, 1 };

    mySort(arr, arr + 5, greater<int>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, lambdaComparator) {
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

TEST_F(QuickSortCornerCasesTest, minMaxValues) {
    int arr[] = { INT_MAX, INT_MIN, 0, -1, 1 };
    int expected[] = { INT_MIN, -1, 0, 1, INT_MAX };

    mySort(arr, arr + 5, less<int>());

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortCornerCasesTest, manySwapsNeeded) {
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

TEST_F(QuickSortCornerCasesTest, insertionSortThreshold) {
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

TEST_F(QuickSortCornerCasesTest, justAboveInsertionSortThreshold) {
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

TEST_F(QuickSortCornerCasesTest, notStableSort) {
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

TEST_F(QuickSortCornerCasesTest, partitionCheck) {
    int arr[] = { 5, 3, 8, 1, 2, 7, 4, 6 };
    int* p = partition(arr, arr + 8, less<int>());
    for (int i = 0; i < p - arr; ++i) {
        EXPECT_LE(arr[i], *p);
    }
    for (int i = p - arr + 1; i < 8; ++i) {
        EXPECT_GE(arr[i], *p);
    }
}

TEST_F(QuickSortCornerCasesTest, performanceTest) {
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



int main(int argc, char** argv) {
    system("chcp 1251");
    const int n = 10;
    int k = 7;

    int a[10];
    for (int i = 0; i < n; i++) a[i] = i * k % n;

    cout << "Массив:\n";
    for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];

    mySort(a, a + n, [](int a, int b) { return a < b; });

    cout << "Сортированный массив:\n";
    for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];


    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
