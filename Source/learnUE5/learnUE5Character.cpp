// Copyright Epic Games, Inc. All Rights Reserved.

#include "learnUE5Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Online/OnlineSessionNames.h>


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AlearnUE5Character

AlearnUE5Character::AlearnUE5Character()
{
	SessionCreateDelegate = 
		FOnCreateSessionCompleteDelegate::
		CreateUObject(this,&AlearnUE5Character::OnSessionCreated);

	FindSessionDelegate = 
		FOnFindSessionsCompleteDelegate::
		CreateUObject(this,&AlearnUE5Character::OnFindSession);

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	IOnlineSubsystem* osb = IOnlineSubsystem::Get();

	if (osb)
	{
		SessionPtr = osb->GetSessionInterface();

		GEngine->AddOnScreenDebugMessage(
			-1,
			30.0f,
			FColor::Red,
			FString::Printf(TEXT("Found subsystem: %s"),
				*osb->GetSubsystemName().ToString()));
	}

}

void AlearnUE5Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AlearnUE5Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AlearnUE5Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AlearnUE5Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AlearnUE5Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AlearnUE5Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AlearnUE5Character::CreateGameSession()
{
	if (SessionPtr.IsValid()) {
		FNamedOnlineSession* session = SessionPtr->GetNamedSession(NAME_GameSession);
		if (session != nullptr)
			SessionPtr->DestroySession(NAME_GameSession);

		TSharedPtr<FOnlineSessionSettings> sesSettings = MakeShareable(new FOnlineSessionSettings());
		//Set settings
		sesSettings->bIsLANMatch = false;
		sesSettings->NumPublicConnections = 4;
		sesSettings->bAllowJoinInProgress = true;
		sesSettings->bAllowJoinViaPresence = true;
		sesSettings->bShouldAdvertise = true;
		sesSettings->bUsesPresence = true;
		sesSettings->bUseLobbiesIfAvailable = true;

		SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(SessionCreateDelegate);

		const ULocalPlayer* local = GetWorld()->GetFirstLocalPlayerFromController();
		SessionPtr->CreateSession(*local->GetPreferredUniqueNetId(), NAME_GameSession, *sesSettings);
	}
}

void AlearnUE5Character::JoinGameSession()
{
		if (SessionPtr.IsValid()){
			SessionPtr->AddOnFindSessionsCompleteDelegate_Handle(FindSessionDelegate);

			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->MaxSearchResults = 10000;
			SessionSearch->bIsLanQuery = false;
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);

			ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
			SessionPtr->FindSessions(*player->GetPreferredUniqueNetId(),SessionSearch.ToSharedRef());
	}
}

void AlearnUE5Character::OnSessionCreated(FName SessionName, bool isSuccessful)
{
	if (isSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1,
			30.0f,
			FColor::Blue,
			FString::Printf(TEXT("Created session: %s"), *SessionName.ToString()));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1,
			30.0f,
			FColor::Red,
			TEXT("Failed to create session!"));
	}
}

void AlearnUE5Character::OnFindSession(bool bWasSuccessful)
{
	if(SessionSearch != nullptr)
	for (auto result : SessionSearch->SearchResults)
	{
		FString Id = result.GetSessionIdStr();
		FString owner = result.Session.OwningUserName;

		GEngine->AddOnScreenDebugMessage(-1,30.0f,FColor::Blue,FString::Printf(TEXT("Found %s, Owner"),*Id,*owner));
	}
}
