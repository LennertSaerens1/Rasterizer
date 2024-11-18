#include <gtest/gtest.h>
#include "../src/Vector3.h"

TEST(Vector3, DotProduct) {
	dae::Vector3 v1(1.0f, 2.0f, 3.0f);
	dae::Vector3 v2(4.0f, 5.0f, 6.0f);

	EXPECT_EQ(32.0f, dae::Vector3::Dot(v1, v2));
}

TEST(Vector3, CrossProduct) {
	dae::Vector3 v1(1.0f, 2.0f, 3.0f);
	dae::Vector3 v2(4.0f, 5.0f, 6.0f);

	EXPECT_EQ(dae::Vector3(-3.0f, 6.0f, -3.0f), dae::Vector3::Cross(v1, v2));
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
