#include "SceneManager.h"
#include "imgui.h"

#include "Editor/MainWindowMenu.h"
#include "Editor/PropertiesPanel.h"

#include "Scene/GameObject.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"
#include "Scene/StaticMesh.h"
#include "Scene/Terrain.h"
#include "Scene/Helicopter.h"

#include "Serialization/Prefab.h"
#include "ResourceManager.h"

#include "Utils/Clock.h"
#include "Scene/Freecam.h"

SceneManager::SceneManager()
    : gameObjects_()
{
	// Create default gameobjects
    createGameObject(ResourceManager::instance()->load<Prefab>("Resources/Prefabs/Camera.prefab"));
    createGameObject(ResourceManager::instance()->load<Prefab>("Resources/Prefabs/Terrain.prefab"));
    createGameObject(ResourceManager::instance()->load<Prefab>("Resources/Prefabs/Helicopter.prefab"));

    // Register menu items for creating new gameobjects
    addCreateGameObjectMenuItem<Transform>("Blank GameObject");
    addCreateGameObjectMenuItem<Camera>("Camera");
    addCreateGameObjectMenuItem<StaticMesh>("Static Mesh");
    addCreateGameObjectMenuItem<Terrain>("Terrain");
}

void SceneManager::frameStart()
{
    const float deltaTime = Clock::instance()->deltaTime();

    for (unsigned int i = 0; i < gameObjects_.size(); ++i)
    {
        gameObjects_[i]->update(deltaTime);
    }
}

GameObject* SceneManager::createGameObject(const std::string& name, Transform* parent)
{
    GameObject* go = new GameObject(name);
    if (parent != nullptr)
    {
        go->createComponent<Transform>()->setParentTransform(parent);
    }
    gameObjects_.push_back(go);

    return go;
}

GameObject* SceneManager::createGameObject(Prefab* prefab)
{
    GameObject* go = new GameObject(prefab);
    gameObjects_.push_back(go);
    return go;
}

Camera* SceneManager::mainCamera() const
{
    // Check every game object for a camera component
    for (unsigned int i = 0; i < gameObjects_.size(); ++i)
    {
        Camera* camera = gameObjects_[i]->findComponent<Camera>();
        if (camera != nullptr)
        {
            return camera;
        }
    }

    // No camera in the scene
    return nullptr;
}

const std::vector<StaticMesh*> SceneManager::staticMeshes() const
{
    // Make a vector to store the meshes
    std::vector<StaticMesh*> meshes;

    // Check every game object for a StaticMesh component
    for (unsigned int i = 0; i < gameObjects_.size(); ++i)
    {
        StaticMesh* mesh = gameObjects_[i]->findComponent<StaticMesh>();
        if (mesh != nullptr)
        {
            meshes.push_back(mesh);
        }
    }

    return meshes;
}

const std::vector<Terrain*> SceneManager::terrains() const
{
    // Make a vector to store the meshes
    std::vector<Terrain*> meshes;

    // Check every game object for a StaticMesh component
    for (unsigned int i = 0; i < gameObjects_.size(); ++i)
    {
        Terrain* mesh = gameObjects_[i]->findComponent<Terrain>();
        if (mesh != nullptr)
        {
            meshes.push_back(mesh);
        }
    }

    return meshes;
}

template<typename T>
void SceneManager::addCreateGameObjectMenuItem(const std::string &gameObjectName)
{
    MainWindowMenu::instance()->addMenuItem(
        "Scene/New GameObject/" + gameObjectName,
        [=] { PropertiesPanel::instance()->inspect(createGameObject(gameObjectName)->createComponent<T>()->gameObject()); }
    );

    // Add a second button, allowing the gameobject to be created as 
    // a child of the gameobject currently in the properties panel.
    MainWindowMenu::instance()->addMenuItem(
        "Scene/New Child GameObject/" + gameObjectName,
        [=] {
        GameObject* parentGO = dynamic_cast<GameObject*>(PropertiesPanel::instance()->current());
        Transform* parentTransform = (parentGO != nullptr) ? parentGO->transform() : nullptr;
        PropertiesPanel::instance()->inspect(createGameObject(gameObjectName, parentTransform)->createComponent<T>()->gameObject());
    });
}