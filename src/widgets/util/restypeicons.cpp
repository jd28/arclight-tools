#include "restypeicons.h"

#include "ZFontIcon/ZFontIcon.h"
#include "ZFontIcon/ZFont_fa6.h"

#include <QColor>

QIcon restypeToIcon(nw::ResourceType::type type)
{
    auto color = QColor(42, 130, 218);

    switch (type) {
    default:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_file, color);

    // Config
    case nw::ResourceType::twoda:
    case nw::ResourceType::tml:
    case nw::ResourceType::ini:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_file_invoice, color);

    case nw::ResourceType::key:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_key, color);
    case nw::ResourceType::hak:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_archive, color);
    case nw::ResourceType::tlk:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_comments, color);

    case nw::ResourceType::ttf:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_font, color);

    // Code
    case nw::ResourceType::ncs:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_gear, color);
    case nw::ResourceType::nss:
    case nw::ResourceType::shd:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_file_code, color);

    // Audio / Video
    case nw::ResourceType::wbm:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_file_video, color);
    case nw::ResourceType::bmu:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_file_audio, color);
    case nw::ResourceType::wav:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_file_audio, color);

    // Objects
    case nw::ResourceType::utc:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_ghost, color);
    case nw::ResourceType::utd:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_door_open, color);
    case nw::ResourceType::uti:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_shield_halved, color);
    case nw::ResourceType::dlg:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_comments, color);

    // Areas
    case nw::ResourceType::caf:
    case nw::ResourceType::are:
    case nw::ResourceType::gic:
    case nw::ResourceType::git:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_map, color);

    // Models, Textures
    case nw::ResourceType::mdl:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_cube, color);
    case nw::ResourceType::dds:
    case nw::ResourceType::tga:
    case nw::ResourceType::plt:
        return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_file_image, color);
    }
}
