#define zox_texture_none 0
#define zox_texture_dirt 1
#define zox_texture_grass 2
#define zox_texture_sand 3
#define zox_texture_stone 4
#define zox_texture_obsidian 5

void generate_texture_graybox(TextureData* textureData, const TextureSize *textureSize, int2 position, int2 size) {
    for (int j = position.x; j < position.x + size.x; j++) {
        for (int k = position.y; k < position.y + size.y; k++) {
            int index = j + k * textureSize->value.x;
            if (j == position.x || k == position.y || j == position.x + size.x - 1 || k == position.y + size.y - 1) {
                textureData->value[index].r = 0;
                textureData->value[index].g = 0;
                textureData->value[index].b = 0;
            } else {
                textureData->value[index].r = 125;
                textureData->value[index].g = 125;
                textureData->value[index].b = 125;
            }
        }
    }
}

void generate_texture_noise(TextureData* textureData, const TextureSize *textureSize, unsigned char texture_type) {
    int2 redRange = { 15, 244 };
    int2 greenRange = { 15, 122 };
    int2 blueRange = { 15, 122 };
    int2 alphaRange = { 144, 256 };
    if (texture_type != zox_texture_none) { // }== zox_texture_dirt) {
        redRange = (int2) { 53, 93 };  // 73
        greenRange = (int2) { 37, 57 };  // 47
        blueRange = (int2) { 7, 27 };  // 17
        alphaRange = (int2) { 255, 256 };
        // if (rand() % 100 >= 50) {
        if (texture_type == zox_texture_grass) {
            //zoxel_log(" > grass texture created\n");
            greenRange.x *= 2;
            greenRange.y *= 2;
            blueRange.x = redRange.x - 30;
            blueRange.y = redRange.y - 30;
            redRange.x /= 2;
            redRange.y /= 2;
        } else if (texture_type == zox_texture_sand) {
            // zoxel_log(" > sand texture created\n");
            redRange.x = redRange.y = 206 - 50;
            greenRange.x = greenRange.y = 179 - 50;
            blueRange.x = blueRange.y = 59 - 30;
            redRange.x -= 10 + rand() % 30;
            greenRange.x -= 10 + rand() % 30;
            blueRange.x -= 10 + rand() % 20;
        } else if (texture_type == zox_texture_stone) {
            // zoxel_log(" > sand texture created\n");
            redRange.x = redRange.y = 100;
            greenRange.x = greenRange.y = 100;
            blueRange.x = blueRange.y = 100;
            redRange.x -= 10 + rand() % 20;
            greenRange.x -= 10 + rand() % 20;
            blueRange.x -= 10 + rand() % 20;
        } else if (zox_texture_obsidian) {
            redRange.x = redRange.y = 42;
            greenRange.x = greenRange.y = 42;
            blueRange.x = blueRange.y = 42;
            redRange.x -= 6 + rand() % 12;
            greenRange.x -= 6 + rand() % 12;
            blueRange.x -= 6 + rand() % 12;
        }
    }
    #ifdef zox_grayboxing
        generate_texture_graybox(textureData, textureSize, (int2) { 0, 0 }, (int2) { textureSize->value.x / 2, textureSize->value.y / 2 });
        generate_texture_graybox(textureData, textureSize, (int2) { textureSize->value.x / 2, 0 }, (int2) { textureSize->value.x / 2, textureSize->value.y / 2 });
        generate_texture_graybox(textureData, textureSize, (int2) { textureSize->value.x / 2, textureSize->value.y / 2 }, (int2) { textureSize->value.x / 2, textureSize->value.y / 2 });
        generate_texture_graybox(textureData, textureSize, (int2) { 0, textureSize->value.y / 2 }, (int2) { textureSize->value.x / 2, textureSize->value.y / 2 });
    #else
    for (int j = 0; j < textureSize->value.x; j++) {
        for (int k = 0; k < textureSize->value.y; k++) {
            int index = j + k * textureSize->value.x;
            if (texture_type == zox_texture_none) {
                int distanceToMidX = abs_integer(textureSize->value.x / 2 - j);
                int distanceToMidY = abs_integer(textureSize->value.y / 2 - k);
                if (distanceToMidX + distanceToMidY >= textureSize->value.x / 2) {
                    textureData->value[index].r = 0;
                    textureData->value[index].g = 0;
                    textureData->value[index].b = 0;
                    textureData->value[index].a = 0;
                    continue;
                }
            }
            textureData->value[index].r = redRange.x + rand() % (redRange.y - redRange.x);
            textureData->value[index].g = greenRange.x + rand() % (greenRange.y - greenRange.x);
            textureData->value[index].b = blueRange.x + rand() % (blueRange.y - blueRange.x);
            textureData->value[index].a = alphaRange.x + rand() % (alphaRange.y - alphaRange.x);
            /*if (j >= textureSize->value.x / 2 && k < textureSize->value.y / 2) {
                textureData->value[index].g *= 2;
                textureData->value[index].b = textureData->value[index].r - 30;
                textureData->value[index].r /= 2;
            }*/
            // textureData->value[index].a = rand() % 256;
            // debug sides of textureData, starts at top left
            if (texture_type == zox_texture_none) {
                if (j == 0) {
                    textureData->value[index].r = 255;
                } else if (k == 0) {
                    textureData->value[index].g = 255;
                } else if (j == textureSize->value.x - 1) {
                    textureData->value[index].b = 255;
                } else if (k == textureSize->value.y - 1) {
                    textureData->value[index].r = 255;
                    textureData->value[index].g = 255;
                }
            } else if (is_texture_outlines == 1) {
                // outline voxel textures
                if (j == 0 || k == 0 || j == textureSize->value.x - 1 || k == textureSize->value.y - 1) {
                    textureData->value[index].r = 0;
                    textureData->value[index].g = 0;
                    textureData->value[index].b = 0;
                }
            } else if (is_texture_outlines == 2) {
                if (j == 0 || k == 0 || j == textureSize->value.x - 1 || k == textureSize->value.y - 1) {
                    textureData->value[index].r /= 2;
                    textureData->value[index].g /= 2;
                    textureData->value[index].b /= 2;
                }
            } else if (is_texture_outlines == 3) {
                if (j == 0 || k == 0 || j == textureSize->value.x - 1 || k == textureSize->value.y - 1) {
                    textureData->value[index].r *= 5;
                    textureData->value[index].g *= 5;
                    textureData->value[index].b *= 5;
                    textureData->value[index].r /= 6;
                    textureData->value[index].g /= 6;
                    textureData->value[index].b /= 6;
                }
            }
            // printf("textureData value: %i\n", textureData->value[index].r);
        }
    }
    #endif
}

void NoiseTextureSystem(ecs_iter_t *it) {
    if (!ecs_query_changed(it->ctx, NULL)) return;
    TextureDirty *textureDirtys = ecs_field(it, TextureDirty, 2);
    TextureData *textures = ecs_field(it, TextureData, 3);
    const TextureSize *textureSizes = ecs_field(it, TextureSize, 4);
    const GenerateTexture *generateTextures = ecs_field(it, GenerateTexture, 5);
    for (int i = 0; i < it->count; i++) {
        const GenerateTexture *generateTexture = &generateTextures[i];
        if (generateTexture->value == 0) continue;
        TextureDirty *textureDirty = &textureDirtys[i];
        if (textureDirty->value != 0) continue;
        TextureData *textureData = &textures[i];
        const TextureSize *textureSize = &textureSizes[i];
        unsigned char texture_type = zox_texture_none;
        if (ecs_has(it->world, it->entities[i], DirtTexture)) {
            texture_type = zox_texture_dirt;
        } else if (ecs_has(it->world, it->entities[i], GrassTexture)) {
            texture_type = zox_texture_grass;
        } else if (ecs_has(it->world, it->entities[i], SandTexture)) {
            texture_type = zox_texture_sand;
        } else if (ecs_has(it->world, it->entities[i], StoneTexture)) {
            texture_type = zox_texture_stone;
        } else if (ecs_has(it->world, it->entities[i], ObsidianTexture)) {
            texture_type = zox_texture_obsidian;
        }
        re_initialize_memory_component(textureData, color, textureSize->value.x * textureSize->value.y);
        generate_texture_noise(textureData, textureSize, texture_type);
        textureDirty->value = 1;
        // if (is_dirt) zoxel_log("    > dirt generated [%lu]\n", it->entities[i]);
    }
} zox_declare_system(NoiseTextureSystem)

//! This breaks the updates. \todo Fix this. Find out why it doesn't work properly.
//! This doesn't work because the table writes all changes in the first iteration which is for one entity.
//!     Possible fix: Make a second system that writes changes at the end of the loop