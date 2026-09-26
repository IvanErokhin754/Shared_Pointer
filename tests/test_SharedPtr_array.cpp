#include <gtest/gtest.h>
#include "SharedPtr.hpp"

struct TestObjectDeleteArray{
    static int destructor_count;

    ~TestObjectDeleteArray() {
        destructor_count++;
    }
};

int TestObjectDeleteArray::destructor_count = 0;

TEST(TestSharedPtr, ArrayConstructor) {
    SharedPtr<int> ptr(new int[10], 10);

    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(TestSharedPtr, ArrayAccess) {
    SharedPtr<int> ptr(new int[5]{10, 20, 30, 40, 50}, 5);

    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[2], 30);
    EXPECT_EQ(ptr[3], 40);
    EXPECT_EQ(ptr[4], 50);
}

TEST(TestSharedPtr, ArrayWrite) {
    SharedPtr<int> ptr(new int[3]{10, 20, 30}, 3);

    ptr[1] = 100;

    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 100);
    EXPECT_EQ(ptr[2], 30);
}

TEST(TestSharedPtr, ArrayOutOfRange) {
    SharedPtr<int> ptr(new int[5]{10, 20, 30, 40, 50}, 5);

    EXPECT_THROW(ptr[100], std::out_of_range);
    EXPECT_THROW(ptr[5], std::out_of_range);
}

TEST(TestSharedPtr, ArrayDereferenceThrows) {
    SharedPtr<int> ptr(new int[5], 5);

    EXPECT_TRUE(ptr);
    EXPECT_THROW(*ptr, std::logic_error);
}

struct TestObject {
    int value = 100;
};

TEST(TestSharedPtr, EmptyArrayAccessThrows) {
    SharedPtr<int> ptr;

    EXPECT_THROW(ptr[0], std::logic_error);
}

TEST(TestSharedTest, ArrayArrowThrows) {
    SharedPtr<TestObject> ptr(new TestObject[3], 3);
    EXPECT_THROW(ptr.operator->(), std::logic_error);
}

TEST(TestSharedTest, ArrayCopy) {
    SharedPtr<int> ptr(new int[5]{10, 20, 30, 40, 50}, 5);
    SharedPtr<int> ptr2;

    ptr2 = ptr;

    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr2[0], 10);
    EXPECT_EQ(ptr2[1], 20);
    EXPECT_EQ(ptr2[2], 30);
    EXPECT_EQ(ptr2[3], 40);
    EXPECT_EQ(ptr2[4], 50);

    EXPECT_EQ(ptr.UseCount(), 2);
}

TEST(TestSharedPtr, ArrayMove) {
    SharedPtr<int> ptr(new int[5]{10, 20, 30, 40, 50}, 5);
    SharedPtr<int> ptr2;

    ptr2 = std::move(ptr);

    EXPECT_FALSE(ptr);
    EXPECT_TRUE(ptr2);

    EXPECT_EQ(ptr2[0], 10);
    EXPECT_EQ(ptr2[1], 20);
    EXPECT_EQ(ptr2[2], 30);
    EXPECT_EQ(ptr2[3], 40);
    EXPECT_EQ(ptr2[4], 50);

    EXPECT_EQ(ptr2.UseCount(), 1);
}

TEST(TestSharedPtr, ArrayDeletedOnce) {
    TestObjectDeleteArray::destructor_count = 0;

    {
        SharedPtr<TestObjectDeleteArray> ptr(new TestObjectDeleteArray[5], 5);
        SharedPtr<TestObjectDeleteArray> ptr2(ptr);
        SharedPtr<TestObjectDeleteArray> ptr3(ptr);

        EXPECT_EQ(ptr.UseCount(), 3);
        EXPECT_EQ(TestObjectDeleteArray::destructor_count, 0);
    }

    EXPECT_EQ(TestObjectDeleteArray::destructor_count, 5);
}