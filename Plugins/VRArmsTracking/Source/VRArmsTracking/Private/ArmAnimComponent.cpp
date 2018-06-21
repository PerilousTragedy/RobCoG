// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "ArmAnimComponent.h"

#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
UArmAnimComponent::UArmAnimComponent()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	bWantsBeginPlay = true;

	//player one will use this
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called before BeginPlay()
void UArmAnimComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	SetPawnComponents();
}

// Called when the game starts or when spawned
void UArmAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	//Set default values
	RightArmBoneName = FName("upperarm_r");
	LeftArmBoneName = FName("upperarm_l");
	BlendWeight = 1.0f;

	HMDName = "OculudHMD";//UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName().ToString();

	SetupPlayerInputComponent();
}

// Called every frame
void UArmAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Mesh follows HMD
	if (bHeadReset == true) 
	{
		Mesh->SetWorldLocation(Camera->GetComponentLocation() + FVector(0, 0, -160));

		if (!bIsTurning)
		{
			float ResultHead = HeadYawStart.Yaw - Camera->GetComponentRotation().Yaw;
			float ResultLHand = ControllerLYawStart.Yaw - MotionControllerLeft->GetComponentRotation().Yaw;
			float ResultRHand = ControllerRYawStart.Yaw - MotionControllerRight->GetComponentRotation().Yaw;
			if (ResultHead < -TurnThreshold && ResultHead > -360 + TurnThreshold && ResultLHand < -TurnThreshold && ResultRHand < -TurnThreshold)
			{
				RotationStep = FRotator(0, TurnThreshold / MaxTurnSteps, 0);
				if (ResultHead < -180) {
					RotationStep = FRotator(0, -(TurnThreshold / MaxTurnSteps), 0);
				}
				RotationStep = FRotator(0, TurnThreshold / MaxTurnSteps, 0);
				TurnStepCounter = 0;
				bIsTurning = true;
				GetOwner()->GetWorldTimerManager().SetTimer(TurnTimer, this, &UArmAnimComponent::TurnInSteps, 0.01, true);
			}
			else if (ResultHead > TurnThreshold && ResultHead < 360 - TurnThreshold && ResultLHand > TurnThreshold && ResultRHand > TurnThreshold)
			{
				RotationStep = FRotator(0, -(TurnThreshold / MaxTurnSteps), 0);
				if (ResultHead > 180) {
					RotationStep = FRotator(0, TurnThreshold / MaxTurnSteps, 0);
				}
				TurnStepCounter = 0;
				bIsTurning = true;
				GetOwner()->GetWorldTimerManager().SetTimer(TurnTimer, this, &UArmAnimComponent::TurnInSteps, 0.01, true);
			}
		}
	}

}

// Called to bind functionality to input
void UArmAnimComponent::SetupPlayerInputComponent()
{
	APawn* Owner = (APawn*)GetOwner();

	Owner->InputComponent->BindAction("resetPosition", IE_Pressed, this, &UArmAnimComponent::SetMovementValues);
}

void UArmAnimComponent::SetMovementValues()
{
	HeadStartLocation = Camera->GetComponentLocation();
	MeshStartLocation = HeadStartLocation + FVector(0, 0, -160);

	HeadYawStart = Camera->GetComponentRotation();
	ControllerRYawStart = MotionControllerRight->GetComponentRotation();
	ControllerLYawStart = MotionControllerLeft->GetComponentRotation();
	Mesh->SetRelativeRotation(MeshRotation);
	bHeadReset = true;
}

void UArmAnimComponent::TurnInSteps()
{
	if (TurnStepCounter < MaxTurnSteps)
	{
		FVector MeshForwardBefore = Mesh->GetForwardVector();
		MeshStartLocation = MeshStartLocation - (MeshForwardBefore *ShouldersLocalX);
		Mesh->SetWorldRotation((Mesh->GetComponentRotation()+RotationStep));
		HeadYawStart += RotationStep;
		ControllerLYawStart += RotationStep;
		ControllerRYawStart += RotationStep;
		FVector MeshForwardAfter = Mesh->GetForwardVector();
		MeshStartLocation = MeshStartLocation + (MeshForwardAfter *ShouldersLocalX);
		TurnStepCounter++;
	}
	else 
	{
		GetOwner()->GetWorldTimerManager().SetTimer(TurnTimer, this, &UArmAnimComponent::TurnInSteps, -1, false);
		bIsTurning = false;
	}
}

// Get all the components created by the MCCharacter and save them to internal variables
void UArmAnimComponent::SetPawnComponents()
{
	TSet<UActorComponent*> ActorComponents = GetOwner()->GetComponents();
	for (UActorComponent* Component : ActorComponents) {
		if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(Component))
		{
			Camera = CameraComponent;
		}
		else if (USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Component))
		{
			Mesh = SkeletalMeshComponent;
			Mesh->SetRelativeRotation(MeshRotation);
			Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		}
		else if (UMotionControllerComponent* MotionControllerComponent = Cast<UMotionControllerComponent>(Component))
		{
			if (MotionControllerComponent->Hand == EControllerHand::Left)
			{
				MotionControllerLeft = MotionControllerComponent;
			}
			else
			{
				MotionControllerRight = MotionControllerComponent;
			}
		}
	}
}