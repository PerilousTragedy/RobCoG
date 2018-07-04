// Fill out your copyright notice in the Description page of Project Settings.

#include "RealisticOpeningActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

// Sets default values
ARealisticOpeningActor::ARealisticOpeningActor() 
{
	PrimaryActorTick.bCanEverTick = true;
	if (bIsActiveOpeningConstraint) 
		{
			BottleOpening();
		}
	
}

ARealisticOpeningActor::ARealisticOpeningActor(const FObjectInitializer & ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (bIsActiveOpeningConstraint) 
		{
			BottleOpening();
		}
}

// Called when the game starts or when spawned
void ARealisticOpeningActor::BeginPlay()
{
	Super::BeginPlay();
	if (bIsActiveOpeningConstraint) 
		{
			BottleOpening();
		}
}

// Called every frame
void ARealisticOpeningActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float currentDegree;
	if (bIsRotateDirectionRight) 
		{
			currentDegree = GetDegreeCalcRight();
			MovingAndBreaking(currentDegree);
		}
		else 
		{
			currentDegree = GetDegreeCalcLeft();
			MovingAndBreaking(currentDegree);
		}
	if (bIsDebuggingModeOn) {
		TickCounter++;
		if (TickCounter % 100 == 0) {
			/*if (MoveToAngle >= currentDegree) 
			{
				this->GetConstraintComp()->ConstraintActor2->AddActorLocalRotation(FRotator::FRotator(0, 0, 1));
				UE_LOG(LogTemp, Display, TEXT("Goes to right."));
			}
			else if (MoveToAngle <= currentDegree) 
				{
					this->GetConstraintComp()->ConstraintActor2->AddActorLocalRotation(FRotator::FRotator(0, 0, -1));
					UE_LOG(LogTemp, Display, TEXT("Goes to left."));
				}*/
		}
		if (OldCurrentDegreeForDebug+1 < currentDegree || OldCurrentDegreeForDebug-1 > currentDegree)
			{
				UE_LOG(LogTemp, Display, TEXT("Current Degree: %f"), currentDegree);
				UE_LOG(LogTemp, Display, TEXT("Current Swing1 Degree: %f"), this->GetConstraintComp()->GetCurrentSwing1());
				if(this->GetConstraintComp()->IsBroken())
					{
						UE_LOG(LogTemp, Display, TEXT("Is broken!"));
					}
				OldCurrentDegreeForDebug = currentDegree;
			}
	}
}

void ARealisticOpeningActor::LoadedFromAnotherClass(const FName & OldClassName) 
{
}

void ARealisticOpeningActor::PostLoad() 
{
	AActor::PostLoad();
}

void ARealisticOpeningActor::BottleOpening() 
{
	TurnCounter = 0;
	if (bIsActiveOpeningConstraint) 
		{
			// Sets the constraint limit, also checks in which direction it should be turnable.
			ActivateConstraintLimit();
			if (Bottle != nullptr) 
				{
					this->SetActorLocation(Bottle->GetActorLocation());
					this->GetConstraintComp()->ConstraintActor1 = Bottle;
					if (Lid != nullptr) 
						{
							this->GetConstraintComp()->ConstraintActor2 = Lid;
						}
				}
			bIsTurnFlagOn = false;
			// Getting the distance to move up per degree.
			UpMovingDistancePerDegree = UpMovingDistance / DestroyAngle;

			OldSwingDegreeDebug = this->GetConstraintComp()->GetCurrentSwing1();
			OldCurrentDegreeForDebug = 0;
			TickCounter = 0;
	}
}

void ARealisticOpeningActor::ActivateConstraintLimit() 
{
	this->GetConstraintComp()->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	this->GetConstraintComp()->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	// Locks movement at 30 degrees.
	this->GetConstraintComp()->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 30.0f);
	if (bIsRotateDirectionRight) 
		{
			// Gives a 30 degree offset to make it impossible turning it to the left, since the 30 degree limit.
			this->GetConstraintComp()->ConstraintInstance.AngularRotationOffset = FRotator::FRotator(0, 30, 0);
		}
		else 
		{
			// Gives a -30 degree offset to make it impossible turning it to the left, since the 30 degree limit.
			this->GetConstraintComp()->ConstraintInstance.AngularRotationOffset = FRotator::FRotator(0, -30, 0);
		}
	this->GetConstraintComp()->SetLinearPositionDrive(false, false, true);
	this->GetConstraintComp()->SetLinearPositionTarget(FVector::FVector(0, 0, 0));
	this->GetConstraintComp()->SetLinearVelocityDrive(false, false, true);
	this->GetConstraintComp()->SetLinearDriveParams(50000, 50000, 0);
	this->GetConstraintComp()->SetLinearVelocityTarget(FVector::FVector(0, 0, 0));
	this->GetConstraintComp()->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 0);
	
	this->GetConstraintComp()->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	this->GetConstraintComp()->SetAngularVelocityDriveTwistAndSwing(false, true);
	this->GetConstraintComp()->SetAngularDriveParams(1, 1, 0);
	this->GetConstraintComp()->SetAngularDriveParams(50000, 50000, 0);
	
	OldSwing1Degree = this->GetConstraintComp()->GetCurrentSwing1();
}

float ARealisticOpeningActor::GetDegreeCalcRight()
{
	// Saves the current degrees of Swing 1.
	float CurrentSwing1Degree = this->GetConstraintComp()->GetCurrentSwing1();
	// Save variable for saving the degree number that are over the 180 degree points.
	float CalculatedOver180Degrees = 0;

	// Checks the direction of the movement. If the direction is to the right its true.
	// Current value will be bigger than the old one if it turns to the right.
	if (CurrentSwing1Degree > OldSwing1Degree)
	{
		// First "OR"-part checks if the 150 degrees are past, because then the 180 degree point is passed and need to be counted.
		// Second "OR"-part checks if the -30 degree are past, also if its already turning, 
		// because then the 180 degree point is passed and need to be counted.
		// The last part makes sure that is turning and it wasnt a left turn over 180 degree point. 
		// If anyone can turn it 20 Degrees in less than 1 tick this fix wont work.
		if (((CurrentSwing1Degree >= 150 && OldSwing1Degree < 150) || (CurrentSwing1Degree >= -30 && OldSwing1Degree < -30)) && bIsTurnFlagOn && CurrentSwing1Degree >= -170 && CurrentSwing1Degree <= 170)
		{
			// Count half turn.
			++TurnCounter;
		}

		// Checks if the 0 degree are point is passed, to set the turning limit free and save that its turning in the turning flag.
		if (!bIsTurnFlagOn && CurrentSwing1Degree >= 0 && OldSwing1Degree < 0)
		{
			this->GetConstraintComp()->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 360.f);
			bIsTurnFlagOn = true;
		}
	}

	// Checks the direction of the movement. If the direction is to the left its true.
	// Current value will be bigger than the old one if it turns to the left.
	else if (CurrentSwing1Degree < OldSwing1Degree)
	{
		// First "OR"-part checks if the 150 degrees are past, because then the 180 degree point is passed and need to be counted.
		// Second "OR"-part checks if the -30 degree are past, also if its already turning, 
		// because then the 180 degree point is passed and need to be counted.
		// The last part makes sure that is turning and it wasnt a left turn over 180 degree point. 
		// If anyone can turn it 20 Degrees in less than 1 tick this fix wont work.
		if ((CurrentSwing1Degree <= 150 && OldSwing1Degree > 150 || CurrentSwing1Degree <= -30 && OldSwing1Degree > -30) && bIsTurnFlagOn && CurrentSwing1Degree >= -170 && CurrentSwing1Degree <= 170)
		{
			// Removes a half turn after passing the half turn point.
			--TurnCounter;
		}
		// Makes sure it can't go over the start point. Also setting the turn flag off.
		if (CurrentSwing1Degree <= 0 && OldSwing1Degree > 0 && TurnCounter == 0)
		{
			this->GetConstraintComp()->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 30.0f);
			bIsTurnFlagOn = false;
		}
	}

	// Calculates the Degrees over the half turn points.
	if (CurrentSwing1Degree >= -30 && CurrentSwing1Degree <= 180)
	{
		// Degree calculation on the Degrees which are over 180.
		CalculatedOver180Degrees = CurrentSwing1Degree + 30;
		// If its bigger than 180, subtract 180 degrees to get the degrees, that are over 180.
		if (CalculatedOver180Degrees >= 180)
		{
			CalculatedOver180Degrees = CalculatedOver180Degrees - 180;
		}
	}
	else if(bIsTurnFlagOn)
	{
		// Since the actual CurrentSwing1Degree value must be below -30 its a negative value.
		// By adding 210 and the negatvie degree value the correct value above the 180 degree point gets calculated.
		CalculatedOver180Degrees = 210 + CurrentSwing1Degree;
	}

	// Set the current Swing1 Degrees as OldSwing1Degrees for the next run in this function.
	OldSwing1Degree = CurrentSwing1Degree;
	// Returns the actual real turned degrees.
	return (CalculatedOver180Degrees + (TurnCounter * 180));
}

float ARealisticOpeningActor::GetDegreeCalcLeft() 
{
	// Saves the current degrees of Swing 1.
	float CurrentSwing1Degree = this->GetConstraintComp()->GetCurrentSwing1();
	// Save variable for saving the degree number that are over the 180 degree points.
	float CalculatedOver180Degrees;

	// Checks the direction of the movement. If the direction is to the left its true.
	// Current value will be bigger than the old one if it turns to the left.
	if (CurrentSwing1Degree < OldSwing1Degree)
	{
		// First "OR"-part checks if the -150 degrees are past, because then the 180 degree point is passed and need to be counted.
		// Second "OR"-part checks if the 30 degree are past, also if its already turning, 
		// because then the 180 degree point is passed and need to be counted.
		// The last part makes sure that is turning and it wasnt a left turn over 180 degree point. 
		// If anyone can turn it 20 Degrees in less than 1 tick this fix wont work.
		if ((CurrentSwing1Degree <= -150 && OldSwing1Degree > -150 || CurrentSwing1Degree <= 30 && OldSwing1Degree > 30) && bIsTurnFlagOn && CurrentSwing1Degree >= -170 && CurrentSwing1Degree <= 170)
		{
			// Count half turn.
			++TurnCounter;
		}

		// Checks if the 0 degree are point is passed, to set the turning limit free and save that its turning in the turning flag.
		if (CurrentSwing1Degree <= 0 && OldSwing1Degree > 0)
		{
			this->GetConstraintComp()->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 360.f);
			bIsTurnFlagOn = true;
		}
	}

	// Checks the direction of the movement. If the direction is to the right its true.
	// Current value will be bigger than the old one if it turns to the right.
	if (CurrentSwing1Degree > OldSwing1Degree)
	{
		// First "OR"-part checks if the -150 degrees are past, because then the 180 degree point is passed and need to be counted.
		// Second "OR"-part checks if the 30 degree are past, also if its already turning, 
		// because then the 180 degree point is passed and need to be counted.
		// The last part makes sure that is turning and it wasnt a left turn over 180 degree point. 
		// If anyone can turn it 20 Degrees in less than 1 tick this fix wont work.
		if ((CurrentSwing1Degree >= -150 && OldSwing1Degree < -150 || CurrentSwing1Degree >= 30 && OldSwing1Degree < 30) && bIsTurnFlagOn && CurrentSwing1Degree >= -170 && CurrentSwing1Degree <= 170)
		{
			// Removes a half turn after passing the half turn point.
			--TurnCounter;
		}
		// Makes sure it can't go over the start point. Also setting the turn flag off.
		if (CurrentSwing1Degree >= 0 && OldSwing1Degree < 0 && TurnCounter == 0)
		{
			this->GetConstraintComp()->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 30.0f);
			bIsTurnFlagOn = false;
		}
	}

	// Calculates the Degrees over the half turn points.
	if (CurrentSwing1Degree <= 30 && CurrentSwing1Degree >= -180)
	{
		// Degree calculation on the Degrees which are over 180.
		CalculatedOver180Degrees = 30 - CurrentSwing1Degree;
		// If its bigger than 180, subtract 180 degrees to get the degrees, that are over 180.
		if (CalculatedOver180Degrees >= 180)
		{
			CalculatedOver180Degrees = CalculatedOver180Degrees - 180;
		}
	}
	else
	{
		// Since the actual CurrentSwing1Degree value must be above 30 its a positive value.
		// By subtracting 210 and the degree value the correct value above the 180 degree point gets calculated.
		CalculatedOver180Degrees = 210 - CurrentSwing1Degree;
	}

	// Set the current Swing1 Degrees as OldSwing1Degrees for the next run in this function.
	OldSwing1Degree = CurrentSwing1Degree;
	// Returns the actual real turned degrees.
	return (CalculatedOver180Degrees + (TurnCounter * 180));
}

// Function to move the lid and break the constraint when the screw angle is passed.
void ARealisticOpeningActor::MovingAndBreaking(float CurrentDegree) 
{
	// To move upwards.
	float moveUp = -(CurrentDegree * UpMovingDistancePerDegree);
	this->GetConstraintComp()->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, -moveUp);
	this->GetConstraintComp()->SetLinearPositionTarget(FVector::FVector(0, 0, moveUp));
	this->GetConstraintComp()->SetLinearVelocityDrive(false, false, true);

	// If it has reached or passed the screw angle with the degrees, break it.
	if (CurrentDegree >= DestroyAngle) 
		{
			this->GetConstraintComp()->BreakConstraint();
		}
}

// Helpfull URL for this function: https://answers.unrealengine.com/questions/252811/how-do-i-use-posteditchangeproperty.html
// Gets triggerd each time a property is changed in the editor.
void ARealisticOpeningActor::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	// Name of the property that is changed. If the proberty isn't named at all, set it on NAME_None.
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	// If the bottle is set.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ARealisticOpeningActor, Bottle) && bIsActiveOpeningConstraint) 
		{
			// Sets the location of the constraint on the position of the bottle automaticly.
			this->SetActorLocation(Bottle->GetActorLocation());
			// Sets the rotation of the constraint on the same rotation the bottle has automaticly.
			this->SetActorRotation(Bottle->GetActorRotation());
			// Set this actor already on the constraint, so it also gets the usal red color for the first actor.
			this->GetConstraintComp()->ConstraintActor1 = Bottle;
		}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ARealisticOpeningActor, Lid) && bIsActiveOpeningConstraint) 
		{
			// Set this actor already on the constraint, so it also gets the usal blue color for the second actor.
			this->GetConstraintComp()->ConstraintActor2 = Lid;
		}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ARealisticOpeningActor, bIsActiveOpeningConstraint)) 
		{
			// If the Opening constraint should be active set everything for the bottle opening.
			if (bIsActiveOpeningConstraint) 
				{
					BottleOpening();
				}
		}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ARealisticOpeningActor, bIsRotateDirectionRight)) 
		{
			// If the Opening constraint should be active set everything for the bottle opening.
			// Will set it the way it's needed for the rotation way.
			if (bIsActiveOpeningConstraint) 
			{
				BottleOpening();
			}
		}
}