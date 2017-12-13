#pragma once
// Class used to manage Index Sprites in Texture

#define MAX_INDEX 199

// Define NUM_FRAMES
#define NUM_FRAMES_ZOOMER 2
#define NUM_FRAMES_ZEB 2
#define NUM_FRAMES_SKREE 3
#define NUM_FRAMES_RIO 2
#define NUM_FRAMES_MELLOW 2
#define NUM_FRAMES_WAVER 3
#define NUM_FRAMES_GUARDIAN 3
#define NUM_FRAMES_ITEM 4
#define NUM_FRAMES_BONUS 2
#define NUM_FRAMES_SAMUS_START 4
#define NUM_FRAMES_SAMUS_RUNNING 3
#define NUM_FRAMES_SAMUS_ROLLING 4
#define NUM_FRAMES_SAMUS_JUMPING 4
#define NUM_FRAMES_SAMUS_BOMB 3
#define NUM_FRAMES_EXPLOSION 2
#define NUM_FRAMES_BOSS_KRAID 2
#define NUM_FRAMES_GATE_ANIM 5



class IndexManager
{
private:
	static IndexManager* instance;
	IndexManager();
	~IndexManager();

public:
	static IndexManager* getInstance();

	// ================================ Don't change - not unless you can
	const int zoomerRed[2] = { 0,1 };
	const int zoomerYellow[2] = { 48,49 };
	const int zoomerBrown[2] = { 50,51 };
	const int zoomerBlue[2] = { 8,9 };

	const int zebYellow[2] = { 2,3 };
	const int zebBrown[2] = { 4,5 };
	const int zebRed[2] = { 6,7 };
	const int zebBlue[2] = { 37,38 };

	const int skreeYellow[3] = { 10,11,12 };
	const int skreeBrown[3] = { 13,14,15 };
	const int skreeRed[3] = { 16,17,18 };
	const int skreeBlue[3] = { 19,20,21 };
	const int skreeDeathEffect = 22;

	const int rioYellow[2] = { 23,24 };
	const int rioBrown[2] = { 25,26 };
	const int rioRed[2] = { 27,28 };
	const int rioBlue[2] = { 29,30 };

	const int ripperYellow = 31;
	const int ripperBrown = 32;
	const int ripperRed = 33;
	const int ripperBlue = 34;

	const int mellow[2] = { 35,36 };

	const int waverBrown[3] = { 39,40,41 };
	const int waverRed[3] = { 42,43,44 };
	const int waverBlue[3] = { 45,46,47 };


	const int guardianBlue[3] = { 104,103,52 };
	const int guardianOrange[3] = { 102,101,100 };
	const int guardianPurple[3] = { 97,98,99 };


	const int missileRocket[4] = { 53,54,55,56 };
	const int energyTank[4] = { 57,58,59,60 };
	const int maruMari[4] = { 61,62,63,64 };
	const int bomb[4] = { 65,66,67,68 };
	const int longBeam[4] = { 69,70,71,72 };
	const int iceBeam[4] = { 73,74,75,76 };
	const int highJump[4] = { 83,77,78,79 };
	const int varia[4] = { 80,81,82,94 };
	const int waveBeam[4] = { 84,85,86,87 };
	const int scremAttack[4] = { 88,89,90,91 };


	const int en[2] = { 92,93 };
	const int rocket[2] = { 95,96 };


	const int samusYellowStart[4] = { 105,106,107,108 };
	const int samusYellowTurnRight = 109;
	const int samusYellowTurnUp = 110;
	const int samusYellowRunningRight[3] = { 112,111,113 };
	const int samusYellowJumpRight = 114;
	const int samusYellowJumpingRight[4] = { 115,116,117,118 };
	const int samusYellowRollingRight[4] = { 119,120,121,122 };
	const int samusYellowHittingRight = 123;
	const int samusYellowHittingUp = 128;
	const int samusYellowHittingAndRunningRight[3] = { 125,124,126 };
	const int samusYellowHittingAndJumpRight = 127;
	const int samusYellowRunningUpRight[3] = { 130,129,131 };
	const int samusYellowJumpUp = 132;
	const int samusYellowHittingJumpUp = 133;
	const int samusYellowBulletNormal = 163;
	const int samusYellowBulletNormalColliding = 164;
	const int samusYellowBulletIce = 165;
	const int samusYellowBulletIceColliding = 166;
	const int samusYellowBombEffect[3] = { 167,168,169 };
	const int samusYellowExplosion[2] = { 170,171 };



	const int samusPinkStart[4] = { 134,135,136,137 };
	const int samusPinkTurnRight = 138;
	const int samusPinkTurnUp = 139;
	const int samusPinkRunningRight[3] = { 141,140,142 };
	const int samusPinkJumpRight = 143;
	const int samusPinkJumpingRight[4] = { 144,145,146,147 };
	const int samusPinkRollingRight[4] = { 148,149,150,151 };
	const int samusPinkHittingRight = 152;
	const int samusPinkHittingUp = 157;
	const int samusPinkHittingAndRunningRight[3] = { 154,153,155 };
	const int samusPinkHittingAndJumpRight = 156;
	const int samusPinkHittingAndRunningUp[3] = { 159,158,160 };
	const int samusPinkJumpUp = 161;
	const int samusPinkHittingJumpUp = 162;
	const int samusPinkBulletNormal = 172;
	const int samusPinkBulletNormalColliding = 173;
	const int samusPinkBulletIce = 174;
	const int samusPinkBulletIceColliding = 175;
	const int samusPinkBombEffect[3] = { 176,177,178 };
	const int samusPinkExplosion[2] = { 179,180 };


	const int alienSmallR = 181;
	const int alienBigR = 182;
	const int brickBlue = 183;
	const int brickGreen = 184;
	const int brickRed = 192;
	const int brickSecretBlue = 185;
	const int brickSecretGreen = 191;
	const int bossDragonL = 186;
	const int gateBlueR = 187;
	const int gateBlueROpen[5] = { 187, 193, 194, 195, 199 };
	const int gateBlueRClose[5] = { 199, 195, 194, 193, 187 };
	const int gateRedR = 188;
	const int gateRedROpen[5] = { 188, 196, 197, 198, 199 };
	const int gateRedRClose[5] = { 199, 198, 197, 196, 188 };
	const int bossKraidR[2] = { 189,190 };
};