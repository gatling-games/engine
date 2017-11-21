#pragma once

#include "Editor/EditorPanel.h"
#include "Renderer/Texture.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"

// Todo: Move into Scene.h and Material.h
struct Material
{
    Shader* shader;
    Texture* texture;
};

struct SceneObject
{
    Mesh* mesh;
    Material* material;
    Matrix4x4 localToWorld;
};

class GamePanel : public EditorPanel
{
public:
    GamePanel(ResourceManager* resourceManager);
    ~GamePanel();

    // EditorPanel overrides
    std::string name() const override { return "Game Panel"; }
    void draw() override;
    void onResize(int width, int height) override;

private:
    Framebuffer* frameBuffer_;
    Texture* depthBuffer_;
    Texture* colorBuffer_;

    ResourceManager* resourceManager_;

    // Todo: Move into Scene.h
    std::vector<SceneObject> objects_;
    Camera camera_;
};
