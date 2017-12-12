#include "GameObject.h"

#include "SceneManager.h"

#include "Scene/Component.h"
#include "Scene/Transform.h"
#include "Scene/Terrain.h"

GameObject::GameObject(const std::string &name)
    : name_(name)
{
    // Give every GameObject instance a transform component
    // This ensures that gameobject can be parented inside each other.
    createComponent<Transform>();
}

void GameObject::serialize(BitWriter& writer) const
{
    
}

void GameObject::deserialize(BitReader& reader)
{
    
}

void GameObject::update(float deltaTime)
{
    for(unsigned int i = 0; i < components_.size(); ++i)
    {
        components_[i]->update(deltaTime);
    }
}

Transform* GameObject::transform() const
{
    return findComponent<Transform>();
}

Camera* GameObject::camera() const
{
    return findComponent<Camera>();
}

StaticMesh* GameObject::staticMesh() const
{
    return findComponent<StaticMesh>();
}

Terrain* GameObject::terrain() const
{
	return findComponent<Terrain>();
}
