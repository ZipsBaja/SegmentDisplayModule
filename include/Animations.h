#pragma once

#include <util/ArrayView.h>

#include <stdint.h>

namespace uazips
{
    constexpr uint8_t segment_top =         0b01000000;
    constexpr uint8_t segment_upper_right = 0b00100000;
    constexpr uint8_t segment_lower_right = 0b00010000;
    constexpr uint8_t segment_bottom =      0b00001000;
    constexpr uint8_t segment_lower_left =  0b00000100;
    constexpr uint8_t segment_upper_left =  0b00000010;
    constexpr uint8_t segment_middle =      0b00000001;
    constexpr uint8_t segment_none =        0b00000000;

    namespace animations
    {
        namespace onedisplay
        {
            constexpr size_t device_count = 1;

            constexpr uint32_t frames_looping[12] = {
                0b01000000 << 24,
                0b01000000 << 16,
                0b01000000 << 8,
                0b01000000 << 0,
                0b00100000 << 0,
                0b00010000 << 0,
                0b00001000 << 0,
                0b00001000 << 8,
                0b00001000 << 16,
                0b00001000 << 24,
                0b00010000 << 24,
                0b00100000 << 24    
            };

            constexpr ArrayView<uint32_t> anim_looping = make_view(frames_looping);
        }

        namespace twodisplay
        {
            constexpr size_t device_count = 2;

            constexpr uint32_t frames_looping[device_count][20] = {
                // display 1
                {
                    0b01000000 << 24,
                    0b01000000 << 16,
                    0b01000000 << 8,
                    0b01000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00001000 << 0,
                    0b00001000 << 8,
                    0b00001000 << 16,
                    0b00001000 << 24,
                    0b00010000 << 24,
                    0b00100000 << 24
                },
                // display 2
                {
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b01000000 << 24,
                    0b01000000 << 16,
                    0b01000000 << 8,
                    0b01000000 << 0,
                    0b00100000 << 0,
                    0b00010000 << 0,
                    0b00001000 << 0,
                    0b00001000 << 8,
                    0b00001000 << 16,
                    0b00001000 << 24,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0,
                    0b00000000 << 0
                }
            };  
            
            constexpr ArrayView2D<uint32_t> anim_looping = make_view(frames_looping);
        }
    }

}