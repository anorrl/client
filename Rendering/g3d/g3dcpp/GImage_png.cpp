#include "g3d/platform.h"
#include "g3d/GImage.h"
#include "png.h"
#include <sstream>
#include "g3d/BinaryOutput.h"

namespace G3D {

static void png_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    BinaryOutput* out = static_cast<BinaryOutput*>(png_get_io_ptr(png_ptr));
    debugAssert(out != nullptr && data != nullptr);
    out->writeBytes(data, length);
}

static void png_flush_data(png_structp) {
    
}

static void png_error(png_structp png_ptr, png_const_charp error_msg) {
    png_set_error_fn(png_ptr, const_cast<png_charp>(error_msg), nullptr, nullptr);
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void png_warning(png_structp, png_const_charp) {
    
}

static void png_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    BinaryInput* in = static_cast<BinaryInput*>(png_get_io_ptr(png_ptr));
    debugAssert(in != nullptr && data != nullptr);

    int64 pos = in->getPosition();
    int64 len = in->getLength();
    if (pos + length > len) {
        length = len - pos;
        G3D::png_error(png_ptr, "Trying to load incomplete image");
    }
    in->readBytes(data, length);
}

void GImage::encodePNG(BinaryOutput& out) const {
    if (!(m_channels >= 1 && m_channels <= 4)) {
        throw GImage::Error(format("Illegal channels for PNG: %d", m_channels));
    }
    if (m_width <= 0 || m_height <= 0) {
        throw GImage::Error("Invalid image dimensions");
    }

    out.setEndian(G3D_LITTLE_ENDIAN);
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, png_error, png_warning);
    if (!png_ptr) throw GImage::Error("Unable to initialize PNG encoder.");

    if (setjmp(png_jmpbuf(png_ptr))) {
        throw GImage::Error("Error during PNG encoding");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        throw GImage::Error("Unable to initialize PNG info struct.");
    }

    png_set_write_fn(png_ptr, &out, png_write_data, png_flush_data);

    int color_type;
    png_color_8 sig_bit = {0, 0, 0, 0, 0};

    switch (m_channels) {
        case 1: color_type = PNG_COLOR_TYPE_GRAY; sig_bit.gray = 8; break;
        case 2: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; sig_bit.gray = sig_bit.alpha = 8; break;
        case 3: color_type = PNG_COLOR_TYPE_RGB; sig_bit.red = sig_bit.green = sig_bit.blue = 8; break;
        case 4: color_type = PNG_COLOR_TYPE_RGBA; sig_bit.red = sig_bit.green = sig_bit.blue = sig_bit.alpha = 8; break;
        default:
            png_destroy_write_struct(&png_ptr, &info_ptr);
            throw GImage::Error("Unsupported number of channels for PNG.");
    }

    png_set_IHDR(png_ptr, info_ptr, m_width, m_height, 8, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    png_write_info(png_ptr, info_ptr);

    png_bytepp row_pointers = new png_bytep[m_height];
    for (int i = 0; i < m_height; ++i) {
        row_pointers[i] = const_cast<png_bytep>(&m_byte[m_width * m_channels * i]);
    }
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);

    delete[] row_pointers;
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

void GImage::decodePNG(BinaryInput& input) {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, png_error, png_warning);
    if (!png_ptr) throw GImage::Error("Unable to initialize PNG decoder.");

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        throw GImage::Error("Error during PNG decoding");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!info_ptr || !end_info) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        throw GImage::Error("Unable to initialize PNG info structs");
    }

    png_set_read_fn(png_ptr, &input, png_read_data);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                 &color_type, &interlace_type, nullptr, nullptr);

    m_width = static_cast<uint32>(width);
    m_height = static_cast<uint32>(height);

    png_set_swap(png_ptr);
    png_set_strip_16(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    if (bit_depth < 8)
        png_set_packing(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    int num_channels = png_get_channels(png_ptr, info_ptr);
    m_channels = static_cast<uint8>(num_channels);

    size_t imageSize = m_width * m_height * m_channels;
    m_byte = static_cast<uint8*>(m_memMan->alloc(imageSize));
    if (!m_byte) throw GImage::Error("Out of memory for image buffer.");

    uint32 passes = png_set_interlace_handling(png_ptr);
    for (uint32 pass = 0; pass < passes; ++pass) {
        for (uint32 y = 0; y < m_height; ++y) {
            png_bytep row = &m_byte[m_width * m_channels * y];
            png_read_rows(png_ptr, &row, nullptr, 1);
        }
    }

    png_read_end(png_ptr, end_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
}

} // namespace G3D
