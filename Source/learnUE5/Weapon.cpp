#include "Weapon.h"
#include "learnUE5Character.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);

	PickArea = CreateDefaultSubobject<USphereComponent>(TEXT("Area of pick"));
	PickArea->SetupAttachment(RootComponent);
	PickArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickArea->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);

	PickWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickText"));
	PickWidget->SetupAttachment(Mesh);
	PickWidget->SetRelativeLocation(FVector(0,-10,60));
	PickWidget->SetDrawAtDesiredSize(true);
	PickWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Blueprints/UWB_PickWidget"));
	if (WidgetClass.Class)
		PickWidget->SetWidgetClass(WidgetClass.Class);


}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

		PickArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		PickWidget->SetVisibility(false);

		PickArea->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnSphereEnterOverlap);
		PickArea->OnComponentEndOverlap.AddDynamic(this,&AWeapon::OnSphereLeaveOverlap);
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnSphereEnterOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 BodyIndex,
	bool bFromSweep,
	const FHitResult& HitResult) {

	auto character = Cast<AlearnUE5Character>(OtherActor);

	if (character) 
		character->SetOverlapWeapon(this);
}

void AWeapon::OnSphereLeaveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto character = Cast<AlearnUE5Character>(OtherActor);
	if (character)
		character->SetOverlapWeapon(nullptr);
}

void AWeapon::ShowPickWidget(bool isShow)
{
	if (PickWidget)
		PickWidget->SetVisibility(isShow);
}
