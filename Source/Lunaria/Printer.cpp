// Fill out your copyright notice in the Description page of Project Settings.

#include "Printer.h"
#include "Engine/GameEngine.h"

Printer::Printer()
{
}

Printer::~Printer()
{
}

void Print(const FString& msg, FColor col, float Duration)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, col, msg);

	if (col == FColor::Red)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *msg);
	}
	else if (col == FColor::Yellow)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *msg);
	}
}

void PrintFloat(float msg, const FString& header, FColor col, float Duration)
{
	Print(FString::Printf(TEXT("%F"), msg));
}