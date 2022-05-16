#pragma once
#include <Ultralight/platform/GPUDriver.h>
#include "RenderWindow.h"
#include "Config.h"

#define ENABLE_OFFSCREEN_GL 1

typedef struct GLFWwindow GLFWwindow;

namespace UIEngine
{

class GPUDriverGL : public ultralight::GPUDriver
{
public:
    GPUDriverGL() = delete;
    GPUDriverGL(GLFWwindow* window, Config* config);

    virtual ~GPUDriverGL() { }


#if ENABLE_OFFSCREEN_GL
    virtual void SetRenderBufferBitmap(uint32_t render_buffer_id,
                                       ultralight::RefPtr<ultralight::Bitmap> bitmap);

    virtual bool IsRenderBufferBitmapDirty(uint32_t render_buffer_id);

    virtual void SetRenderBufferBitmapDirty(uint32_t render_buffer_id,
                                            bool dirty);
#endif

    virtual void CreateTexture(uint32_t texture_id,
                               ultralight::RefPtr<ultralight::Bitmap> bitmap) override;

    virtual void UpdateTexture(uint32_t texture_id,
                               ultralight::RefPtr<ultralight::Bitmap> bitmap) override;

    virtual void BindTexture(uint8_t texture_unit,
                             uint32_t texture_id);

    virtual void DestroyTexture(uint32_t texture_id) override;

    virtual void CreateRenderBuffer(uint32_t render_buffer_id,
                                    const ultralight::RenderBuffer& buffer) override;

    virtual void BindRenderBuffer(uint32_t render_buffer_id);

    virtual void ClearRenderBuffer(uint32_t render_buffer_id);

    virtual void DestroyRenderBuffer(uint32_t render_buffer_id) override;

    virtual void CreateGeometry(uint32_t geometry_id,
                                const ultralight::VertexBuffer& vertices,
                                const ultralight::IndexBuffer& indices) override;

    virtual void UpdateGeometry(uint32_t geometry_id,
                                const ultralight::VertexBuffer& vertices,
                                const ultralight::IndexBuffer& indices) override;

    virtual void DrawGeometry(uint32_t geometry_id,
                              uint32_t indices_count,
                              uint32_t indices_offset,
                              const ultralight::GPUState& state);

    virtual void DestroyGeometry(uint32_t geometry_id) override;

    virtual void DrawCommandList();
    virtual bool HasCommandsPending();
    virtual void BeginSynchronize() override;
    virtual void EndSynchronize() override;
    virtual void UpdateCommandList(const ultralight::CommandList& list) override;

    void BindUltralightTexture(uint32_t ultralight_texture_id);

    void LoadPrograms();
    void DestroyPrograms();

    void LoadProgram(ultralight::ShaderType type);
    void SelectProgram(ultralight::ShaderType type);
    void UpdateUniforms(const ultralight::GPUState& state);
    void SetUniform1ui(const char* name, GLuint val);
    void SetUniform1f(const char* name, float val);
    void SetUniform1fv(const char* name, size_t count, const float* val);
    void SetUniform4f(const char* name, const float val[4]);
    void SetUniform4fv(const char* name, size_t count, const float* val);
    void SetUniformMatrix4fv(const char* name, size_t count, const float* val);
    void SetViewport(uint32_t width, uint32_t height);

    virtual uint32_t NextTextureId() override;
    virtual uint32_t NextRenderBufferId() override;
    virtual uint32_t NextGeometryId() override;

protected:
    ultralight::Matrix ApplyProjection(const ultralight::Matrix4x4& transform, float screen_width, float screen_height,
                                       bool flip_y);

    void CreateFBOTexture(uint32_t texture_id, ultralight::RefPtr<ultralight::Bitmap> bitmap);

    struct TextureEntry
    {
        GLuint tex_id = 0; // GL Texture ID
        GLuint msaa_tex_id = 0; // GL Texture ID (only used if MSAA is enabled)
        uint32_t render_buffer_id = 0; // Used to check if we need to perform MSAA resolve
        GLuint width, height; // Used when resolving MSAA FBO, only valid if FBO
        bool is_sRGB = false; // Whether or not the primary texture is sRGB or not.
    };

    // Maps Ultralight Texture IDs to OpenGL texture handles
    std::map<uint32_t, TextureEntry> texture_map;

    struct GeometryEntry
    {
        // VAOs are not shared across GL contexts so we create them lazily for each
        std::map<GLFWwindow*, GLuint> vao_map;
        ultralight::VertexBufferFormat vertex_format;
        GLuint vbo_vertices = 0; // VBO id for vertices
        GLuint vbo_indices = 0; // VBO id for indices
    };
    std::map<uint32_t, GeometryEntry> geometry_map;

    struct FBOEntry
    {
        GLuint fbo_id = 0; // GL FBO ID (if MSAA is enabled, this will be used for resolve)
        GLuint msaa_fbo_id = 0; // GL FBO ID for MSAA
        bool needs_resolve = false; // Whether or not we need to perform MSAA resolve
    };

    struct RenderBufferEntry
    {
        // FBOs are not shared across GL contexts so we create them lazily for each
        std::map<GLFWwindow*, FBOEntry> fbo_map;
        uint32_t texture_id = 0; // The Ultralight texture ID backing this RenderBuffer.
#if ENABLE_OFFSCREEN_GL
        ultralight::RefPtr<ultralight::Bitmap> bitmap;
        GLuint pbo_id = 0;
        bool is_bitmap_dirty = false;
        bool is_first_draw = true;
        bool needs_update = false;
#endif
    };

    void CreateFBOIfNeededForActiveContext(uint32_t render_buffer_id);

    void CreateVAOIfNeededForActiveContext(uint32_t geometry_id);

    void ResolveIfNeeded(uint32_t render_buffer_id);

    void MakeTextureSRGBIfNeeded(uint32_t texture_id);

#if ENABLE_OFFSCREEN_GL
    void UpdateBitmap(RenderBufferEntry& entry, GLuint pbo_id);
#endif

    std::map<uint32_t, RenderBufferEntry> render_buffer_map;

    uint32_t next_texture_id_ = 1;
    uint32_t next_render_buffer_id_ = 1; // render buffer id 0 is reserved for default render target view.
    uint32_t next_geometry_id_ = 1;

    std::vector<ultralight::Command> command_list_;
    int batch_count_;

    struct ProgramEntry
    {
        GLuint program_id;
        GLuint vert_shader_id;
        GLuint frag_shader_id;
    };
    std::map<ultralight::ShaderType, ProgramEntry> programs_;
    GLuint cur_program_id_;

    GLFWwindow* _window;
    Config* _config;

    //ultralight::GPUContextGL* context_;
};

}
