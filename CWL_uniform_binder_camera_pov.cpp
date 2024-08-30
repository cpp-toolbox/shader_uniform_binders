#include "CWL_uniform_binder_camera_pov.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/**
 *
 * \brief binds matrices into uniforms given the shader program's id and a camera with yaw and pitch attributes
 *
 * \description Specifically world_to_camera translates the world coordinate system to have origin centered at the
 * characters position, facing in the direction that the character is looking
 *
 * It binds camera_to_clip to a basic perspective transformation
 *
 * It binds local_to_world as the identity, which can be overridden if needed
 *
 * \precondition the shader program uses a CWL transformation with attribute, opengl has been initialized
 * names:
 * - camera_to_clip
 * - world_to_camera
 * - local_to_world
 *
 * \param shader_program_id
 * \param screen_width
 * \param screen_height
 * \param character_position
 */
void bind_CWL_matrix_uniforms_camera_pov(ShaderCache &shader_cache, int screen_width, int screen_height,
                                         glm::vec3 character_position, glm::mat4 local_to_world, Camera camera,
                                         float fov, float render_distance) {

    shader_cache.use_shader_program(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES);

    glm::mat4 world_to_camera =
        glm::lookAt(character_position, character_position + camera.look_direction, camera.up_direction);
    glm::mat4 camera_to_clip =
        glm::perspective(glm::radians(fov), (float)screen_width / (float)screen_height, 0.1f, render_distance);

    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES, ShaderUniformVariable::LOCAL_TO_WORLD,
                             local_to_world);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES, ShaderUniformVariable::WORLD_TO_CAMERA,
                             world_to_camera);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES, ShaderUniformVariable::CAMERA_TO_CLIP,
                             camera_to_clip);

    bool temp = true;
    if (temp) {
        glm::vec3 origin(0.0f);
        glm::mat4 world_to_camera_no_translation =
            glm::lookAt(origin, origin + camera.look_direction, camera.up_direction);
        shader_cache.use_shader_program(ShaderType::SKYBOX);
        // wrong name
        shader_cache.set_uniform(ShaderType::SKYBOX, ShaderUniformVariable::WORLD_TO_CAMERA,
                                 world_to_camera_no_translation);
        shader_cache.set_uniform(ShaderType::SKYBOX, ShaderUniformVariable::CAMERA_TO_CLIP, camera_to_clip);
    }

    shader_cache.stop_using_shader_program();
}
