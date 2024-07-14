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
#include "Online/OnlineSessionNames.h"
#include "Net/UnrealNetwork.h"
#include "OverheadWidget.h"

DEFINE_LOG_CATEGORY(CharacterLog);

//////////////////////////////////////////////////////////////////////////
// AlearnUE5Character

AlearnUE5Character::AlearnUE5Character()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(85.0f);

	GetMesh()->SetWorldTransform(
		FTransform(
			FRotator(0, 0, -80.0f),
			FVector(0, 0, -90.0f),
			FVector(0.9f, 0.9f, 0.9f))
	);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -80.0f));
	GetMesh()->SetRelativeRotation(FRotator(0,0,-90));
	GetMesh()->SetWorldScale3D(FVector(0.9,0.9,0.9));

	ThirdPersonCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	ThirdPersonCameraArm->SetupAttachment(GetMesh());
	ThirdPersonCameraArm->TargetArmLength = 150.f;
	ThirdPersonCameraArm->bUsePawnControlRotation = true;
	ThirdPersonCameraArm->SetRelativeLocation(FVector(-55, 66, 166));

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonCameraArm,USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = true;

	HeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Overhead"));
	HeadWidget->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<UOverheadWidget> OverheadClass(TEXT("/Game/Blueprints/UWB_OverheadWidget"));
	if (OverheadClass.Class != NULL)
		HeadWidget->SetWidgetClass(OverheadClass.Class);

	HeadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HeadWidget->SetDrawAtDesiredSize(true);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AlearnUE5Character::BeginPlay()
{
	Super::BeginPlay();
	if (HeadWidget != nullptr && HeadWidget->GetWidget() != nullptr)
	{
		UOverheadWidget* widget = Cast<UOverheadWidget>(HeadWidget->GetUserWidgetObject());
		widget->ShowNetRole(this);
	}
}

void AlearnUE5Character::Tick(float delta)
{
	Super::Tick(delta);
}

void AlearnUE5Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AlearnUE5Character, OverlappingWeapon,COND_OwnerOnly)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AlearnUE5Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APlayerController* controller = Cast<APlayerController>(GetController());

	if(controller)
	{
		auto local = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer());

		local->AddMappingContext(DefaultMappingContext,0);
		
		UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

		if (input)
		{	
			input->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AlearnUE5Character::Move);

			input->BindAction(LookAction, ETriggerEvent::Triggered,this,&AlearnUE5Character::Look);

			input->BindAction(JumpAction,ETriggerEvent::Started,this,&AlearnUE5Character::Jump);

			input->BindAction(JumpAction,ETriggerEvent::Completed,this,&AlearnUE5Character::StopJumping);

			input->BindAction(UseAction, ETriggerEvent::Triggered,this,AlearnUE5Character::UseKey);
		}
		else {
			UE_LOG(LogTemp,Error,TEXT("Failed to initialize enchanced input subsystem! Legacy input system will be used"));

		}
	}
	else {
		UE_LOG(LogTemp,Error,TEXT("Failed to cast controller! The code will not work!"));
	}
}

void AlearnUE5Character::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Yaw(0,Rotation.Yaw,0);

		const FVector ForwardVector = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector,MoveVector.Y);
		AddMovementInput(RightVector,MoveVector.X);
	}
}

void AlearnUE5Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxis.X);
		AddControllerPitchInput(LookAxis.Y);
	}
}

void AlearnUE5Character::UseKey(const FInputActionValue& Value)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1,15,FColor::Blue,FString("Equip!"));
}

void AlearnUE5Character::OnRep_OverlapWeapon(AWeapon* LastWeapon)
{
	if (LastWeapon)
		LastWeapon->ShowPickWidget(false);

	if (OverlappingWeapon)
		OverlappingWeapon->ShowPickWidget(true);
}
