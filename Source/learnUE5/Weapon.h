#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapon.generated.h"

UENUM()
enum class EWeaponState : uint8 {
	EWS_None UMETA(DisplayName = "Idle"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped")

};

UCLASS()
class LEARNUE5_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	virtual void Tick(float DeltaTime) override;

	void ShowPickWidget(bool isShow);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereEnterOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 BodyIndex,
		bool bFromSweep,
		const FHitResult& HitResult);

	UFUNCTION()
	virtual void OnSphereLeaveOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);


private:
	UPROPERTY(VisibleAnywhere,Category = "Weapon", meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere,Category = "Weapon", meta = (AllowPrivateAccess = true))
	USphereComponent* PickArea;

	UPROPERTY(EditAnywhere, Category = "UI")
	UWidgetComponent* PickWidget;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	EWeaponState WeaponState;
};
