// Created by Kirobas inc. All Rights Reserved.


#include "Weapon/LMABaseWeapon.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

// Sets default values
ALMABaseWeapon::ALMABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);

	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;

}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALMABaseWeapon::Fire() {
	//Shoot();
	GetWorldTimerManager().SetTimer(ShooTimerFrequencyHandle, this, &ALMABaseWeapon::OnTimeToShoot, ShootFrequency, true);
}

void ALMABaseWeapon::StopFire() {
	GetWorldTimerManager().ClearTimer(ShooTimerFrequencyHandle);
}

void ALMABaseWeapon::Shoot() {
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}
	DecrementBullets();
}

void ALMABaseWeapon::ChangeClip() {
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const 
{
	return CurrentAmmoWeapon.Bullets == 0;
}

void ALMABaseWeapon::DecrementBullets() {
	CurrentAmmoWeapon.Bullets--;
	//UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));

	if (IsCurrentClipEmpty())
	{
		OnEmptyClipsSignature.Broadcast();
		//ChangeClip();
	}
}

void ALMABaseWeapon::OnTimeToShoot() {
	Shoot();
}

bool ALMABaseWeapon::IsClipIsFull() {
	return (CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets);
}

FAmmoWeapon ALMABaseWeapon::GetCurrentAmmoWeapon() const {
	return CurrentAmmoWeapon;
}