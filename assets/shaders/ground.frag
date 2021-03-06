#version 450
precision mediump float;

#include "inc/render_target.h"

layout(location = 0) in mediump vec3 vEyeVec;

layout(push_constant, std430) uniform Constants
{
    mat4 Model;
    mat4 Normal;
} registers;

layout(location = 1) in highp vec2 vUV;

layout(set = 2, binding = 1) uniform sampler2D uNormalsTerrain;
layout(set = 2, binding = 2) uniform sampler2D uOcclusionTerrain;
layout(set = 2, binding = 4) uniform mediump sampler2DArray uBaseColor;
layout(set = 2, binding = 5) uniform sampler2D uSplatMap;
layout(set = 2, binding = 6) uniform sampler2D uDeepRoughNormals;

layout(std140, set = 3, binding = 1) uniform GroundData
{
    vec2 uInvHeightmapSize;
    vec2 uUVShift;
    vec2 uUVTilingScale;
    vec2 uTangentScale;
};

float horiz_max(vec4 v)
{
    vec2 x = max(v.xy, v.zw);
    return max(x.x, x.y);
}

void main()
{
    highp vec2 uv = vUV * uUVTilingScale;

    vec4 types = vec4(textureLod(uSplatMap, vUV, 0.0).rgb, 0.25);
    float max_weight = horiz_max(types);
    types = types / max_weight;
    types = clamp(2.0 * (types - 0.5), vec4(0.0), vec4(1.0));
    float weight = 1.0 / dot(types, vec4(1.0));
    types *= weight;

    const float lod = 0.5;
    vec3 base_color =
        types.x * texture(uBaseColor, vec3(uv, 0.0), lod).rgb +
        types.y * texture(uBaseColor, vec3(uv, 1.0), lod).rgb +
        types.z * texture(uBaseColor, vec3(uv, 2.0), lod).rgb +
        types.w * texture(uBaseColor, vec3(uv, 3.0), lod).rgb;

    vec3 terrain = texture(uNormalsTerrain, vUV).xyz * 2.0 - 1.0;
    terrain.xy += types.w * 0.5 * (texture(uDeepRoughNormals, uv).xy * 2.0 - 1.0);
    vec3 normal = normalize(mat3(registers.Normal) * terrain.xzy); // Normal is +Y, Bitangent is +Z.

    emit_render_target(vec3(0.0), vec4(base_color, 1.0), normal, 0.0, 1.0, texture(uOcclusionTerrain, vUV).x, vEyeVec);
}

