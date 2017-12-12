#include "Transform.h"

#include <imgui.h>

Transform::Transform(GameObject* gameObject)
    : Component(gameObject),
    position_(Point3::origin()),
    rotation_(Quaternion::identity()),
    scale_(Vector3::one()),
    localToWorld_(Matrix4x4::identity()),
    worldToLocal_(Matrix4x4::identity())
{

}

void Transform::drawProperties()
{
    ImGui::DragFloat3("Position", &position_.x, 0.1f);
    ImGui::DragFloat4("Rotation", (float*)&rotation_, 0.01f, -1.0f, 1.0f);
    ImGui::DragFloat3("Scale", &scale_.x, 0.1f);

    // The rotation quat needs to be re-normalized.
    rotation_.normalize();

    // The matrices may be changes by the above editing
    // Recompute them every time the properties editor is shown.
    recomputeMatrices();
}

void Transform::serialize(PropertyTable &table)
{
    table.serialize<Point3>("Position", position_, Point3::origin());
    table.serialize<Quaternion>("Rotation", rotation_, Quaternion::identity());
    table.serialize<Vector3>("Scale", scale_, Vector3::one());

    // The matrices may be changed by the above editing
    // Recompute them every time the properties editor is shown.
    recomputeMatrices();
}

Point3 Transform::positionWorld() const
{
    // There is no gamneobject parenting yet
    // world space = local space for objects with no parent
    return positionLocal();
}

Quaternion Transform::rotationWorld() const
{
    // There is no gamneobject parenting yet
    // world space = local space for objects with no parent
    return rotationLocal();
}

Vector3 Transform::scaleWorld() const
{
    // There is no gamneobject parenting yet
    // world space = local space for objects with no parent
    return scaleLocal();
}

Vector3 Transform::left() const
{
    return rotationWorld() * Vector3::left();
}

Vector3 Transform::right() const
{
    return rotationWorld() * Vector3::right();
}

Vector3 Transform::forwards() const
{
    return rotationWorld() * Vector3::forwards();
}

Vector3 Transform::backwards() const
{
    return rotationWorld() * Vector3::backwards();
}

Vector3 Transform::up() const
{
    return rotationWorld() * Vector3::up();
}

Vector3 Transform::down() const
{
    return rotationWorld() * Vector3::down();
}

void Transform::setPositionLocal(const Point3& pos)
{
    position_ = pos;
    recomputeMatrices();
}

void Transform::setRotationLocal(const Quaternion& rot)
{
    rotation_ = rot;
    recomputeMatrices();
}

void Transform::setScaleLocal(const Vector3& scale)
{
    scale_ = scale;
    recomputeMatrices();
}

void Transform::translateLocal(const Vector3& translation)
{
    position_ += translation;
    recomputeMatrices();
}

void Transform::rotateLocal(float angle, const Vector3& axis)
{
    // Construct quaternion using input angle and axis
    const Quaternion newRotation = Quaternion::rotation(angle, axis);

    // Apply new rotation to existing local space rotation
    rotation_ = newRotation * rotation_;
}

void Transform::recomputeMatrices()
{
    localToWorld_ = Matrix4x4::trs(position_, rotation_, scale_);
    worldToLocal_ = Matrix4x4::trsInverse(position_, rotation_, scale_);
}
