#include "assignment8/Assignment8.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment8::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 26.6f);
    camera->SetPosition(glm::vec3(0.f, -4.1469f, 0.73693f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    return camera;
}

std::shared_ptr<Scene> Assignment8::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

    // Material
    std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
    cubeMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    cubeMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);

    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("CornellBox/CornellBox-Photon.obj", &loadedMaterials);
    for (size_t i = 0; i < cubeObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
        cubeObjects[i]->SetMaterial(materialCopy);
    }

    std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
    cubeSceneObject->AddMeshObject(cubeObjects);
    cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
    newScene->AddSceneObject(cubeSceneObject);

    // Lights
    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.77640f));
    pointLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));
    newScene->AddLight(pointLight);

    return newScene;

}
std::shared_ptr<ColorSampler> Assignment8::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> jitter = std::make_shared<JitterColorSampler>();
    jitter->SetGridSize(glm::ivec3(1, 1, 1));
    return jitter;
}

std::shared_ptr<class Renderer> Assignment8::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
{
    return std::make_shared<PhotonMappingRenderer>(scene, sampler);
}

int Assignment8::GetSamplesPerPixel() const
{
    return 1;
}

bool Assignment8::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment8::GetMaxReflectionBounces() const
{
    return 0;
}

int Assignment8::GetMaxRefractionBounces() const
{
    return 0;
}

glm::vec2 Assignment8::GetImageOutputResolution() const
{
    return glm::vec2(640.f, 480.f);
}
