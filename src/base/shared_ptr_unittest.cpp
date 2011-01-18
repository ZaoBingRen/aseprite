// ASE base library
// Copyright (C) 2001-2011  David Capello
//
// This source file is ditributed under a BSD-like license, please
// read LICENSE.txt for more information.

#include <gtest/gtest.h>

#include "base/shared_ptr.h"

TEST(SharedPtr, IntPtr)
{
  SharedPtr<int> a(new int(5));
  EXPECT_EQ(5, *a);
}

TEST(SharedPtr, RefCount)
{
  SharedPtr<int> a(new int(5));
  EXPECT_EQ(1, a.getRefCount());
  a.reset();
  EXPECT_EQ(0, a.getRefCount());

  SharedPtr<int> b(new int(5));
  {
    SharedPtr<int> c(b);
    EXPECT_EQ(2, b.getRefCount());
    EXPECT_EQ(2, c.getRefCount());
    a = c;
    EXPECT_EQ(3, a.getRefCount());
    EXPECT_EQ(3, b.getRefCount());
    EXPECT_EQ(3, c.getRefCount());
    a.reset();
    EXPECT_EQ(2, b.getRefCount());
    EXPECT_EQ(2, c.getRefCount());
  }
  EXPECT_EQ(1, b.getRefCount());
}


class DeleteIsCalled
{
public:
  DeleteIsCalled(bool& flag) : m_flag(flag) { }
  ~DeleteIsCalled() { m_flag = true; }
private:
  bool& m_flag;
};

TEST(SharedPtr, DeleteIsCalled)
{
  bool flag = false;
  {
    SharedPtr<DeleteIsCalled> a(new DeleteIsCalled(flag));
  }
  EXPECT_EQ(true, flag);
}


class A { };
class B : public A { };

TEST(SharedPtr, Hierarchy)
{
  SharedPtr<A> a(new B);
  SharedPtr<B> b = a;
  SharedPtr<A> c = a;
  SharedPtr<A> d = b;
  EXPECT_EQ(4, a.getRefCount());
}


TEST(SharedPtr, Compare)
{
  SharedPtr<int> a(new int(0));
  SharedPtr<int> b(a);
  SharedPtr<int> c(new int(0));

  // Compare pointers
  EXPECT_TRUE(a == b);
  EXPECT_TRUE(a != c);
  EXPECT_TRUE(b != c);

  // Compare pointers
  a = c;
  c = b;
  EXPECT_TRUE(a != b);
  EXPECT_TRUE(a != c);
  EXPECT_TRUE(b == c);

  // Compare values
  EXPECT_TRUE(*a == *b);
  EXPECT_TRUE(*a == *c);
  EXPECT_TRUE(*b == *c);

  // Change values
  *a = 2;
  *b = 5;
  EXPECT_EQ(2, *a);
  EXPECT_EQ(5, *b);
  EXPECT_EQ(5, *c);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
