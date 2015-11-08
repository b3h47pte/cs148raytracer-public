#pragma once

#include "common/Rendering/Renderer.h"
#include "common/Rendering/Renderer/Photon/Photon.h"
#include <kdtree++/kdtree.hpp>
#include <functional>
#include "common/Scene/Geometry/Mesh/MeshObject.h"

class PhotonMappingRenderer : public Renderer
{
public:
    PhotonMappingRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler);
    virtual void InitializeRenderer() override;
    glm::vec3 ComputeSampleColor(const struct IntersectionState& intersection, const class Ray& fromCameraRay) const override;

    void SetNumberOfDiffusePhotons(int diffuse);
    void SetNumberOfCasuticPhotons(int caustic);
private:
    using PhotonKdtree = KDTree::KDTree<3, Photon, PhotonAccessor>;
    PhotonKdtree diffuseMap;
    PhotonKdtree causticMap;

    int diffusePhotonNumber;
    int causticPhotonNumber;

    void GenericPhotonMapGeneration(PhotonKdtree& photonMap, int totalPhotons, std::function<bool(const class MeshObject&)> objectFilter);
};