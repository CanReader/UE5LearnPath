// All copyrights are reserved by Canberk Pitirli

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEARNUE5_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NetRoleText;

	UFUNCTION(BlueprintCallable)
	void ShowNetRole(APawn* pawn);

protected:
	virtual void NativeDestruct() override;

};
