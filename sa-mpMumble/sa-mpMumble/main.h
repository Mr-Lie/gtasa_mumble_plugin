#pragma once

#include "mumble_plugin_win32.h"

#include <windows.h>
#include <assert.h>


#pragma pack(1)
typedef struct _C_VECTOR2 { // compressed velocity vector
	short X,Y,Z;
} C_VECTOR2;

#pragma pack(1)
typedef struct _C_VECTOR1 { // compressed normalised rotation vector
	short X,Y,Z;
} C_VECTOR1;

#pragma pack(1)
typedef struct _RGBA {
	unsigned char r,g,b,a;
} RGBA, *PRGBA;

#pragma pack(1)
typedef struct _VECTOR {
	float X,Y,Z;
} VECTOR, *PVECTOR;

#pragma pack(1)
typedef struct _VECTOR2D {
	float X,Y;
} VECTOR2D, *PVECTOR2D;

#pragma pack(1)
typedef struct _MATRIX4X4 {
	VECTOR right;
	DWORD  flags;
	VECTOR up;
	float  pad_u;
	VECTOR at;
	float  pad_a;
	VECTOR pos;
	float  pad_p;
} MATRIX4X4, *PMATRIX4X4;

//-----------------------------------------------------------

#define PADDING(x,y) BYTE x[y]

//-----------------------------------------------------------

#define IN_VEHICLE(x) ((x->dwStateFlags & 256) >> 8)

//-----------------------------------------------------------
#pragma pack(1)
typedef struct _ENTITY_TYPE
{
	// ENTITY STUFF
	DWORD vtable; // 0-4
	PADDING(_pad0,12); // 4-16
	FLOAT fRotZBeforeMat; // 16-20 (likely contains the rotation of the entity when mat==0);
	MATRIX4X4 *mat; // 20-24
	DWORD *pdwRenderWare; // 24-28
	DWORD dwProcessingFlags; // 28-32
	PADDING(_pad1,2); // 32-34
	WORD nModelIndex; // 34-36
	PADDING(_pad2,18); // 36-54
	BYTE nControlFlags; // 54-55
	PADDING(_pad3,11); // 55-66
	BYTE byteImmunities; // 66-67
	BYTE byteUnkEntFlags; // 67-68
	VECTOR vecMoveSpeed; // 68-80
	VECTOR vecTurnSpeed; // 80-92
	PADDING(_pad5,72); // 92-164
	BYTE byteAudio[5]; // 164-169
	PADDING(_pad5a,11); // 169-180
	DWORD dwUnkModelRel; // 180-184
} ENTITY_TYPE;

//-----------------------------------------------------------
#pragma pack(1)
typedef struct _WEAPON_SLOT_TYPE
{
	DWORD dwType;
	DWORD dwState;
	DWORD dwAmmoInClip;
	DWORD dwAmmo;
	PADDING(_pwep1,12);
} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytes

//-----------------------------------------------------------


#pragma pack(1)
typedef struct _PED_TASKS_TYPE
{
	DWORD * pdwPed;
	// Basic Tasks
	DWORD * pdwDamage;
	DWORD * pdwFallEnterExit;
	DWORD * pdwSwimWasted;
	DWORD * pdwJumpJetPack;
	DWORD * pdwAction;
	// Extended Tasks
	DWORD * pdwFighting;
	DWORD * pdwCrouching;
	DWORD * pdwSay;
	DWORD * pdwFacialComplex;
	DWORD * pdwPartialAnim;
	DWORD * pdwIK;
} PED_TASKS_TYPE;

//-----------------------------------------------------------
#pragma pack(1)
typedef struct _PED_TYPE
{
	ENTITY_TYPE entity; // 0-184

	// CPED STUFF
	
	PADDING(_pad100,948); // 184-1132
	DWORD dwStateFlags; // 1132-1136
	DWORD dwInvulFlags; // 1136-1140		0x1000 = can_decap
	DWORD dwUnkFlags; // 1140-1444
	PADDING(_pad104,4); // 1144-1148
	PED_TASKS_TYPE *Tasks; // 1148-1152
	DWORD dwPlayerInfoOffset; // 1152-1156
	PADDING(_pad200,124); // 1156-1280
	DWORD dwActiveVision; // 1280-1284
	PADDING(_pad133,16); // 1284-1300
	float fAimZ; // 1300-1304
	PADDING(_pad201,16); // 1304-1320
	BYTE byteAimAnimState; // 1320-1321
	PADDING(_pad202,7); // 1321-1328
	DWORD dwAction; // 1328-1332
	PADDING(_pad203,12); // 1332-1344
	float fHealth;		 // 1344-1348
	float fMaxHealth;	// 1348-1352
	float fArmour;		// 1352-1356
	PADDING(_pad250,4); // 1356-1360
	float fMoveRot1;	// 1360
	float fMoveRot2;	// 1364
	float fRotation1;	// 1368-1372
	float fRotation2;	// 1372-1376
	float fUnkRotRel;
	float fRotCamAdjust;
	DWORD pContactVehicle; // 1384 - 1388
	PADDING(_pad292, 24);
	DWORD pContactEntity; // 1412 - 1416
	PADDING(_pad224, 4);
	DWORD pVehicle;	// 1420-1424
	PADDING(_pad261,8); // 1424-1432
	DWORD dwPedType; // 1432-1436
	DWORD dwUnk1;	 // 1436-1440
	WEAPON_SLOT_TYPE WeaponSlots[13]; // 1440-1804
	PADDING(_pad270,12); // 1804-1816
	BYTE byteCurWeaponSlot; // 1816-1817
	PADDING(_pad280,20); // 1817-1837
	BYTE byteFightingStyle; // 1837-1838
	BYTE byteFightingType; // 1838-1839
	BYTE _pad282;			// 1839-1840
	DWORD pFireObject;	 // 1840-1844
	PADDING(_pad281,44); // 1844-1888
	DWORD  dwWeaponUsed; // 1888-1892
	PDWORD pdwDamageEntity; // 1892-1896
	PADDING(_pad290,52); // 1896-1948
	DWORD pTarget; // 1948
	
} PED_TYPE;

//-----------------------------------------------------------


PED_TYPE * __stdcall GamePool_FindPlayerPed()
{
	return *(PED_TYPE **)(0xB7CD98);
}