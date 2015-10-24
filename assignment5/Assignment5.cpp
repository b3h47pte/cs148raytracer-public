#include "assignment5/Assignment5.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment5::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 45.f);
    camera->SetPosition(glm::vec3(0.f, 0.f, 15.f));
    return camera;
}

std::shared_ptr<Scene> Assignment5::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

    // Material
    std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
    cubeMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    cubeMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);

    // Objects
    std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("cube.obj");
    for (size_t i = 0; i < cubeObjects.size(); ++i) {
        cubeObjects[i]->SetMaterial(cubeMaterial->Clone());
    }

    std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
    cubeSceneObject->AddMeshObject(cubeObjects);
    cubeSceneObject->MultScale(0.5f);
    newScene->AddSceneObject(cubeSceneObject);

    std::shared_ptr<SceneObject> cubeSceneObject2 = std::make_shared<SceneObject>();
    cubeSceneObject2->AddMeshObject(cubeObjects);
    cubeSceneObject2->Translate(glm::vec3(6.f, 0.f, 0.f));
    cubeSceneObject2->MultScale(0.5f);
    newScene->AddSceneObject(cubeSceneObject2);

    std::shared_ptr<SceneObject> cubeSceneObject3 = std::make_shared<SceneObject>();
    cubeSceneObject3->AddMeshObject(cubeObjects);
    cubeSceneObject3->Translate(glm::vec3(-6.f, 0.f, 0.f));
    cubeSceneObject3->MultScale(0.5f);
    newScene->AddSceneObject(cubeSceneObject3);

    // Lights
    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3(10.f, 10.f, 10.f));
    pointLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));
    newScene->AddLight(pointLight);

    std::shared_ptr<Light> sunLight = std::make_shared<DirectionalLight>();
    sunLight->Rotate(glm::vec3(SceneObject::GetWorldRight()), -PI / 4.f);

    sunLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));
    newScene->AddLight(sunLight);

    return newScene;
}

std::shared_ptr<ColorSampler> Assignment5::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> sampler = std::make_shared<JitterColorSampler>();
    sampler->SetGridSize(glm::ivec3(1, 1, 1));
    return sampler;
}

std::shared_ptr<class Renderer> Assignment5::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
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