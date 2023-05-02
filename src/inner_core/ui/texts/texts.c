#ifndef zoxel_texts
#define zoxel_texts

const double zext_animation_speed = 10.0;
zoxel_declare_tag(FontStyle)                    //! Contains a bunch of fonts!
zoxel_declare_tag(Font)                         //! A basic tag for a UI Element.
zoxel_declare_tag(Zigel)                        //! An individual text character entity.
zoxel_declare_tag(FontTexture)                  //! A basic tag for a Font TextureData. Generated on a Zigel.
zoxel_declare_tag(Zext)                         //! Holds all the zigels.
zoxel_component(ZigelIndex, unsigned char)      //! A character index per zigel.
zoxel_component(ZextSize, int)                  //! The size of the font in a Zext.
zoxel_component(ZextDirty, unsigned char)       //! A state event for when Zext is dirty.
zoxel_component(AnimateZext, double)            //! A Zext that animates.
zoxel_memory_component(FontData, byte2)         //! An array of points used for generating a font texture
zoxel_memory_component(ZextData, unsigned char) //! An array of bytes for characters.
#include "util/default_font.c"
#include "prefabs/font.c"
#include "prefabs/font_style.c"
#include "prefabs/zigel.c"
#include "util/zext_util.c"
#include "prefabs/zext.c"
#include "systems/font_texture_system.c"
#include "systems/zext_update_system.c"
#include "systems/animate_text_system.c"
zoxel_reset_system(ZextDirtyResetSystem, ZextDirty)

zoxel_begin_module(Texts)
zoxel_define_tag(Font)
zoxel_define_tag(Zigel)
zoxel_define_tag(FontTexture)
zoxel_define_tag(Zext)
zoxel_define_tag(FontStyle)
zoxel_define_component(ZigelIndex)
zoxel_define_component(ZextSize)
zoxel_define_component(ZextDirty)
zoxel_define_component(AnimateZext)
zoxel_define_memory_component(FontData)
zoxel_define_memory_component(ZextData)
spawn_font_style_prefab(world);
spawn_font_prefab(world);
spawn_zigel_prefab(world);
spawn_zext_prefab(world);
zoxel_filter(zextDirtyQuery, world, [none] Zext, [in] ZextDirty)
zoxel_filter(generateTextureQuery, world, [none] FontTexture, [in] GenerateTexture)
// zoxel_system_1(AnimateTextSystem, EcsOnUpdate, [out] AnimateZext, [out] ZextDirty, [out] ZextData)
zoxel_system(AnimateTextSystem, EcsOnUpdate, [out] AnimateZext, [out] ZextDirty, [out] ZextData)
#ifdef main_thread_zext_update_system
    zoxel_system_ctx_1(ZextUpdateSystem, EcsOnUpdate, zextDirtyQuery, [none] Zext, [in] ZextDirty,
        [in] ZextData, [in] ZextSize, [in] Layer2D, [in] Position2D, [in] PixelSize, [out] Children)
#else
    zoxel_system_ctx(ZextUpdateSystem, EcsOnUpdate, zextDirtyQuery, [none] Zext, [in] ZextDirty,
        [in] ZextData, [in] ZextSize, [in] Layer2D, [in] Position2D, [in] PixelSize, [out] Children)
#endif
zoxel_system_ctx(FontTextureSystem, EcsPreStore, generateTextureQuery, [none] FontTexture, [out] TextureDirty,
    [out] TextureData, [in] TextureSize, [in] GenerateTexture, [in] ZigelIndex, [in] Color)
zoxel_define_reset_system(ZextDirtyResetSystem, ZextDirty)
zoxel_end_module(Texts)

// \todo Display a UI Element anchored, with a pixel position.
// \todo Change colour when ray hits a button.

#endif