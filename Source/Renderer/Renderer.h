#pragma once

#include "Renderer/Framebuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/UniformBuffer.h"

#include "Scene/Camera.h"

class Renderer
{
public:
    // Creates a renderer that draws directly to the back buffer
    Renderer();

    // Creates a renderer that draws to the specified framebuffer.
    Renderer(const Framebuffer* targetFramebuffer);

    // Renders a new frame from the point of view
    // of the specified camera.
    void renderFrame(const Camera* camera) const;

private:
    // The framebuffer being rendered to
    const Framebuffer* targetFramebuffer_;

    // Uniform buffers used in different render stages
    UniformBuffer<SceneUniformData> sceneUniformBuffer_;
    UniformBuffer<CameraUniformData> cameraUniformBuffer_;
    UniformBuffer<PerDrawUniformData> perDrawUniformBuffer_;
    UniformBuffer<TerrainUniformData> terrainUniformBuffer_;

    // Shaders used for each render pass
    ResourcePPtr<Shader> forwardShader_;
    ResourcePPtr<Shader> terrainShader_;

    // Methods for updating the contents of uniform buffers
    void updateSceneUniformBuffer() const;
    void updateCameraUniformBuffer(const Camera* camera) const;
    void updatePerDrawUniformBuffer(const StaticMesh* draw) const;
	void updateTerrainUniformBuffer(const Terrain* terrain) const;

    // Methods for each render pass
    void executeForwardPass() const;
};