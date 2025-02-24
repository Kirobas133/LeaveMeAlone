// Created by Kirobas inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;
class ULMAWeaponComponent;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }  

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnDeathSignature OnDeathSignature;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float ArmLengthMin = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float ArmLengthMax = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float ArmLengthChange = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float Stamina = 200.0f;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Components")
	float MaxWalkSpeed = 300.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Components")
	bool Sprinted = false;
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Components")
	bool CanSprint = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0f;
	
	


	void MoveForward(float Value);
	void MoveRight(float Value);
	void CameraZoomIn();
	void CameraZoomOut();
	void Sprinting();
	void StaminaLogic();

	void OnDeath();
	void OnHealthChanged(float NewHealth);

	void RotationPlayerOnCursor();
};
