// Robbie Grier

#include "ActionApplyStatus.h"
#include "Boon.h"
#include "CombatComponent.h"
#include "AttributesComponent.h"

void UActionApplyStatus::PerformApplyStatus(AActor* InAgent, AActor* InSubject, ATool* InTool, TSubclassOf<class ABoon> InStatusClass, float InDuration)
{
	if (InAgent && InSubject)
	{
		auto Action = NewObject<UActionApplyStatus>();
		Action->Agent = InAgent;
		Action->StatusClass = InStatusClass;
		Action->Subject = InSubject;
		Action->Tool = InTool;
		Action->Duration = InDuration;

		if (auto CombatComponent = InAgent->FindComponentByClass<UCombatComponent>())
		{
			CombatComponent->AddAction(Action);
		}
	}
}

void UActionApplyStatus::Execute()
{
	if (Agent)
	{
		if (auto Attributes = Agent->FindComponentByClass<UAttributesComponent>())
		{
			Attributes->StatusEffectAppliedDuration->Set(Duration);
			Duration = Attributes->StatusEffectAppliedDuration->Render(this);
		}
	}

	if (Subject && !Subject->IsActorBeingDestroyed())
	{
		if (auto SubjectAttributes = Subject->FindComponentByClass<UAttributesComponent>())
		{
			SubjectAttributes->AddStatusEffectFromClass(StatusClass, Agent, Duration);
		}
	}
}