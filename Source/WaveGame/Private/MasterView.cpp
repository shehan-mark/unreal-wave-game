// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterView.h"
#include "StartMenu.h"

void UMasterView::NativeConstruct()
{
	if (StartMenu_WBP)
	{
		UStartMenu* StartMenuRef = Cast<UStartMenu>(StartMenu_WBP);
		if (StartMenuRef)
		{
			StartMenuRef->ParentViewRef = this;
		}
	}

	OnStartGame.AddDynamic(this, &UMasterView::HandleStartGame);
	OnQuitGame.AddDynamic(this, &UMasterView::HandleQuitGame);
}

void UMasterView::HandleStartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("UMasterView::HandleStartGame"));
}

void UMasterView::HandleQuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("UMasterView::HandleQuitGame"));

}
