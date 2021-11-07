// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class LUNARIA_API Printer
{
public:
	Printer();
	~Printer();
};

void Print(const FString& msg, FColor col = FColor::White, float Duration = 5.0f);
void PrintFloat(float msg, const FString& header = "", FColor col = FColor::White, float Duration = 5.0f);