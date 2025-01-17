extern unsigned char fullscreen;
extern unsigned char halfscreen;
extern unsigned char is_split_screen;
extern unsigned char vsync;
extern unsigned char override_opengl_es;
extern unsigned char is_using_vulkan;
extern unsigned char game_rule_attach_to_character;
extern unsigned char headless;
extern unsigned char is_multithreading;
extern unsigned char profiler;
extern unsigned char server_mode;
extern unsigned char target_fps;
#ifdef zox_mod_voxels
    #define terrain_mode_tiny 1
    #define terrain_mode_medium 2
    #define terrain_mode_large 3
    extern unsigned char terrain_mode;
    extern unsigned char disable_block_voxes;
#endif
#ifdef zox_mod_characters
    extern unsigned char disable_npcs;
#endif

void print_help_menu(const char* arg0) {
    zoxel_log("\n");
    zoxel_log("-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-\n");
    zoxel_log("\n");
    zoxel_log("Welcome to Zoxel Help Menu\n\n");
    zoxel_log("    Usage: %s [options]\n", arg0);
    zoxel_log("\n");
    zoxel_log("    Options:\n");
    zoxel_log("\n");
    zoxel_log("        -h --help            print this help\n");
    zoxel_log("        -z --headless        enable headless\n");
    zoxel_log("        -v --vulkan          enables vulkan\n");
    zoxel_log("        -w --windowed        windowed mode\n");
    zoxel_log("        -f --fullscreen      fullscreen window\n");
    zoxel_log("        -g --halfscreen      halfscreen window\n");
    zoxel_log("        -s --splitscreen     split screen local coop\n");
    zoxel_log("        -p --profiler        enable profiler\n");
    zoxel_log("        -x --server          enable server mode\n");
    zoxel_log("        -e --opengles        enables opengl es\n");
    zoxel_log("        -b --vsync           enable vsync\n");
    zoxel_log("        --singlethread       run with only one thread\n");
    zoxel_log("\n");
    zoxel_log("    Extras:\n");
    zoxel_log("\n");
    zoxel_log("        --freeroam           freeroam camera on game start\n");
    zoxel_log("        --tiny               set game terrain size to 2\n");
    zoxel_log("        --medium             set game terrain size to 6\n");
    zoxel_log("        --large              set game terrain size to 12\n");
    zoxel_log("\n");
    zoxel_log("-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-\n");
    zoxel_log("\n");
}

int process_arguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help_menu(argv[0]);
            return EXIT_FAILURE;
        } else if (strcmp(argv[i], "--fps") == 0) {
            target_fps = (unsigned char) (atoi(argv[i + 1]));
            i++;
        } else if (strcmp(argv[i], "--singlethread") == 0) {
            is_multithreading = 0;
        } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--windowed") == 0) {
            fullscreen = 0;
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fullscreen") == 0) {
            fullscreen = 1;
        } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--halfscreen") == 0) {
            halfscreen = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--splitscreen") == 0) {
            is_split_screen = 1;
        } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--vsync") == 0) {
            vsync = 1;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--profiler") == 0) {
            profiler = 1;
        } else if (strcmp(argv[i], "-z") == 0 || strcmp(argv[i], "--headless") == 0) {
            headless = 1;
        } else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--server") == 0) {
            server_mode = 1;
        } else if (strcmp(argv[i], "--client") == 0) {
            server_mode = 0;
        } else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--server") == 0) {
            server_mode = 1;
        } else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--opengles") == 0) {
            override_opengl_es = 1;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--vulkan") == 0) {
            is_using_vulkan = 1;
        }
        else if (strcmp(argv[i], "--freeroam") == 0) {
            game_rule_attach_to_character = 0;
        }
#ifdef zox_mod_voxels
        else if (strcmp(argv[i], "--tiny") == 0) {
            terrain_mode = terrain_mode_tiny;
            zox_log(" + tiny mode enabled\n")
        } else if (strcmp(argv[i], "--medium") == 0) {
            terrain_mode = terrain_mode_medium;
        } else if (strcmp(argv[i], "--large") == 0) {
            terrain_mode = terrain_mode_large;
        }
        else if (strcmp(argv[i], "--noblockvoxes") == 0) {
            disable_block_voxes = 1;
        }
#endif
#ifdef zox_mod_characters
        else if (strcmp(argv[i], "--nonpc") == 0 || strcmp(argv[i], "--nonpcs") == 0) {
            disable_npcs = 1;
        }
#endif
    }
    return EXIT_SUCCESS;
}
