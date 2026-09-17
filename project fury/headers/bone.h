#pragma once

typedef enum bones : int {
	head = 7,
	neck = 6,
	spine = 4,
	spine_1 = 2,
	left_shoulder = 9,
	left_arm = 10,
	left_hand = 11,
	cock = 1,
	right_shoulder = 13,
	right_arm = 14,
	right_hand = 15,
	left_hip = 17,
	left_knee = 18,
	left_feet = 19,
	right_hip = 20,
	right_knee = 21,
	right_feet = 22
};

typedef struct BoneConnection {
	int bone1, bone2;

	BoneConnection(int b1, int b2) : bone1(b1), bone2(b2) {}
};