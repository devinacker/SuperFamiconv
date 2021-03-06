// mode-specific defaults, constraints and functions
//
// david lindecrantz <optiroc@gmail.com>

#pragma once

#include "Common.h"

namespace sfc {

enum class Mode {
  none,
  snes,
  snes_mode7,
  gb,
  gbc,
  pce,
  pce_sprite,
};

inline Mode mode(const std::string& str) {
  if (str == "snes") {
    return Mode::snes;
  } else if (str == "snes_mode7") {
    return Mode::snes_mode7;
  } else if (str == "gb") {
    // TODO: GB
    // return Mode::gb;
    return Mode::gbc;
  } else if (str == "gbc") {
    return Mode::gbc;
  } else if (str == "pce") {
    return Mode::pce;
  } else if (str == "pce_sprite") {
    return Mode::pce_sprite;
  }
  return Mode::none;
}

inline std::string mode(Mode mode) {
  switch (mode) {
  case Mode::snes:
    return std::string("snes");
  case Mode::snes_mode7:
    return std::string("snes_mode7");
  case Mode::gb:
    // TODO: GB
    // return std::string("gb");
  case Mode::gbc:
    return std::string("gbc");
  case Mode::pce:
    return std::string("pce");
  case Mode::pce_sprite:
    return std::string("pce_sprite");
  default:
    return std::string("none");
  }
}

constexpr unsigned default_bpp_for_mode(Mode mode) {
  switch (mode) {
  case Mode::snes:
    return 4;
  case Mode::snes_mode7:
    return 8;
  case Mode::gb:
  case Mode::gbc:
    return 2;
  case Mode::pce:
  case Mode::pce_sprite:
    return 4;
  default:
    return 4;
  }
}

constexpr bool bpp_allowed_for_mode(unsigned bpp, Mode mode) {
  switch (mode) {
  case Mode::snes:
    return bpp == 2 || bpp == 4 || bpp == 8;
  case Mode::snes_mode7:
    return bpp == 8;
  case Mode::gb:
  case Mode::gbc:
    return bpp == 2;
  case Mode::pce:
  case Mode::pce_sprite:
    return bpp == 4;
  default:
    return false;
  }
}

constexpr unsigned default_tile_size_for_mode(Mode mode) {
  switch (mode) {
  case Mode::pce_sprite:
    return 16;
  default:
    return 8;
  }
}

constexpr unsigned max_tile_count_for_mode(Mode mode) {
  switch (mode) {
  case Mode::snes:
    return 1024;
  case Mode::snes_mode7:
    return 256;
  case Mode::gb:
    return 256;
  case Mode::gbc:
    return 512;
  case Mode::pce:
    return 2048;
  case Mode::pce_sprite:
    return 0;
  default:
    return 0;
  }
}

constexpr bool tile_width_allowed_for_mode(unsigned width, Mode mode) {
  switch (mode) {
  case Mode::snes:
    return width == 8 || width == 16;
  case Mode::snes_mode7:
  case Mode::gb:
  case Mode::gbc:
  case Mode::pce:
    return width == 8;
  case Mode::pce_sprite:
    return width == 16;
  default:
    return false;
  }
}

constexpr bool tile_height_allowed_for_mode(unsigned height, Mode mode) {
  switch (mode) {
  case Mode::snes:
    return height == 8 || height == 16;
  case Mode::snes_mode7:
  case Mode::gb:
  case Mode::gbc:
  case Mode::pce:
    return height == 8;
  case Mode::pce_sprite:
    return height == 16;
  default:
    return false;
  }
}

constexpr bool tile_flipping_allowed_for_mode(Mode mode) {
  switch (mode) {
  case Mode::snes:
  case Mode::gbc:
    return true;
  case Mode::snes_mode7:
  case Mode::gb:
  case Mode::pce:
  case Mode::pce_sprite:
    return false;
  default:
    return false;
  }
}

constexpr unsigned default_map_size_for_mode(Mode mode) {
  switch (mode) {
  case Mode::snes:
    return 32;
  case Mode::snes_mode7:
    return 128;
  case Mode::gb:
  case Mode::gbc:
  case Mode::pce:
  case Mode::pce_sprite:
    return 0;
  default:
    return 32;
  }
}

constexpr unsigned default_palette_count_for_mode(Mode mode) {
  switch (mode) {
  case Mode::snes:
    return 8;
  case Mode::snes_mode7:
    return 1;
  case Mode::gb:
    return 1;
  case Mode::gbc:
    return 8;
  case Mode::pce:
  case Mode::pce_sprite:
    return 16;
  default:
    return 8;
  }
}

constexpr bool col0_is_shared_for_mode(Mode mode) {
  switch (mode) {
  case Mode::snes:
  case Mode::snes_mode7:
  case Mode::pce_sprite:
    return true;
  case Mode::gb:
  case Mode::gbc:
    return false;
  case Mode::pce:
    return true;
  default:
    return true;
  }
}

constexpr bool col0_is_shared_for_sprite_mode(Mode mode) {
  switch (mode) {
  case Mode::snes:
  case Mode::snes_mode7:
  case Mode::pce_sprite:
  case Mode::gb:
  case Mode::gbc:
  case Mode::pce:
    return true;
  default:
    return true;
  }
}

//
// mode-specific color transformations
//

// scale standard rgba color to mode-specific range
inline rgba_t reduce_color(const rgba_t color, Mode to_mode) {
  switch (to_mode) {
  case Mode::snes:
  case Mode::snes_mode7:
  case Mode::gbc:
    if (((color & 0xff000000) >> 24) < 0x80) {
      return transparent_color;
    } else {
      rgba_color c(color);
      c.r >>= 3;
      c.g >>= 3;
      c.b >>= 3;
      rgba_t scaled = c;
      return (scaled & 0x00ffffff) + 0xff000000;
    }
    break;
  case Mode::gb:
    // TODO: GB
    // http://problemkaputt.de/pandocs.htm#lcdmonochromepalettes
    // http://problemkaputt.de/pandocs.htm#lcdcolorpalettescgbonly
    return 0;
  case Mode::pce:
  case Mode::pce_sprite:
    if (((color & 0xff000000) >> 24) < 0x80) {
      return transparent_color;
    } else {
      rgba_color c(color);
      c.r >>= 5;
      c.g >>= 5;
      c.b >>= 5;
      rgba_t scaled = c;
      return (scaled & 0x00ffffff) + 0xff000000;
    }
    break;
  default:
    return 0;
  }
}

// scale standard rgba colors to mode-specific range
inline rgba_vec_t reduce_colors(const rgba_vec_t& colors, Mode to_mode) {
  auto vc = colors;
  for (rgba_t& color : vc)
    color = reduce_color(color, to_mode);
  return vc;
}

inline rgba_set_t reduce_colors(const rgba_set_t& colors, Mode to_mode) {
  auto sc = rgba_set_t();
  for (const rgba_t& color : colors)
    sc.insert(reduce_color(color, to_mode));
  return sc;
}

// scale color from mode-specific range to 8bpc RGBA range
inline rgba_t normalize_color(const rgba_t color, Mode from_mode) {
  rgba_color c(color);
  switch (from_mode) {
  case Mode::snes:
  case Mode::snes_mode7:
  case Mode::gbc:
    c.r = scale_up(c.r, 3);
    c.g = scale_up(c.g, 3);
    c.b = scale_up(c.b, 3);
    c.a = scale_up(c.a, 3);
    return c;
  case Mode::gb:
    // TODO: GB
    return 0;
  case Mode::pce:
  case Mode::pce_sprite:
    c.r = scale_up(c.r, 5);
    c.g = scale_up(c.g, 5);
    c.b = scale_up(c.b, 5);
    c.a = scale_up(c.a, 5);
    return c;
  default:
    return 0;
  }
}

// scale colors from mode-specific range to 8bpc RGBA range
inline rgba_vec_t normalize_colors(const rgba_vec_t& colors, Mode from_mode) {
  auto vc = colors;
  for (rgba_t& color : vc)
    color = normalize_color(color, from_mode);
  return vc;
}

//
// to/from native color data
//

// pack scaled rgba color to native format
inline byte_vec_t pack_native_color(const rgba_t color, Mode mode) {
  byte_vec_t v;
  switch (mode) {
  case Mode::snes:
  case Mode::snes_mode7:
  case Mode::gbc:
    v.push_back((color & 0x1f) | ((color >> 3) & 0xe0));
    v.push_back(((color >> 11) & 0x03) | ((color >> 14) & 0x7c));
    break;
  case Mode::gb:
    // TODO: GB
    break;
  case Mode::pce:
  case Mode::pce_sprite:
    v.push_back(((color >> 16) & 0x07) | (color << 3 & 0x38) | ((color >> 2) & 0xc0));
    v.push_back((color >> 10) & 0x01);
    break;
  default:
    break;
  }
  return v;
}

// unpack native format color to (scaled) rgba color
inline rgba_vec_t unpack_native_colors(const byte_vec_t& colors, Mode mode) {
  rgba_vec_t v;
  switch (mode) {
  case Mode::snes:
  case Mode::snes_mode7:
  case Mode::gbc:
    if (colors.size() % 2 != 0) {
      throw std::runtime_error("snes/gbc native palette size not a multiple of 2");
    }
    for (unsigned i = 0; i < colors.size(); i += 2) {
      uint16_t cw = (colors[i + 1] << 8) + colors[i];
      rgba_t nc = (cw & 0x001f) | ((cw & 0x03e0) << 3) | ((cw & 0x7c00) << 6) | 0xff000000;
      v.push_back(nc);
    }
    break;
  case Mode::gb:
    // TODO: GB
    break;
  case Mode::pce:
  case Mode::pce_sprite:
    if (colors.size() % 2 != 0) {
      throw std::runtime_error("pce native palette size not a multiple of 2");
    }
    for (unsigned i = 0; i < colors.size(); i += 2) {
      uint16_t cw = (colors[i + 1] << 8) + colors[i];
      rgba_t nc = ((cw & 0x0038) >> 3) | ((cw & 0x0007) << 8) | ((cw & 0x1c00) << 10) | 0xff000000;
      v.push_back(nc);
    }
    break;
  default:
    break;
  }
  return v;
}

//
// to/from native tile data
//

inline byte_vec_t pack_native_tile(const index_vec_t& data, Mode mode, unsigned bpp, unsigned width, unsigned height) {

  auto make_2bpp_tile = [](const index_vec_t& in_data, unsigned plane_index) {
    byte_vec_t p(16);
    if (in_data.empty())
      return p;

    index_t mask0 = 1;
    for (unsigned i = 0; i < plane_index; ++i)
      mask0 <<= 1;
    index_t mask1 = mask0 << 1;

    unsigned shift0 = plane_index;
    unsigned shift1 = plane_index + 1;

    for (unsigned y = 0; y < 8; ++y) {
      for (unsigned x = 0; x < 8; ++x) {
        p[y * 2 + 0] |= ((in_data[y * 8 + x] & mask0) >> shift0) << (7 - x);
        p[y * 2 + 1] |= ((in_data[y * 8 + x] & mask1) >> shift1) << (7 - x);
      }
    }
    return p;
  };

  auto make_bitplane_data = [](const index_vec_t& in_data, unsigned plane) {
    if (in_data.size() % 8)
      throw std::runtime_error("programmer error (in_data not multiple of 8 in make_bitplane_data())");

    size_t plane_size = in_data.size() >> 3;
    byte_vec_t p(plane_size);

    index_t mask = 1;
    for (unsigned i = 0; i < plane; ++i)
      mask <<= 1;

    for (unsigned index_b = 0, index_i = 0; index_b < plane_size; ++index_b) {
      index_t byte = 0;
      for (unsigned b = 0; b < 8; ++b) {
        if (in_data[index_i + b] & mask)
          byte |= 1 << b;
      }
      p[index_b] = byte;
      index_i += 8;
    }
    return p;
  };

  byte_vec_t nd;

  if (mode == Mode::snes || mode == Mode::gb || mode == Mode::gbc || mode == Mode::pce) {
    if (width != 8 || height != 8)
      throw std::runtime_error(
        fmt::format("programmer error (tile size not 8x8 in pack_native_tile() for mode \"{}\")", sfc::mode(mode)));

    unsigned planes = bpp >> 1;
    for (unsigned i = 0; i < planes; ++i) {
      auto plane = make_2bpp_tile(data, i * 2);
      nd.insert(nd.end(), plane.begin(), plane.end());
    }

  } else if (mode == Mode::snes_mode7) {
    nd = data;

  } else if (mode == Mode::pce_sprite) {
    for (unsigned p = 0; p < 4; ++p) {
      auto plane = make_bitplane_data(data, p);
      nd.insert(nd.end(), plane.begin(), plane.end());
    }
  }

  return nd;
}

inline index_vec_t unpack_native_tile(const byte_vec_t& data, Mode mode, unsigned bpp, unsigned width, unsigned height) {

  auto add_1bit_plane = [](index_vec_t& out_data, const byte_vec_t& in_data, unsigned plane_index) {
    int plane_offset = ((plane_index >> 1) * 16) + (plane_index & 1);
    for (int y = 0; y < 8; ++y) {
      for (int x = 0; x < 8; ++x) {
        out_data[y * 8 + x] += ((in_data[plane_offset + (y * 2)] >> (7 - x)) & 1) << plane_index;
      }
    }
  };

  index_vec_t ud(width * height);

  if (mode == Mode::snes || mode == Mode::gb || mode == Mode::gbc || mode == Mode::pce) {
    for (unsigned i = 0; i < bpp; ++i)
      add_1bit_plane(ud, data, i);
  } else if (mode == Mode::snes_mode7) {
    ud = data;
  } else if (mode == Mode::pce_sprite) {
    // TODO: read pce_sprite data
    throw std::runtime_error("Using pce_sprite native data as input not implemented");
  }

  return ud;
}

} /* namespace sfc */
