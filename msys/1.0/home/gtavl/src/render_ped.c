#include <injector.h>
#include <CVehicle.h>
#include <CPad.h>
#include <CFont.h>
#include <CRadar.h>
#include <CPool.h>
#include <CRGBA.h>
#include <string.h>
#include <common.h>
#include <CTxdStore.h>
#include <stdio.h>
#include <math.h>
#include <bit.h>

#include <CCam.h>

#include "model_ids.h"

#include "hooks.h"


static void *splate = NULL;
static RpClump *cplate = NULL;
static RpAtomic *aplate = NULL;
static RwFrame *fplate = NULL;
static RwTexture *tplate = NULL;

//Events_initGameEvent.before += []
//{
//	memset(helmets, 0, sizeof(helmets));
//	int txdSlot = CTxdStore_AddTxdSlot("helmetmod");
//	CTxdStore_LoadTxd(txdSlot, "helmets/helmetmod.txd");
//	CTxdStore_AddRef(txdSlot);
//	CTxdStore_SetCurrentTxd(txdSlot);
//	for (unsigned int i = 0; i < GetHelmetCfgVector().size(); i++) {
//		StoreHelmet(GetHelmetCfgVector()[i].HelmetId, GetHelmetCfgVector()[i].DFFId, txdSlot);
//	}
//	CTxdStore_PopCurrentTxd();
//};

//Events_shutdownPoolsEvent += []() { // during CGame_Shutdown(), if not, it crashes
//	for (unsigned int i = 0; i < GetHelmetCfgVector().size(); i++) {
//		if (helmets[i]) RpClumpDestroy(helmets[i]);
//	}
//};

void CPed_Render(uint32_t ped);

static void StoreHelmet(int dff_id, int txd_id)
{
	char txd_name[32], dff_name[32];
	sprintf(dff_name, "models/helmetmod_%d.dff", dff_id);
	sprintf(txd_name, "helmetmod_%d", txd_id);
    int txdSlot = CTxdStore_FindTxdSlot("helmetmod");
    if ( txdSlot == -1 )
        txdSlot = CTxdStore_AddTxdSlot("helmetmod");
	CTxdStore_LoadTxd(txdSlot, "models/helmetmod.txd");
	CTxdStore_AddRef(txdSlot);
	CTxdStore_SetCurrentTxd(txdSlot);

	splate = RwStreamOpen(2, 1, dff_name);
	if (splate)
	{
		if (RwStreamFindChunk(splate, 0x10, 0, 0))
		{
			cplate = RpClumpStreamRead(splate);
			if (cplate)
			{
				aplate = ClumpGetFirstAtomic(cplate);
				aplate->renderCallBack = (void*)&AtomicDefaultRenderCallBack;
				if (aplate)
				{   
					tplate = RwTextureRead(txd_name, 0);
					if (tplate)
					{
						tplate->filterAddressing = 2;
						aplate->geometry->matList.materials[0]->texture = tplate;
					}
				}
			}
		}
	}

	RwStreamClose(splate, 0);

    CTxdStore_PopCurrentTxd();
	CTxdStore_RemoveTxd(txdSlot);
}


static bool hasHelmetLoaded = false;

typedef struct {
    unsigned int bIsStanding : 1; // is ped standing on something
    unsigned int bWasStanding : 1; // was ped standing on something
    unsigned int bIsLooking : 1; // is ped looking at something or in a direction
    unsigned int bIsRestoringLook : 1; // is ped restoring head postion from a look
    unsigned int bIsAimingGun : 1; // is ped aiming gun
    unsigned int bIsRestoringGun : 1; // is ped moving gun back to default posn
    unsigned int bCanPointGunAtTarget : 1; // can ped point gun at target
    unsigned int bIsTalking : 1; // is ped talking(see Chat())
    unsigned int bInVehicle : 1; // is in a vehicle
    unsigned int bIsInTheAir : 1; // is in the air
    unsigned int bIsLanding : 1; // is landing after being in the air
    unsigned int bHitSomethingLastFrame : 1; // has been in a collision last fram
    unsigned int bIsNearCar : 1; // has been in a collision last fram
    unsigned int bRenderPedInCar : 1; // has been in a collision last fram
    unsigned int bUpdateAnimHeading : 1; // update ped heading due to heading change during anim sequence
    unsigned int bRemoveHead : 1; // waiting on AntiSpazTimer to remove head
    unsigned int bFiringWeapon : 1; // is pulling trigger
    unsigned int bHasACamera : 1; // does ped possess a camera to document accidents
    unsigned int bPedIsBleeding : 1; // Ped loses a lot of blood if true
    unsigned int bStopAndShoot : 1; // Ped cannot reach target to attack with fist, need to use gun
    unsigned int bIsPedDieAnimPlaying : 1; // is ped die animation finished so can dead now
    unsigned int bStayInSamePlace : 1; // when set, ped stays put
    unsigned int bKindaStayInSamePlace : 1; // when set, ped doesn't seek out opponent or cover large distances. Will still shuffle and look for cover
    unsigned int bBeingChasedByPolice : 1; // use nodes for routefind
    unsigned int bNotAllowedToDuck : 1; // Is this ped allowed to duck at all?
    unsigned int bCrouchWhenShooting : 1; // duck behind cars etc
    unsigned int bIsDucking : 1; // duck behind cars etc
    unsigned int bGetUpAnimStarted : 1; // don't want to play getup anim if under something
    unsigned int bDoBloodyFootprints : 1; // unsigned int bIsLeader :1;
    unsigned int bDontDragMeOutCar : 1;
    unsigned int bStillOnValidPoly : 1; // set if the polygon the ped is on is still valid for collision
    unsigned int bAllowMedicsToReviveMe : 1;
    unsigned int bResetWalkAnims : 1;
    unsigned int bOnBoat : 1; // flee but only using nodes
    unsigned int bBusJacked : 1; // flee but only using nodes
    unsigned int bFadeOut : 1; // set if you want ped to fade out
    unsigned int bKnockedUpIntoAir : 1; // has ped been knocked up into the air by a car collision
    unsigned int bHitSteepSlope : 1; // has ped collided/is standing on a steep slope (surface type)
    unsigned int bCullExtraFarAway : 1; // special ped only gets culled if it's extra far away (for roadblocks)
    unsigned int bTryingToReachDryLand : 1; // has ped just exited boat and trying to get to dry land
    unsigned int bCollidedWithMyVehicle : 1;
    unsigned int bRichFromMugging : 1; // ped has lots of cash cause they've been mugging people
    unsigned int bChrisCriminal : 1; // Is a criminal as killed during Chris' police mission (should be counted as such)
    unsigned int bShakeFist : 1; // test shake hand at look entity
    unsigned int bNoCriticalHits : 1; // ped cannot be killed by a single bullet
    unsigned int bHasAlreadyBeenRecorded : 1; // Used for replays
    unsigned int bUpdateMatricesRequired : 1; // if PedIK has altered bones so matrices need updated this frame
    unsigned int bFleeWhenStanding : 1; //
    unsigned int bMiamiViceCop : 1;  //
    unsigned int bMoneyHasBeenGivenByScript : 1; //
    unsigned int bHasBeenPhotographed : 1;  //
    unsigned int bIsDrowning : 1;
    unsigned int bDrownsInWater : 1;
    unsigned int bHeadStuckInCollision : 1;
    unsigned int bDeadPedInFrontOfCar : 1;
    unsigned int bStayInCarOnJack : 1;
    unsigned int bDontFight : 1;
    unsigned int bDoomAim : 1;
    unsigned int bCanBeShotInVehicle : 1;
    unsigned int bPushedAlongByCar : 1; // ped is getting pushed along by car collision (so don't take damage from horz velocity)
    unsigned int bNeverEverTargetThisPed : 1;
    unsigned int bThisPedIsATargetPriority : 1;
    unsigned int bCrouchWhenScared : 1;
    unsigned int bKnockedOffBike : 1;
    unsigned int bDonePositionOutOfCollision : 1;
    unsigned int bDontRender : 1;
    unsigned int bHasBeenAddedToPopulation : 1;
    unsigned int bHasJustLeftCar : 1;
    unsigned int bIsInDisguise : 1;
    unsigned int bDoesntListenToPlayerGroupCommands : 1;
    unsigned int bIsBeingArrested : 1;
    unsigned int bHasJustSoughtCover : 1;
    unsigned int bKilledByStealth : 1;
    unsigned int bDoesntDropWeaponsWhenDead : 1;
    unsigned int bCalledPreRender : 1;
    unsigned int bBloodPuddleCreated : 1; // Has a static puddle of blood been created yet
    unsigned int bPartOfAttackWave : 1;
    unsigned int bClearRadarBlipOnDeath : 1;
    unsigned int bNeverLeavesGroup : 1; // flag that we want to test 3 extra spheres on col model
    unsigned int bTestForBlockedPositions : 1; // this sets these indicator flags for various posisions on the front of the ped
    unsigned int bRightArmBlocked : 1;
    unsigned int bLeftArmBlocked : 1;
    unsigned int bDuckRightArmBlocked : 1;
    unsigned int bMidriffBlockedForJump : 1;
    unsigned int bFallenDown : 1;
    unsigned int bUseAttractorInstantly : 1;
    unsigned int bDontAcceptIKLookAts : 1;
    unsigned int bHasAScriptBrain : 1;
    unsigned int bWaitingForScriptBrainToLoad : 1;
    unsigned int bHasGroupDriveTask : 1;
    unsigned int bCanExitCar : 1;
    unsigned int CantBeKnockedOffBike : 2; // 0=Default(harder for mission peds) 1=Never 2=Always normal(also for mission peds)
    unsigned int bHasBeenRendered : 1;
    unsigned int bIsCached : 1;
    unsigned int bPushOtherPeds : 1; // GETS RESET EVERY FRAME - SET IN TASK: want to push other peds around (eg. leader of a group or ped trying to get in a car)
    unsigned int bHasBulletProofVest : 1;
    unsigned int bUsingMobilePhone : 1;
    unsigned int bUpperBodyDamageAnimsOnly : 1;
    unsigned int bStuckUnderCar : 1;
    unsigned int bKeepTasksAfterCleanUp : 1; // If true ped will carry on with task even after cleanup
    unsigned int bIsDyingStuck : 1;
    unsigned int bIgnoreHeightCheckOnGotoPointTask : 1; // set when walking round buildings, reset when task quits
    unsigned int bForceDieInCar : 1;
    unsigned int bCheckColAboveHead : 1;
    unsigned int bIgnoreWeaponRange : 1;
    unsigned int bDruggedUp : 1;
    unsigned int bWantedByPolice : 1; // if this is set, the cops will always go after this ped when they are doing a KillCriminal task
    unsigned int bSignalAfterKill : 1;
    unsigned int bCanClimbOntoBoat : 1;
    unsigned int bPedHitWallLastFrame : 1; // useful to store this so that AI knows (normal will still be available)
    unsigned int bIgnoreHeightDifferenceFollowingNodes : 1;
    unsigned int bMoveAnimSpeedHasBeenSetByTask : 1;
    unsigned int bGetOutUpsideDownCar : 1;
    unsigned int bJustGotOffTrain : 1;
    unsigned int bDeathPickupsPersist : 1;
    unsigned int bTestForShotInVehicle : 1;
    unsigned int bUsedForReplay : 1; // This ped is controlled by replay and should be removed when replay is done.
} m_nPedFlags;

static void CPed_CustomRender(uint32_t ped) {
    CPed_Render(ped);

    //(*(m_nPedFlags *)(ped + 0x4A4)).bForceDieInCar = true;

	if (IsPlayer(ped)) {
		//*(uint8_t *)(ped + 1940) = 2;
		//(*(m_nPedFlags *)(ped + 0x4A4)).bRemoveHead = true;
		
		if (((*(uint8_t *)(ped + 0x4A5) & 1) && getVehicleSubClass(*(uint32_t *)(ped + 0x5CC)) == VEHICLE_BIKE ))
		{
			if (((CEntity*)(ped))->m_bIsVisible)
			{
    	        if (!hasHelmetLoaded) {
    	            hasHelmetLoaded = true;
					uint16_t veh_id = *(uint16_t*)(*(uint32_t *)(ped + 0x5CC) + 0x22);
					switch (veh_id) {
						case MODEL_PIZZABOY:
						case MODEL_FAGGIO:
						case MODEL_FREEWAY:
							StoreHelmet(3, randint(23, 28));
							break;
						case MODEL_COPBIKE:
							StoreHelmet(4, randint(28, 30));
							break;
						case MODEL_SANCHEZ:
							StoreHelmet(2, randint(10, 23));
							break;
						default:
							StoreHelmet(1, randint(1, 10));
							break;
					}
    	        } else {
				    RpClump *clump;
				    float height = 0.03f;
				    float size = 1.0f;
				    float front = 0.0f;

				    clump = cplate;
				    //if (ped->m_nModelIndex == 0 && ped->IsPlayer())
				    //{
				    //	int playerId;
				    //	if (ped->m_nPedType == ePedType_PED_TYPE_PLAYER1) playerId = 0; else playerId = 1;
				    //}

				    RwFrame *frame = (RwFrame*)ClumpGetFirstAtomic(clump)->object.object.parent;

				    RpHAnimHierarchy *hAnimHier = GetAnimHierarchyFromSkinClump(((CEntity*)(ped))->m_pRwClump);

					RwMatrix *boneMatrix = &RpHAnimHierarchyGetMatrixArray(hAnimHier)[RpHAnimIDGetIndex(hAnimHier, 5)];
					RwFrameTransform(frame, boneMatrix, 0);

				    CVector pointsIn = { height, front, 0.0f };

					RwFrameTranslate(frame, &pointsIn, 1);

				    CVector rotationAxis = { 1.0f, 0.0f, 0.0f };
				    RwFrameRotate(frame, &rotationAxis, 90.0f, 1);
				    CVector rotationAxis2 = { 0.0f, 1.0f, 0.0f };
				    RwFrameRotate(frame, &rotationAxis2, 90.0f, 1);

				    RwFrameUpdateObjects(frame);
				    RpClumpRender(clump);
    	        }
			}
		} else if (hasHelmetLoaded) {
			hasHelmetLoaded = false;
			RpClumpDestroy(cplate);
			RwTextureDestroy(tplate);
		}
	}
}

int sub_419FF0(int, int, int);

static int CTaskComplexDieInCar_CreateSubTask(int a1, int a2, int a3) {
    (*(m_nPedFlags *)(a3 + 0x4A4)).bCanBeShotInVehicle = false;

    return sub_419FF0(a1, 214, a3);
}

void CTaskComplexDieInCar_PreparePedVehicleForPedDeath(int, int);

static int CTaskComplexDieInCar_CreateSubTaskPrepare(int a1, int a2, int a3) {
    CTaskComplexDieInCar_PreparePedVehicleForPedDeath(a1, *(int *)(a3 + 1484));
    *(uint32_t *)(a1 + 16) = CTimer_m_snTimeInMilliseconds;
    *(uint32_t *)(a1 + 20) = 2000;
    *(uint8_t *)(a1 + 24) = 1;

    (*(m_nPedFlags *)(a3 + 0x4A4)).bCanBeShotInVehicle = false;

    return sub_419FF0(a1, 214, a3);
}

int CFireManager_StartScriptFire(uint32_t *this, CVector* pos, CEntity* target, float _fUnused, char _nUnused, char nGenerations, int nStrength);

static void FireOnDrivers() {
    uint32_t s3;
    asm("move %0, $s3" : "=r" (s3)); // get from our parent function

    int v1 = *(int *)(s3 + 0x468);
    if ( v1 )
        CFireManager_StartScriptFire(&gFireManager, v1 + 4, v1, 0.8f, 1, 0, 1);

    asm volatile ("lw $v1, 0x5D0(%0)\n" : : "r"(s3));
}

void injectPedRenderPatches() {
    WriteDword(0x65BAA0, CPed_CustomRender);
    WriteDword(0x65BB10, CPed_CustomRender);
    WriteDword(0x65BB80, CPed_CustomRender);
    WriteDword(0x65BBF0, CPed_CustomRender);
    WriteDword(0x65BE00, CPed_CustomRender);

    RedirectCall(0x419DA0, CTaskComplexDieInCar_CreateSubTaskPrepare);
    RedirectCall(0x419DFC, CTaskComplexDieInCar_CreateSubTask);

    RedirectCall(0x145C50, FireOnDrivers);
}
