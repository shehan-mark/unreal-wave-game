// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Button.h"

#include "GameOverView.h"
#include "MasterView.h"

void UGameOverView::NativeConstruct()
{
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddUniqueDynamic(this, &UGameOverView::OnMainMenuPressed);
	}
}

void UGameOverView::OnMainMenuPressed()
{
	ParentViewRef->OnMainMenu.Broadcast();
}