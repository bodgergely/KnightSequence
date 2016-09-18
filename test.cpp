#include "knight.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(KnightSequenceCount, TestervsNonTester)
{
	// up to 12 steps as above that the the tester class struggles
	for(int steps=2;steps<12;steps++)
	{
		CompositeGenerator k1(steps, 2);
		CompositeGenerator k2(steps, 2, true);
		auto c1 = k1.count();
		auto c2 = k2.count();
		ASSERT_EQ(c1,c2);
	}
}



