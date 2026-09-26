#include <gtest/gtest.h>
#include "SharedPtr.hpp"

struct TestObjectDeleteSingle{
    static int destructor_count;

    ~TestObjectDeleteSingle() {
        destructor_count++;
    }
};

int TestObjectDeleteSingle::destructor_count = 0;

TEST(TestSharedPtr, DefaultConstructorTest) {
    SharedPtr<int> ptr;

    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_THROW(*ptr, std::logic_error);
}


TEST(TestSharedPtr, ConstructorFromRawPointer) {
    SharedPtr<int> ptr(new int(10));

    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, 10);
    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(TestSharedPtr, CopyConstructor) {
    SharedPtr<int> ptr(new int(10));
    SharedPtr<int> ptr2(ptr);

    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr.Get(), ptr2.Get());
    EXPECT_EQ(*ptr2, 10);
    EXPECT_EQ(ptr2.UseCount(), 2);
}

TEST(TestSharedPtr, MoveConstructor) {
    int *a = new int(10);
    SharedPtr<int> ptr(a);
    SharedPtr<int> ptr2(std::move(ptr));

    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(ptr.UseCount(), 0);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(*ptr2, 10);
    EXPECT_EQ(ptr2.Get(), a);
}

TEST(TestSharedPtr, CopyAssignment) {
    SharedPtr<int> ptr(new int(5));
    SharedPtr<int> ptr2(new int(100));

    ptr2 = ptr;

    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr2.UseCount(), 2);
    EXPECT_EQ(*ptr2, 5);
    EXPECT_EQ(ptr.Get(), ptr2.Get());
}

TEST(TestSharedPtr, SelfCopyAssignment) {
    SharedPtr<int> ptr(new int(10));

    ptr = ptr;

    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, 10);
    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(TestSharedPtr, MoveAssignment) {
    int *a = new int(52);
    SharedPtr<int> ptr(a);
    SharedPtr<int> ptr2(new int(100));

    ptr2 = std::move(ptr);

    EXPECT_FALSE(ptr);
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(ptr2.Get(), a);
    EXPECT_EQ(*ptr2, 52);
}

TEST(TestSharedPtr, SelfMoveAssignment) {
    SharedPtr<int> ptr(new int(10));

    ptr = std::move(ptr);

    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, 10);
    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(TestSharedPtr, CopyEmpty) {
    SharedPtr<int> ptr;
    SharedPtr<int> ptr2(ptr);

    EXPECT_FALSE(ptr);
    EXPECT_FALSE(ptr2);

    EXPECT_EQ(ptr.UseCount(), 0);
    EXPECT_EQ(ptr2.UseCount(), 0);
}

TEST(TestSharedPtr, MoveEmpty) {
    SharedPtr<int> ptr;
    SharedPtr<int> ptr2(std::move(ptr));

    EXPECT_FALSE(ptr);
    EXPECT_FALSE(ptr2);

    EXPECT_EQ(ptr.UseCount(), 0);
    EXPECT_EQ(ptr2.UseCount(), 0);
}

TEST(TestSharedPtr, MultipleOwners) {
    SharedPtr<int> a(new int(10));

    {
        SharedPtr<int> b(a);
        SharedPtr<int> c(a);

        EXPECT_EQ(a.UseCount(), 3);
    }

    EXPECT_EQ(a.UseCount(), 1);
    EXPECT_EQ(*a, 10);
}

TEST(TestSharedPtr, ObjectDeletedOnce) {
    TestObjectDeleteSingle::destructor_count = 0;

    {
        SharedPtr<TestObjectDeleteSingle> ptr(new TestObjectDeleteSingle);
        SharedPtr<TestObjectDeleteSingle> ptr2(ptr);
        SharedPtr<TestObjectDeleteSingle> ptr3(ptr);

        EXPECT_EQ(ptr.UseCount(), 3);
        EXPECT_EQ(TestObjectDeleteSingle::destructor_count, 0);
    }

    EXPECT_EQ(TestObjectDeleteSingle::destructor_count, 1);
}