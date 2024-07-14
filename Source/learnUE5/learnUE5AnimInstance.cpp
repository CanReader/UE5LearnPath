// All copyrights are reserved by Canberk Pitirli


#include "learnUE5AnimInstance.h"
#include "learnUE5Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UlearnUE5AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	character = Cast<AlearnUE5Character>(TryGetPawnOwner());
}

void UlearnUE5AnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();
}

void UlearnUE5AnimInstance::NativeUpdateAnimation(float deltatime)
{
	Super::NativeUpdateAnimation(deltatime);
	
	if (character == nullptr)
		character = Cast<AlearnUE5Character>(TryGetPawnOwner());

	if (character) {
	FVector velocity = character->GetVelocity();
	velocity.Z = 0;
	speed = velocity.Size();

	auto moveComp = character->GetMovementComponent();
	bIsInAir = moveComp->IsFalling();

	bIsAccellerating = character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0;
	}
}
