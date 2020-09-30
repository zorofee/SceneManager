// Fill out your copyright notice in the Description page of Project Settings.


#include "SWidgetTreasureBox.h"
#include "DelegateManager.h"
#include "SaveDataManager.h"

void SPlanDropList::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("+ 新建")))
			.OnClicked(this,&SPlanDropList::ExecuteOnClick)
			.ContentPadding(4.0f)
		]

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(20, 0, 0, 0)
		[
			SAssignNew(PlanNameText, SEditableTextBox)
			.HintText(FText::FromString(TEXT("输入新的方案名称")))
		]

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(20, 0, 0, 0)
		[
			SNew(SCheckBox)
			.OnCheckStateChanged(this, &SPlanDropList::CheckBoxChanged)
			.IsChecked(ECheckBoxState::Unchecked)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("拷贝当前的方案")))
			]
		]
	];

	OnAddPlan = InArgs._OnAddPlan;
	Type = InArgs._Type;
}

FReply SPlanDropList::ExecuteOnClick()
{
	FString planName = PlanNameText->GetText().ToString();
	if (planName != TEXT(""))
	{
		//不为空时添加成功
		if (Type == EPlanListType::Material) {
			DelegateManager::Get()->AddSceneMatPlan.Broadcast(planName);
		}
		else if (Type == EPlanListType::PostProcess)
		{
			DelegateManager::Get()->AddPostProcessPlan.Broadcast(planName);
		}
	}

	if (OnAddPlan.IsBound())
	{
		FReply Reply = OnAddPlan.Execute();
		return Reply;
	}
	else
	{
		return FReply::Handled();
	}



	return FReply::Handled();
}


void SPlanDropList::CheckBoxChanged(ECheckBoxState newState)
{
	if (Type == EPlanListType::Material)
	{
		SaveDataManager::Get()->bAllowCopySelectedPlan = newState == ECheckBoxState::Checked ? true : false;
	}
}