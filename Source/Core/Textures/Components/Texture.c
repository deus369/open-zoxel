// #include <cstdint> ? https://stackoverflow.com/questions/20024690/is-there-byte-data-type-in-c
//! A texture with pixels!
typedef struct
{
    color *value;
} Texture;
ECS_COMPONENT_DECLARE(Texture);


/*

You could use the same change tracking feature that you've used before
If the Texture field is inout/out, it will automatically get flagged as dirty when you iterate it
Then in a system/query that reads the texture, you can use ecs_query_changed

*/