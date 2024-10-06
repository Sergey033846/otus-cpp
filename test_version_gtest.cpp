#include <gtest/gtest.h>
#include "lib.h"

TEST(Version, Pozitive) {
	ASSERT_GT(version(), 0);
}
