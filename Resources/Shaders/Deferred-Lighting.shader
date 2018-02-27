
#include "Common.inc.shader"
#include "UniformBuffers.inc.shader"

#define USE_SCREENSPACE_VERT_SHADER
#define USE_GBUFFER_READ
#include "Deferred.inc.shader"

#include "PhysicallyBasedShading.inc.shader"
#include "Shadows.inc.shader"

#include "Atmosphere.inc.shader"

#ifdef FRAGMENT_SHADER

out vec4 fragColor;

void main()
{
    // Sample the gBuffer textures
    SurfaceProperties surface = readGBuffer();

    // Retrive the world space position by sampling the depth buffer
    vec3 worldPosition = readGBufferWorldPos();

    // Compute the view vector and half vector from the world position
    vec3 viewDirUnnormalized = _CameraPosition.xyz - worldPosition;
    float viewDistance = length(viewDirUnnormalized);
    vec3 viewDir = viewDirUnnormalized / viewDistance;

    // Attenuate the direct light by the atmospheric scattering
    vec3 sunDir = _LightDirectionIntensity.xyz;
    vec3 sunColor = _LightDirectionIntensity.w * TDirection(worldPosition + vec3(0.0, Rg, 0.0), _LightDirectionIntensity.xyz);

    // Compute the ambient and direct light separately
    vec3 ambientLight = surface.diffuseColor * _AmbientColor.rgb * 0.2;
    vec3 directLight = PhysicallyBasedBRDF(surface, sunColor, sunDir, viewDir);

    // Attenuate the direct light by a shadow factor
#ifdef SHADOWS_ON
    directLight *= SampleSunShadow(worldPosition, viewDistance);
#endif

    // Combine the lighting components
    vec3 light = ambientLight + directLight;

    // Add light from in-scattering
    vec3 inScattering = _LightDirectionIntensity.w * 5.0 * InScatteringPointToPoint(_CameraPosition.xyz + vec3(0.0, Rg, 0.0), worldPosition + vec3(0.0, Rg, 0.0));
    inScattering = max(inScattering, 0.0);

    // Add fog based on view distance.
#ifdef FOG_ON
    float fogDensity = computeVolumetricFog(_CameraPosition.xyz, -viewDirUnnormalized, viewDistance);
    light = applyVolumetricFog(light, fogDensity, inScattering);
#endif

    fragColor = vec4(light, 0.0);
}

#endif // FRAGMENT_SHADER