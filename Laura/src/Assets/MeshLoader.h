#pragma once

#include "lrpch.h"
#include "Components/MeshComponent.h"

namespace Laura 
{
    namespace MeshLoader
    {
        // Loads a 3D mesh from various filetypes .obj .glb .gltf ...
        MeshComponent loadMesh(const std::string& filepath);
    }
}