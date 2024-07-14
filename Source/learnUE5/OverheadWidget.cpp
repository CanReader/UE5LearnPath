#include "OverheadWidget.h"

void UOverheadWidget::ShowNetRole(APawn* pawn) {
	ENetRole role = pawn->GetLocalRole();
	FString roleText;
	
	switch (role) {
		case ENetRole::ROLE_Authority:
			roleText = FString("Authoritive");
		break;

		case ENetRole::ROLE_SimulatedProxy:
			roleText = FString("Simulated Proxy");
			break;

		case ENetRole::ROLE_AutonomousProxy:
			roleText = FString("Autonomous Proxy");
			break;

		default:
			roleText = FString("Unknown network role");
	}

	if (NetRoleText)
		NetRoleText->SetText(FText::FromString(FString::Printf(TEXT("Local Role: %s"), *roleText)));
}

void UOverheadWidget::NativeDestruct() 
{
	RemoveFromParent();
	Super::NativeDestruct();
}