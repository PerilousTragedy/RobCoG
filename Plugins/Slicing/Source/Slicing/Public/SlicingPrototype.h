#pragma once

#include "Slicing.h"
#include "LevelEditor.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "StaticMeshResources.h"

class FSlicingPrototype
{
	public:

		// 0 = CreateNewSectionForCap (default), 1 = No Cap, 2 = UseLastSectionForCap
		int NewSectionOptions = 0;

		bool bDebugPrintText = false;
		bool bDebugShowPlane = false;
		bool bDebugShowWay = false;

		void SetMaterialForSection(UMaterialInterface* InputTexture);

	private:

		UMaterialInterface* MaterialReferenceNewSection = NULL;

		void Debug(UPrimitiveComponent* InputBladeComponent);
		void ConvertToProceduralMeshComponent(UPrimitiveComponent* ReferencedComponent);
		void CutGivenComponent(UPrimitiveComponent* ReferencedComponent, FVector PlanePosition, FVector PlaneNormal);
};