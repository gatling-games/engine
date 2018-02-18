#include "Scene.h"

#include <imgui.h>

#include "SceneManager.h"

Scene::Scene(ResourceID resourceID)
    : Resource(resourceID),
    gameObjects_()
{

}

void Scene::drawEditor()
{
    ImGui::Text("Scene contains %d gameobjects", (int)gameObjects_.size());
}

void Scene::serialize(PropertyTable &table)
{
    // Serialize the entire list of gameobjects
    table.serialize("gameobjects", gameObjects_);
}