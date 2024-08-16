#pragma once

#include "Components/SkyboxComponent.h"

/// This whole thing is very temporary
// in use until there will be an ECS to manage entities for not it is a simple data storage class containing a specific component
namespace Laura 
{
	class EnvironmentEntity
	{
	public:
		EnvironmentEntity();

		SkyboxComponent skybox;
	};
}