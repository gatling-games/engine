#pragma once

#include "Scene/Component.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "Math/Color.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

class Material;

struct TerrainLayer : ISerializedObject
{
    float altitudeBorder = 0.0f;
    float altitudeTransition = 0.0f;
    float slopeBorder = -1.0f;
    float slopeHardness = 1.0f;
    Vector2 textureTileSize = Vector2(10.0f, 10.0f);
    Vector2 textureTileOffset = Vector2::zero();
    Material* material = ResourceManager::instance()->load<Material>("Resources/Materials/ground_grass_01.material");;

    void serialize(PropertyTable& table) override;
};

class Terrain : public Component
{
public:
    const static int HEIGHTMAP_RESOLUTION = 1024;
    const static int MAX_LAYERS = 32;

    explicit Terrain(GameObject* gameObject);
    ~Terrain() override { }

    // Draws the properties fold out
    void drawProperties() override;

    // Serialisation function
    void serialize(PropertyTable &table) override;

    const Mesh* mesh() const { return mesh_; }
    const Texture* heightmap() const { return &heightMap_; }

    // Total size of the terrain, in m, in X,Y,Z
    Vector3 size() const { return dimensions_; }
    
    // The base color of the water
    Color waterColor() const { return waterColor_; }

    // The depth of the water at its deepest point
    float waterDepth() const { return waterDepth_; }

    const TerrainLayer* layers() const { return &terrainLayers_.front(); }
    int layerCount() const { return (int)terrainLayers_.size(); }

private:
    Mesh* mesh_;
    Texture heightMap_;
    Vector3 dimensions_;
    Color waterColor_;
    float waterDepth_;
    std::vector<TerrainLayer> terrainLayers_;

    // Terrain generation settings
    int seed_;
    float fractalSmoothness_;
    float mountainScale_;
    float islandFactor_;

    // The current heightmap
    std::vector<float> heights_;

    // A list of objects placed on the terrain
    std::vector<GameObject*> placedObjects_;

    // Regenerates the terrain
    void generateTerrain();
    void placeObjects();

    // Gets the heightmap height at a specified point
    // The x and z coordinates are in world space.
    float sampleHeightmap(float x, float z);
};
