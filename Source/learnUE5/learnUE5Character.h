// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Weapon.h"
#include "CombatComponent.h"
#include "learnUE5Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(CharacterLog, Log, All);

UCLASS(config=Game)
class AlearnUE5Character : public ACharacter
{
	GENERATED_BODY()

public:
	AlearnUE5Character();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float delta) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region Functions

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Use(const FInputActionValue & Value);

	UFUNCTION()
	void OnRep_OverlapWeapon(AWeapon* LastWeapon);

#pragma endregion



#pragma region Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsSprinting;

	UPROPERTY(ReplicatedUsing = OnRep_OverlapWeapon)
	AWeapon* OverlappingWeapon;

#pragma endregion

private:
#pragma region Actor components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* ThirdPersonCameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UserWidget, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HeadWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;
#pragma endregion

#pragma region Input processes
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseAction;
	
#pragma endregion

#pragma region Inlines
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return ThirdPersonCameraArm; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return ThirdPersonCamera; }
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }
	FORCEINLINE void SetOverlapWeapon(AWeapon* weapon) {
		if (OverlappingWeapon)
			OverlappingWeapon->ShowPickWidget(false);

		OverlappingWeapon = weapon;
		if (IsLocallyControlled() && weapon)
			weapon->ShowPickWidget(true);
	}
#pragma endregion

};
