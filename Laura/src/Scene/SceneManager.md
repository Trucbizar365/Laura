# Scene Manager Design Plan

## Overview
The Scene Manager will take in a scene and output the correct data structure for the renderer to process and render. Its responsibilities include iterating over the scene, finding the main camera, determining which objects need to be rendered, and optimizing the rendering pipeline by organizing and submitting necessary data to the renderer.

## Responsibilities

### Camera and Object Management
- The Scene Manager will iterate over the scene to find the main camera.
- If no main camera is found, the Scene Manager will not pass any objects to the renderer, resulting in a black screen.
- The Scene Manager will check for updates from the Asset Manager, specifically to determine if any new meshes have been added, in which case it will construct the BVH for the new mesh.

### Object Visibility and Dynamics
- During the iteration, the Scene Manager will identify which objects need to be rendered by checking for the presence of a mesh, material, and transform.
- It will determine if each object is dynamic. If an object is static (not dynamic), the Scene Manager will use the BVH that was constructed when the mesh was imported, ensuring efficiency.

### TLAS Construction
- Based on the objects identified for rendering, the Scene Manager will create a Top-Level Acceleration Structure (TLAS) that organizes all the objects in the scene based on their bounding volumes.

### Handling Mesh Data
- If multiple objects share the same mesh, the Scene Manager will only pass the mesh triangles once to the renderer to avoid redundancy.
- The Scene Manager will request a pointer to each mesh's unique BVH from the Asset Manager to speed up rendering.

### BVH for Each Mesh
- The Asset Manager will be responsible for generating a BVH for each mesh during asset import. This BVH will be written to a file for quicker loading in future sessions.

## Submission to Renderer
The Scene Manager will submit the following to the renderer:
- **Non-repeating Triangles**: A list of triangles for all visible objects, ensuring that shared meshes are only submitted once.
- **Mesh BVHs**: A BVH for each unique mesh.
- **TLAS**: The top-level acceleration structure that organizes the objects spatially.
- **Materials**: A list of unique materials used by the visible objects. Each object currently has one material, but support for multiple materials and textures per object will be expanded in the future.
- **Main Camera**: The camera’s data for view and projection transformations.
- **Skybox Texture**: The texture used for rendering the skybox.

## Summary
The Scene Manager serves as the intermediate system that optimizes and organizes the scene's data before submitting it to the renderer. By leveraging BVH structures, material deduplication, and TLAS creation, it ensures efficient rendering and data management. The Asset Manager will handle per-mesh BVH generation and caching for fast asset loading. Future plans include expanding material and texture support for more complex rendering scenarios.
