#pragma once
// Class used to manage Index Sprites in Texture

#define MAX_INDEX 305

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
#define NUM_FRAMES_SAMUS_BOMB 2
#define NUM_FRAMES_EXPLOSION 2
#define NUM_FRAMES_BOSS_KRAID 2
#define NUM_FRAMES_GATE_ANIM 5
#define NUM_FRAMES_MOTHER 4



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

	const int ripperYellow = 31 ;
	const int ripperBrown = 32;
	const int ripperRed = 33;
	const int ripperBlue[1] = { 35 };

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


	const int bonusHealth[2] = { 92,93 };
	const int rocket[2] = { 95,96 };
	const int rocketYellowR = 260;
	const int rocketYellowUp = 261;
	const int rocketPinkR = 262;
	const int rocketPinkUp = 263;


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
	const int samusYellowBombEffect[2] = { 167,168 };
	const int samusYellowExplosion[2] = { 170,171 };

	const int samusYellowIceTurnRight = 230;
	const int samusYellowIceTurnUp = 229;
	const int samusYellowIceRunningRight[3] = { 227,228,226 };
	const int samusYellowIceJumpRight = 225;
	const int samusYellowIceJumpingRight[4] = { 224,223,222,220 };
	const int samusYellowIceRollingRight[4] = { 217,218,219,221 };
	const int samusYellowIceHittingRight = 216;
	const int samusYellowIceHittingUp = 211;
	const int samusYellowIceHittingAndRunningRight[3] = { 214,215,213 };
	const int samusYellowIceHittingAndJumpRight = 212;
	const int samusYellowIceRunningUpRight[3] = { 209,210,208 };
	const int samusYellowIceJumpUp = 207;
	const int samusYellowIceHittingJumpUp = 206;



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
	const int samusPinkRunningUpRight[3] = { 159,158,160 };
	const int samusPinkJumpUp = 161;
	const int samusPinkHittingJumpUp = 162;
	const int samusPinkBulletNormal = 172;
	const int samusPinkBulletNormalColliding = 173;
	const int samusPinkBulletIce = 174;
	const int samusPinkBulletIceColliding = 175;
	const int samusPinkBombEffect[2] = { 176,177 };
	const int samusPinkExplosion[2] = { 179,180 };

	const int samusPinkIceTurnRight = 255;
	const int samusPinkIceTurnUp = 254;
	const int samusPinkIceRunningRight[3] = { 252,253,251 };
	const int samusPinkIceJumpRight = 249;
	const int samusPinkIceJumpingRight[4] = { 250,248,247,246 };
	const int samusPinkIceRollingRight[4] = { 242,243,244,245 };
	const int samusPinkIceHittingRight = 241;
	const int samusPinkIceHittingUp = 236;
	const int samusPinkIceHittingAndRunningRight[3] = { 239,240,238 };
	const int samusPinkIceHittingAndJumpRight = 237;
	const int samusPinkIceRunningUpRight[3] = { 234,235,233 };
	const int samusPinkIceJumpUp = 232;
	const int samusPinkIceHittingJumpUp = 231;


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
	const int motherBrain[8] = { 264,265,266,267,268,269,270,271 };
	const int motherBrainOpening[8] = { 272,273,274,275,276,277,278,279 };
	const int motherBrainOpened[2] = { 280,281 };
	const int bulletMotherBrain[4] = { 256,257,258,259 };

	const int canonUp = 282;
	const int canonRightUp = 283;
	const int canonRight = 284;
	const int canonRightDown = 285;
	const int canonDown = 286;
	const int canonLeftDown = 287;
	const int canonLeft = 288;
	const int canonLeftUp = 289;

	const int canonUNKNOW1 = 290;
	const int canonUNKNOW2 = 291;

	const int canonBulletUp = 292;
	const int canonBulletRightUp = 293;
	const int canonBulletRight = 294;
	const int canonBulletRightDown = 295;
	const int canonBulletDown = 296;
	const int canonBulletLeftDown = 297;
	const int canonBulletLeft = 298;
	const int canonBulletLeftUp = 299;

	const int canonBulletUNKNOW1 = 300;
	const int canonBulletUNKNOW2 = 301;

	const int wallFrontMotherBrain[4] = { 302,303,304,305 };



	

	const int enIcon = 201;
	const int liveIcon = 200;
};