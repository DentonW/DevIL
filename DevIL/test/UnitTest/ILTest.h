//ILTest.h
#ifndef ILTEST_H
#define ILTEST_H

#include <cppunit/extensions/HelperMacros.h>

class ILTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ILTest );
  CPPUNIT_TEST( TestilActiveImage );
  CPPUNIT_TEST( TestilSaveF );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void TestilActiveImage();
  void TestilSaveF();
};

#endif  // ILTEST_H
