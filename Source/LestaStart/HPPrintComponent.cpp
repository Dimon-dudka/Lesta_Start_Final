#include "HPPrintComponent.h"

UHPPrintComponent::UHPPrintComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TextSize = 25;
	HeighAboveActor = 100;

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRender->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TextRender->SetTextRenderColor(FColor::Black);
	TextRender->SetXScale(1.f);
	TextRender->SetYScale(1.f);
	TextRender->SetWorldSize(TextSize);

	SetupHealthPoints(0);
}

void UHPPrintComponent::SetupHealthPoints(const double& HP) {
	TextRender->SetText(FText::FromString(LexToSanitizedString(((static_cast<int>(HP))/5)*5) + " HP"));
}

void UHPPrintComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UHPPrintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TextRender->SetWorldLocation(GetOwner()->GetActorLocation() + FVector(0.0, 0.0, HeighAboveActor));

	//	Rotation text to Player
	FRotator UserView = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();
	UserView.Yaw += 180;
	TextRender->SetWorldRotation(UserView);
}