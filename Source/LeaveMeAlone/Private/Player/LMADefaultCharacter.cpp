// Created by Kirobas inc. All Rights Reserved.


#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CursorMaterial))
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
	
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC))
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
	
	StaminaLogic();

}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ALMADefaultCharacter::CameraZoomIn);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ALMADefaultCharacter::CameraZoomOut);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::Sprinting);

}

void ALMADefaultCharacter::MoveForward(float Value) {
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value) {
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::CameraZoomIn() {
	if (ArmLength > ArmLengthMin)
	{
		ArmLength -= ArmLengthChange;
		SpringArmComponent->TargetArmLength = ArmLength;
	}
}

void ALMADefaultCharacter::CameraZoomOut() {
	if (ArmLength < ArmLengthMax)
	{
		ArmLength += ArmLengthChange;
		SpringArmComponent->TargetArmLength = ArmLength;
	}
}

void ALMADefaultCharacter::Sprinting() {
	if (CanSprint && !Sprinted && (FVector::DotProduct(ALMADefaultCharacter::GetVelocity(), ALMADefaultCharacter::GetActorRotation().Vector()) > 0))
	{
		//MaxWalkSpeed = 600.0f;
		GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
		Sprinted = true;
	}
	else
	{
		//MaxWalkSpeed = 300.0f;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		Sprinted = false;
	}
}

void ALMADefaultCharacter::StaminaLogic() {
	
	if (Stamina < 100 && !Sprinted)
	{
		Stamina += 0.3;
	}
	if (Sprinted)
	{
		if (Stamina > 0)
		{
			Stamina = Stamina - 0.5;
		}
		else
		{
			CanSprint = false;
			Sprinted = false;
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		}
	}

	if (Stamina > 5)
	{
		CanSprint = true;
	}

}

