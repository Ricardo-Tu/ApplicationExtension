#pragma once
#include <thread>

extern BOOLEAN cardSlotCoolDown[10];

extern PVOID lpkillZombiesFlagAddress;

VOID WINAPI LockSunShine();

VOID WINAPI RecoverLockSunShineShineOriginalCode();

VOID WINAPI ModifySunShine();

VOID WINAPI DecreaseCardCooldown();

VOID WINAPI RecoverCardCooldownSourceCode();

VOID WINAPI NoCardCooldown();

VOID WINAPI CancelNoCardCooldown();

VOID WINAPI AutomaticallyCollectionSunShine();

VOID WINAPI RecoverAutomaticallyCollectionSunShine();

VOID WINAPI Stregthenbullet();

VOID WINAPI RecoverStrengthenbullet();

VOID WINAPI BTBullet();

VOID WINAPI RecoverBTBullet();

VOID WINAPI ModifyCardSlotPlant(ULONG cardslot, ULONG plant);

VOID WINAPI Plant(ULONG row, ULONG column, ULONG plant);

VOID WINAPI PlantOverlapping();

VOID WINAPI RecoverPlantOverlappint();

VOID WINAPI ZombiesPlant(ULONG row, ULONG column, ULONG zombies);

VOID WINAPI PurpleDirectPlant();

VOID WINAPI RecoverPurpleDirectPlant();

VOID WINAPI BackgroundRunGame();

VOID WINAPI RecoverBackgroundRunGame();

VOID WINAPI KillAllZombies();

VOID WINAPI RecoverKillAllZombies();

VOID WINAPI KillOneZombies();

VOID WINAPI RecoverKillOneZombies();

VOID WINAPI RomdomizedBullet();

VOID WINAPI RecoverRomdomizedBullet();

VOID WINAPI ClayPotPerspective();

VOID WINAPI RecoverClayPotPerspective();

VOID WINAPI QuickConveyorBelt();

VOID WINAPI RecoverQucikConveyorBelt();

VOID WINAPI ListPlant();

VOID WINAPI RecoverListPlant();

VOID WINAPI ChangeCornBulletToButter();

VOID WINAPI ChangeCornBulletToBomb();

VOID WINAPI RecoverChangeCornBullet();

VOID WINAPI CleanTombstone();

VOID WINAPI CleanPlants();

VOID WINAPI RomdomPlant();

VOID WINAPI CancelRomdomPlant();

VOID WINAPI BigMouthFlowerCoolDown();

VOID WINAPI RecoverBigMouthFlowerCoolDown();

VOID WINAPI PotatoMineCoolDown();

VOID WINAPI RecoverPotatoMineCoolDown();

VOID WINAPI CornCannonCoolDown();

VOID WINAPI RecoverCornCannonCoolDown();

VOID WINAPI MagneticMushRoomCoolDown();

VOID WINAPI RecoverMagneticMushRoomCoolDown();

VOID WINAPI ClearFog();

VOID WINAPI RecoverClearFog();

VOID WINAPI ClearFog();

VOID WINAPI RecoverClearFog();

VOID WINAPI MushRoomDonotSleep();

VOID WINAPI RecoverMushRoomDonotSleep();

VOID WINAPI SpikeZombies();

VOID WINAPI RecoverSpikeZombies();

VOID WINAPI UndefeatedForever();

VOID WINAPI RecoverUndefeatedForever();

VOID WINAPI PenetrateBullet();

VOID WINAPI RecoverPenetrateBullet();

VOID WINAPI ModifyMoney();

VOID WINAPI BulletTracking();

VOID WINAPI RecoverBulletTracking();

VOID WINAPI StartupFiveCar();

VOID WINAPI RecoverFiveCar();

VOID WINAPI SetFiveCarTimer();

VOID WINAPI CancelFiveCarTimer();

VOID WINAPI StartupSixCar();

VOID WINAPI RecoverSixCar();

VOID WINAPI SetSixCarTimer();

VOID WINAPI CancelSixCarTimer();
