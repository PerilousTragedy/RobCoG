// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealisticOpeningActor.generated.h"

UCLASS()
class UCORKSCREW_API ARealisticOpeningActor : public APhysicsConstraintActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARealisticOpeningActor();
	ARealisticOpeningActor(const FObjectInitializer & ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Needed to extend APhysicsConstraintActor
	virtual void LoadedFromAnotherClass(const FName & OldClassName);
	void PostLoad();


private:
	UPROPERTY(EditAnywhere, Category = "ScrewPropertys")
		bool bIsActiveOpeningConstraint = true;
	UPROPERTY(EditAnywhere, Category = "ScrewPropertys")
		AActor* Bottle;
	UPROPERTY(EditAnywhere, Category = "ScrewPropertys")
		AActor* Lid;
	// Sets the angle. At this angle the constraint will break if the function is activated.
	UPROPERTY(EditAnywhere, Category = "ScrewPropertys", meta = (UIMin = "1.0"))
		float DestroyAngle = 60;
	// Distance it should move up. Is just prepared yet.
	UPROPERTY(EditAnywhere, Category = "ScrewPropertys", meta = (UIMin = "0.0"))
		float UpMovingDistance = 0;
	// If its true, the rotation to open it is to the right. Else it is going to be left.
	UPROPERTY(EditAnywhere, Category = "ScrewPropertys")
		bool bIsRotateDirectionRight = true;

	// If true, it turns the debugging mode on. This causes the Slider to work and to log the debug values. Standard is false for normal use.
	UPROPERTY(EditAnywhere, Category = "ForDebugging")
		bool bIsDebuggingModeOn = false;
	// Turning distance in percent. The value 100 is the max value. It should rotate slowly until the rotation is reached.
	UPROPERTY(EditAnywhere, Category = "ForDebugging", meta = (UIMin = "0.0", UIMax = "100.0"))
		float MoveToDegree = 0;

	

	// Some needed variables, in different functions.

	// Variables that helps to know which values should be logged.
	float OldCurrentDegreeForDebug;
	float OldSwingDegreeDebug;
	float MoveToAngle;
	int TickCounter;

	// Helper variable to make a calculation easier.
	float UpMovingDistancePerDegree;
	// Old degree value of swing 1.
	float OldSwing1Degree;
	// The turn flag says, if it is turned more than 30 degrees or less.
	// Important to know if the limit should be locked or not.
	bool bIsTurnFlagOn;
	// Counts every half turn.
	int TurnCounter;
	// Up moving distance per degree

	// Sets values for the bottle opening constraint.
	void BottleOpening();
	// Activates the Limits for the constraint.
	void ActivateConstraintLimit();
	// Checks if a value is changed in the editor.
	void PostEditChangeProperty(struct FPropertyChangedEvent& e);
	// Calculates the actual real degrees the lid is turned on the right site.
	float GetDegreeCalcRight();
	// Calculates the actual real degrees the lid is turned on the left site.
	float GetDegreeCalcLeft();
	// Moves and breaks the constraint, depeends on the current degrees its turned.
	void MovingAndBreaking(float CurrentDegree);
};
