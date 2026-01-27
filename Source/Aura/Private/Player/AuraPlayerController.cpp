// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	/**
	 * 有这样几种情况
	 * 1.LastActor和ThisActor都为空 -什么都不做
	 * 2.LastActor为空ThisActor有效 -Hightlight ThisActor
	 * 3.LastActor有效ThisActor为空 -无高光的lastActor
	 * 4.两个都有效
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor ->HighlightActor();
		}
		else
		{
			
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor ->unHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor ->unHighlightActor();
				ThisActor ->HighlightActor();
			}
			else
			{
				//do nothing
			}
		}
	}
}
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
    
	// 添加调试日志
	if (MoveAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveAction绑定成功"));
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AAuraPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AAuraPlayerController::Move);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MoveAction为空，无法绑定"));
	}
}

void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	// 添加调试日志
	UE_LOG(LogTemp, Warning, TEXT("Move函数被调用，输入值: %s"), *InputActionValue.Get<FVector2D>().ToString());
    
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotation(0.f, Rotator.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn)
	{
		// 添加调试日志
		UE_LOG(LogTemp, Warning, TEXT("获取到受控角色，应用移动"));
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("未获取到受控角色"));
	}
}


