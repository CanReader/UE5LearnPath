// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Kismet/GameplayStatics.h"
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionDelegates.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <OnlineSessionSettings.h>

#include "learnUE5Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AlearnUE5Character : public ACharacter
{
	GENERATED_BODY()

public:
	AlearnUE5Character();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	UFUNCTION(BlueprintCallable)
	void JoinGameSession();
	
	UFUNCTION(BlueprintCallable)
	void OnSessionCreated(FName SessionName, bool isSuccessful);

	UFUNCTION(BlueprintCallable)
	void OnFindSession(bool bWasSuccessful);

private:
#pragma region Actor components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion

#pragma region Input peocesses
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
#pragma endregion

#pragma region Online processes
	IOnlineSessionPtr SessionPtr;

	TSharedPtr<FOnlineSessionSearch>SessionSearch;

	FOnCreateSessionCompleteDelegate SessionCreateDelegate;
	FOnFindSessionsCompleteDelegate FindSessionDelegate;
#pragma endregion


#pragma region Inlines
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
#pragma endregion

};

