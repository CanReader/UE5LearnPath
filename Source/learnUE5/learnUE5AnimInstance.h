// All copyrights are reserved by Canberk Pitirli

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "learnUE5AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEARNUE5_API UlearnUE5AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float deltatime) override;

private:
	UPROPERTY(BlueprintReadOnly,Category = Player ,meta = (AllowPrivateAccess = "true"))
	class AlearnUE5Character* character;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float speed;

	UPROPERTY(BlueprintReadOnly,Category = Movement,meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccellerating;
	
};
