#pragma once

#include "Util/TextureId.h"
#include "Util/G3DCore.h"

namespace ARL
{
	class PartInstance;
	class Humanoid;
	class CharacterMesh;
	class Accoutrement;

		// if the part is a humanoid, get further details with this.
		class HumanoidIdentifier
		{
		public:
			explicit HumanoidIdentifier(ARL::Humanoid* humanoid);

			Humanoid* humanoid;

			PartInstance* head;
			PartInstance* leftLeg;
			PartInstance* rightLeg;
			PartInstance* leftArm;
			PartInstance* rightArm;
			PartInstance* torso;
			
			std::vector<Accoutrement*> accoutrements;
			
			TextureId pants;
			TextureId shirt;
			TextureId shirtGraphic;

			CharacterMesh* leftLegMesh;
			CharacterMesh* rightLegMesh;
			CharacterMesh* leftArmMesh;
			CharacterMesh* rightArmMesh;
			CharacterMesh* torsoMesh;

			bool isBodyPart(ARL::PartInstance* part) const;
			bool isBodyPartComposited(ARL::PartInstance* part) const;
			bool isPartComposited(ARL::PartInstance* part) const;
			bool isPartHead(ARL::PartInstance* part) const;

			// helper
			CharacterMesh* getRelevantMesh(ARL::PartInstance* bodyPart) const;

            enum BodyPartType
            {
                PartType_Head,
                PartType_Torso,
                PartType_Arm,
                PartType_Leg,
                PartType_Unknown,
                PartType_Count
            };

            BodyPartType getBodyPartType(ARL::PartInstance* bodyPart) const;
            Vector3 getBodyPartScale(ARL::PartInstance* bodyPart) const;
		};
}
