#include "teleios/teleios.h"
#include "teleios/state.h"
#include "glad/glad.h"
#include "teleios/graphics_helpers.h"

// #####################################################################################################
//
//                                           H E L P E R S
//
// #####################################################################################################

u8 tl_parse_buffer_size(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);

        case TL_BUFFER_TYPE_MARIX22: return 4;

        case TL_BUFFER_TYPE_MARIX32:
        case TL_BUFFER_TYPE_MARIX23: return 6;

        case TL_BUFFER_TYPE_MARIX24:
        case TL_BUFFER_TYPE_MARIX42: return 8;

        case TL_BUFFER_TYPE_MARIX33: return 9;

        case TL_BUFFER_TYPE_MARIX34:
        case TL_BUFFER_TYPE_MARIX43: return 12;

        case TL_BUFFER_TYPE_MARIX44: return 16;

        case TL_BUFFER_TYPE_BYTE1:
        case TL_BUFFER_TYPE_SHORT1:
        case TL_BUFFER_TYPE_INT1:
        case TL_BUFFER_TYPE_UBYTE1:
        case TL_BUFFER_TYPE_USHORT1:
        case TL_BUFFER_TYPE_UINT1:
        case TL_BUFFER_TYPE_FLOAT1:
        case TL_BUFFER_TYPE_DOUBLE1: return 1;

        case TL_BUFFER_TYPE_BYTE2:
        case TL_BUFFER_TYPE_SHORT2:
        case TL_BUFFER_TYPE_INT2:
        case TL_BUFFER_TYPE_UBYTE2:
        case TL_BUFFER_TYPE_USHORT2:
        case TL_BUFFER_TYPE_UINT2:
        case TL_BUFFER_TYPE_FLOAT2:
        case TL_BUFFER_TYPE_DOUBLE2: return 2;
        
        case TL_BUFFER_TYPE_BYTE3:
        case TL_BUFFER_TYPE_SHORT3:
        case TL_BUFFER_TYPE_INT3:
        case TL_BUFFER_TYPE_UBYTE3:
        case TL_BUFFER_TYPE_USHORT3:
        case TL_BUFFER_TYPE_UINT3:
        case TL_BUFFER_TYPE_FLOAT3:
        case TL_BUFFER_TYPE_DOUBLE3: return 3;
        
        case TL_BUFFER_TYPE_BYTE4:
        case TL_BUFFER_TYPE_SHORT4:
        case TL_BUFFER_TYPE_INT4:
        case TL_BUFFER_TYPE_UBYTE4:
        case TL_BUFFER_TYPE_USHORT4:
        case TL_BUFFER_TYPE_UINT4:
        case TL_BUFFER_TYPE_FLOAT4:
        case TL_BUFFER_TYPE_DOUBLE4: return 4;
    }
}

u32 tl_parse_buffer_type(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);
        
        case TL_BUFFER_TYPE_BYTE1:
        case TL_BUFFER_TYPE_BYTE2:
        case TL_BUFFER_TYPE_BYTE3:
        case TL_BUFFER_TYPE_BYTE4: return GL_BYTE;
        
        case TL_BUFFER_TYPE_SHORT1:
        case TL_BUFFER_TYPE_SHORT2:
        case TL_BUFFER_TYPE_SHORT3:
        case TL_BUFFER_TYPE_SHORT4: return GL_SHORT;

        case TL_BUFFER_TYPE_INT1:
        case TL_BUFFER_TYPE_INT2:
        case TL_BUFFER_TYPE_INT3:
        case TL_BUFFER_TYPE_INT4: return GL_INT;
        
        case TL_BUFFER_TYPE_UBYTE1:
        case TL_BUFFER_TYPE_UBYTE2:
        case TL_BUFFER_TYPE_UBYTE3:
        case TL_BUFFER_TYPE_UBYTE4: return GL_UNSIGNED_BYTE;
        
        case TL_BUFFER_TYPE_USHORT1:
        case TL_BUFFER_TYPE_USHORT2:
        case TL_BUFFER_TYPE_USHORT3:
        case TL_BUFFER_TYPE_USHORT4: return GL_UNSIGNED_SHORT;

        case TL_BUFFER_TYPE_UINT1:
        case TL_BUFFER_TYPE_UINT2:
        case TL_BUFFER_TYPE_UINT3:
        case TL_BUFFER_TYPE_UINT4: return GL_UNSIGNED_INT;

        case TL_BUFFER_TYPE_FLOAT1:
        case TL_BUFFER_TYPE_FLOAT2:
        case TL_BUFFER_TYPE_FLOAT3:
        case TL_BUFFER_TYPE_FLOAT4: return GL_FLOAT;
        
        case TL_BUFFER_TYPE_DOUBLE1:
        case TL_BUFFER_TYPE_DOUBLE2:
        case TL_BUFFER_TYPE_DOUBLE3:
        case TL_BUFFER_TYPE_DOUBLE4: return GL_DOUBLE;
    }
}

u32 tl_parse_buffer_bytes(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);
        
        case TL_BUFFER_TYPE_BYTE1: return 1 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE2: return 2 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE3: return 3 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE4: return 4 * sizeof(i8);
        
        case TL_BUFFER_TYPE_SHORT1: return 1 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT2: return 2 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT3: return 3 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT4: return 4 * sizeof(i16);

        case TL_BUFFER_TYPE_INT1: return 1 * sizeof(i32);
        case TL_BUFFER_TYPE_INT2: return 2 * sizeof(i32);
        case TL_BUFFER_TYPE_INT3: return 3 * sizeof(i32);
        case TL_BUFFER_TYPE_INT4: return 4 * sizeof(i32);
        
        case TL_BUFFER_TYPE_UBYTE1: return 1 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE2: return 2 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE3: return 3 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE4: return 4 * sizeof(u8);
        
        case TL_BUFFER_TYPE_USHORT1: return 1 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT2: return 2 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT3: return 3 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT4: return 4 * sizeof(u16);

        case TL_BUFFER_TYPE_UINT1: return 1 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT2: return 2 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT3: return 3 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT4: return 4 * sizeof(u32);

        case TL_BUFFER_TYPE_FLOAT1: return 1 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT2: return 2 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT3: return 3 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT4: return 4 * sizeof(f32);
        
        case TL_BUFFER_TYPE_DOUBLE1: return 1 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE2: return 2 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE3: return 3 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE4: return 4 * sizeof(f64);
    }
}

// #####################################################################################################
//
//                                           R E N D E R E R
//
// #####################################################################################################

void tl_graphics_clear(void) {
    TLDPUSH;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    TLDRE;
}

void tl_graphics_draw(TLGeometry* geometry) {
    TLDPUSH;
    
    glDrawElements(
        tl_parse_geometry_mode(geometry->mode), 
        geometry->ebo_length, 
        tl_parse_buffer_type(geometry->ebo_type), 
        0
    );

	TLDRE;
}