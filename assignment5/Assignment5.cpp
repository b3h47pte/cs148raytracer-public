#include "assignment5/Assignment5.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment5::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 75.f);
    camera->SetPosition(glm::vec3(0.f, 0.f, 15.f));
    return camera;
}

std::shared_ptr<Scene> Assignment5::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

    // Material
    std::shared_ptr<BlinnPhongMaterial> sphereMaterial = std::make_shared<BlinnPhongMaterial>();
    sphereMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    sphereMaterial->SetSpecular(glm::vec3(0.f, 0.f, 0.f), 40.f);

    // Objects
    std::vector<std::shared_ptr<MeshObject>> sphereObjects = MeshLoader::LoadMesh("cube.obj");
    for (size_t i = 0; i < sphereObjects.size(); ++i) {
        sphereObjects[i]->SetMaterial(sphereMaterial);
    }

    std::shared_ptr<SceneObject> sphereSceneObject = std::make_shared<SceneObject>();
    sphereSceneObject->AddMeshObject(sphereObjects);
    newScene->AddSceneObject(sphereSceneObject);

    // Lights
    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3(10.f, 10.f, 10.f));
    pointLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));
    newScene->AddLight(pointLight);

    return newScene;
}

std::shared_ptr<class Sampler> Assignment5::CreateSampler() const
{
    return std::make_shared<RandomSampler>();
}

std::shared_ptr<class Renderer> Assignment5::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Sampler> sampler) const
{
    return std::make_shared<ForwardRenderer>(scene, sampler);
}

int Assignment5::GetSamplesPerPixel() const
{
    return 1;
}

bool Assignment5::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment5::GetMaxReflectionBounces() const
{
    return 0;
}

int Assignment5::GetMaxRefractionBounces() const
{
    return 0;
}