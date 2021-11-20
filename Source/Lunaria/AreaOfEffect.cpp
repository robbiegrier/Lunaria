// Robbie Grier

#include "AreaOfEffect.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Printer.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Hittable.h"
#include "Helpers.h"
#include "GameplayEventManager.h"

AAreaOfEffect::AAreaOfEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	RootComponent = SceneRoot;
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag("HitStrategy.AoE"));
}

void AAreaOfEffect::SetColor(const FLinearColor& InColor)
{
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(InColor.R, InColor.G, InColor.B));
}

void AAreaOfEffect::Launch(const FGameplayTagContainer& TagContainer, const TArray<TSubclassOf<ABoon>>& StatusesToApply, float InDamage, float InRadius, float InDelay)
{
	GameplayTags.AppendTags(TagContainer);
	StatusPayload = StatusesToApply;
	DamagePayload = InDamage;
	Radius = InRadius;
	Delay = InDelay;
	FrameRadius = 0.f;
	Mesh->SetWorldScale3D(FVector(Radius / 10.f));
	SetActorTickEnabled(true);
}

void AAreaOfEffect::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void AAreaOfEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FrameRadius < Radius || !FMath::IsNearlyEqual(FrameRadius, Radius))
	{
		auto ChunkElapsed = Delay / DeltaTime;
		auto GrowthRate = Radius / ChunkElapsed;
		FrameRadius += GrowthRate;
		FrameRadius = FMath::Min(FrameRadius, Radius);

		auto ScaledRadius = FrameRadius / (Radius / 10.f);
		Mesh->SetScalarParameterValueOnMaterials(TEXT("Outer Radius"), ScaledRadius);
		Mesh->SetScalarParameterValueOnMaterials(TEXT("Inner Radius"), FMath::Max(ScaledRadius - Thickness, 0.f));
		Mesh->SetScalarParameterValueOnMaterials(TEXT("Inner Opacity"), FMath::Max(0.f, ((FrameRadius * FrameRadius) / (Radius * Radius)) * .1f));
	}
	else
	{
		auto ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

		auto Ignore = TArray<AActor*>();
		Ignore.Add(this);

		auto OutActors = TArray<AActor*>();
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Radius, ObjectTypes, nullptr, Ignore, OutActors);

		for (auto Actor : OutActors)
		{
			if (auto OtherAsHittable = Cast<IHittable>(Actor))
			{
				if (Helpers::AreDifferentTeams(GetOwner(), Actor))
				{
					auto Event = FGameplayEvent(GetOwner(), ENativeEventType::Hit, Actor);
					Event.Values.Add("Damage", DamagePayload);
					Event.Vectors.Add("Location", GetActorLocation());
					Event.EventTags.AppendTags(GameplayTags);
					AGameplayEventManager::Get(GetWorld())->SubmitEvent(Event);

					for (auto Effect : StatusPayload)
					{
						auto StatusEvent = FGameplayEvent(GetOwner(), ENativeEventType::ApplyStatus, Actor);
						StatusEvent.Classes.Add("StatusEffectClass", Effect);
						StatusEvent.Vectors.Add("Location", GetActorLocation());
						StatusEvent.EventTags.AppendTags(GameplayTags);
						AGameplayEventManager::Get(GetWorld())->SubmitEvent(StatusEvent);
					}
				}
			}
		}

		Helpers::SafelyKillMyself(this);
		SetActorTickEnabled(false);
	}
}