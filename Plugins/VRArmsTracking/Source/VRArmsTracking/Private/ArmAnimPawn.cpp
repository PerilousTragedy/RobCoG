// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "ArmAnimPawn.h"
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
	
	InitializeRootComponent();
}

// Called when the game starts or when spawned
void UArmAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	//Set default values
	RightArmBoneName = FName("upperarm_r");
	LeftArmBoneName = FName("upperarm_l");
	BlendWeight = 1.0f;

	//Setup the physical animation and crate animation data with magical numbers

	HMDName = "OculudHMD";//UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName().ToString();

	//adjust relative position of camera and mesh to fit HMD
	if (HMDName == "OculusHMD") {
		//Camera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		//Mesh->SetRelativeLocation(FVector(ShouldersLocalX, 0, -UserHeight));
		GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, UserHeight));
	}
	else if (HMDName == "SteamVR") {
		//Camera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		//Mesh->SetRelativeLocation(FVector(ShouldersLocalX, 0, -UserHeight));
		GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, UserHeight));
	}

	SetupPlayerInputComponent();
}

// Called every frame
void UArmAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Mesh follows HMD
	if (bHeadReset == true) 
	{
		FVector HeadMovedDistance = HeadStartLocation - Camera->GetComponentLocation();
		Mesh->SetWorldLocation(MeshStartLocation - HeadMovedDistance);
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

	Owner->InputComponent->BindAction("resetPosition", IE_Pressed, this, &UArmAnimComponent::ResetHeadPosition);
}

//Reset HMD position and make mesh follow HMD
void UArmAnimComponent::ResetHeadPosition()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(0, EOrientPositionSelector::OrientationAndPosition);
	FTimerHandle UnusedHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(UnusedHandle, this, &UArmAnimComponent::SetMovementValues, 0.5, false);
}

void UArmAnimComponent::SetMovementValues()
{
	MeshStartLocation = Mesh->GetComponentLocation();
	HeadStartLocation = Camera->GetComponentLocation();
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

void UArmAnimComponent::InitializeRootComponent()
{

	TSet<UActorComponent*> Components = GetOwner()->GetComponents();
	for (auto s : Components) {
		if (s->IsA(UCameraComponent::StaticClass()))
		{
			UCameraComponent* CameraTest = static_cast<UCameraComponent*>(s);
			Camera = CameraTest;
		}
		else if (s->IsA(USkeletalMeshComponent::StaticClass()))
		{
			USkeletalMeshComponent* MeshTest = static_cast<USkeletalMeshComponent*>(s);
			Mesh = MeshTest;
			Mesh->SetRelativeRotation(MeshRotation);
			Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		}
		else if (s->IsA(UMotionControllerComponent::StaticClass()))
		{
			UMotionControllerComponent* MotionController = static_cast<UMotionControllerComponent*>(s);
			if (MotionController->Hand == EControllerHand::Left)
			{
				MotionControllerLeft = MotionController;
			}
			else
			{
				MotionControllerRight = MotionController;
			}
		}

	}
	//Create Default Root component
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	////Create the camera root component
	//CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	//CameraRoot->SetupAttachment(RootComponent);

	////Create default camera
	//Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Camera->SetupAttachment(CameraRoot);
	//Camera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, UserHeight), FRotator(0.0f, 0.0f, 0.0f));

	////Create the mesh component 
	//Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(RootComponent);
	//Mesh->SetRelativeRotation(MeshRotation);
	//Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//Mesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(Mesh, *SkeletalPath));

	////Setup the physical animation for this mesh
	//Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Mesh->bMultiBodyOverlap = true;
	//Mesh->bGenerateOverlapEvents = 1;
	//Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	//Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	//Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	//Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	////Create default motion controllers
	//MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	//MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	//MotionControllerLeft->SetupAttachment(RootComponent);
	//MotionControllerRight->SetupAttachment(RootComponent);
	//MotionControllerLeft->Hand = EControllerHand::Left;
	//MotionControllerRight->Hand = EControllerHand::Right;

	//// Finalize the initialziation process
	//GetOwner()->SetRootComponent(RootComponent);
}