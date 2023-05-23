#include "sprite.h"

Sprite* create_sprite(xpm_map_t pic, uint16_t x, uint16_t y, int16_t xspeed, int16_t yspeed) {
    Sprite* sp = (Sprite*)malloc(sizeof(Sprite));

    if (sp == NULL) return NULL;

    xpm_image_t img;
    uint32_t* colormap = (uint32_t*)xpm_load(pic, XPM_8_8_8_8, &img);

    if (colormap == NULL) {
        free(sp);
        return NULL;
    }

    sp->x = x;
    sp->y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    sp->width = img.width;
    sp->height = img.height;
    sp->map = colormap;

    return sp;
}

int load_sprites() {
    background = create_sprite((xpm_map_t)background_xpm, 0, 0, 0, 0);
    cursor = create_sprite((xpm_map_t)cursor_xpm, 0, 0, 0, 0);
    logo = create_sprite((xpm_map_t)logo_xpm, 0, 20, 0, 0);
    play_text = create_sprite((xpm_map_t)play_xpm, 350, 250, 0, 0);
    play_texth = create_sprite((xpm_map_t)playh_xpm, 350, 250, 0, 0);
    settings_text = create_sprite((xpm_map_t)settings_xpm, 100, 450, 0, 0);
    settings_texth = create_sprite((xpm_map_t)settingsh_xpm, 100, 450, 0, 0);
    exit_text = create_sprite((xpm_map_t)exit_xpm, 350, 650, 0, 0);
    exit_texth = create_sprite((xpm_map_t)exith_xpm, 350, 650, 0, 0);
    fireboy = create_sprite((xpm_map_t)fireboy_xpm, 300, 400, 0, 0);
    watergirl = create_sprite((xpm_map_t)watergirl_xpm, 500, 400, 0, 0);

    return 0;
}

int destroy_sprite(Sprite* sp) {
    if (!sp) return 1;

    if (sp->map) free(sp->map);

    free(sp);
    return 0;
}

int draw_sprite(Sprite* sp) {
    if (!sp) return 1;

    for (int h = 0; h < sp->height; h++) {
        for (int w = 0; w < sp->width; w++) {
            uint32_t pixel = sp->map[h * sp->width + w];
            if (pixel == xpm_transparency_color(XPM_8_8_8_8)) continue;
            video_draw_pixel(sp->x + w, sp->y + h, pixel);
        }
    }

    return 0;
}

int erase_sprite(Sprite* sp) {
    if (!sp) return 1;

    for (int h = 0; h < sp->height; h++) {
        for (int w = 0; w < sp->width; w++) {
            video_draw_pixel(sp->x + w, sp->y + h, 0);
        }
    }

    return 0;
}

int move_sprite(Sprite* sp) {
    if (!sp) return 1;

    sp->x += sp->xspeed;
    sp->y += sp->yspeed;

    return 0;
}

int animate_sprite(Sprite* sp) {
    if (!sp) return 1;

    if (move_sprite(sp)) return 1;

    if (draw_sprite(sp)) return 1;

    return 0;
}
