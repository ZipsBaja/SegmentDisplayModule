#pragma once

#include <module/CollectionModule.h> // Assuming ZipsPicoLib is included in project.
#include <util/ArrayView.h>
#include <event/Event.h>

#include <stdio.h>

#include <functional>

extern "C"
{
#include <PicoTM1637.h>
}

namespace uazips
{
    
    constexpr size_t digits_per_device = 4;

    class SegmentDisplayModule : public CollectionModule<TM1637_device>
    {
    private:
        size_t device_count;
        size_t digit_count;
        bool break_animation;

        static uint32_t GetHexFromSegments(uint32_t segments);

    public:
        struct SegmentDisplaySettings
        {
            PIO pio; // PIO instance
            uint8_t clk_pin; // Clock
            uint8_t dio_pin; // D i/o
            uint8_t sm; // State machine
            uint8_t brightness;
            bool colon;
            bool is_upside_down;
            pio_sm_config config;

            TM1637_device* device_ptr;
        };

        static constexpr SegmentDisplaySettings Create(PIO pio, uint8_t clk_pin, uint8_t dio_pin, uint8_t sm, uint8_t brightness, bool colon, bool upside_down = false)
        {
            return SegmentDisplaySettings{
                .pio = pio,
                .clk_pin = clk_pin,
                .dio_pin = dio_pin,
                .sm = sm,
                .brightness = brightness,
                .colon = colon,
                .is_upside_down = upside_down,
                .device_ptr = nullptr
            };
        }
    public:
        SegmentDisplayModule(SegmentDisplaySettings& settings);
        SegmentDisplayModule(SegmentDisplaySettings* settings, size_t display_amount);
        virtual ~SegmentDisplayModule();

        virtual bool Initialize() override;
        
        template<typename V>
        void DisplayValueAll(V value)
        {
            char display[digit_count];
            if constexpr (std::is_integral_v<V>)
            {
                if constexpr (std::is_unsigned_v<V>)
                    snprintf(display, digit_count, "%u", value);
                else
                    snprintf(display, digit_count, "%d", value);
            }
            else if constexpr (std::is_floating_point_v<V>)
                snprintf(display, digit_count, "%f");
            else if constexpr (std::is_same_v<V, const char*> || std::is_same_v<V, char*>)
                snprintf(display, digit_count, "%s");
            
            DisplayTextAll(display);
        }

        template<typename V>
        void DisplayValue(size_t index, V value)
        {
            char display[digits_per_device];
            if constexpr (std::is_integral_v<V>)
            {
                if constexpr (std::is_unsigned_v<V>)
                    snprintf(display, digits_per_device, "%u", value);
                else
                    snprintf(display, digits_per_device, "%d", value);
            }
            else if constexpr (std::is_floating_point_v<V>)
                snprintf(display, digits_per_device, "%f");
            else if constexpr (std::is_same_v<V, const char*> || std::is_same_v<V, char*>)
                snprintf(display, digits_per_device, "%s");
            
            DisplayText(index, display);
        }

        template<typename V>
        void DisplayValue(const SegmentDisplaySettings& device, V value)
        {
            DisplayValue(FindElement(device.device_ptr), value);
        }

        void DisplayClearAll();
        void DisplayClear(size_t index);
        void DisplayClear(const SegmentDisplaySettings& device);
        
        void DisplayTextAll(const char* text, bool align_left = true);
        void DisplayText(size_t index, const char* text, bool align_left = true);
        void DisplayText(const SegmentDisplaySettings& device, const char* text, bool align_left = true);
        
        void DisplayMovingTextAll(const char* text, uint32_t interval_ms, bool right_incoming = true);
        void DisplayMovingText(size_t index, const char* text, uint32_t interval_ms, bool right_incoming = true);
        void DisplayMovingText(const SegmentDisplaySettings& device, const char* text, uint32_t interval_ms, bool right_incoming = true);
        
        void DisplayFlashingTextAll(const char* text, uint32_t interval_ms, uint32_t loops, bool align_left = true);
        void DisplayFlashingText(size_t index, const char* text, uint32_t interval_ms, uint32_t loops, bool align_left = true);
        void DisplayFlashingText(const SegmentDisplaySettings& device, const char* text, uint32_t interval_ms, uint32_t loops, bool align_left = true);
        
        void DisplaySetAll(char value);
        void DisplaySetSingle(size_t digit_index, char value);
        void DisplaySet(size_t index, char value);
        void DisplaySet(const SegmentDisplaySettings& device, char value);

        void DisplaySegmentsAll(uint32_t segments);
        void DisplaySegmentsAll(const uint32_t* segments_array, size_t arr_size);
        void DisplaySegmentsSingle(size_t digit_index, uint8_t segments);
        void DisplaySegments(size_t index, uint32_t segments);
        void DisplaySegments(const SegmentDisplaySettings& device, uint32_t segments);

        void DisplayAnimationAll(const ArrayView2D<uint32_t>& frames, float target_fps, const std::function<bool()>& loop_until, bool reverse = false);
        void DisplayAnimationAll(const ArrayView<uint32_t>& frames, float target_fps, const std::function<bool()>& loop_until, bool reverse = false);
        void DisplayAnimationAll(const uint32_t** frames, size_t frame_count, size_t display_count, float target_fps, const std::function<bool()>& loop_until, bool reverse = false);
        void DisplayAnimationAll(const uint32_t* frames, size_t frame_count, float target_fps, const std::function<bool()>& loop_until, bool reverse = false);
        void DisplayAnimationAll(const ArrayView2D<uint32_t>& frames, float target_fps, bool* loop_until, bool reverse = false);
        void DisplayAnimationAll(const ArrayView<uint32_t>& frames, float target_fps, bool* loop_until, bool reverse = false);
        void DisplayAnimationAll(const uint32_t** frames, size_t frame_count, size_t display_count, float target_fps, bool* loop_until, bool reverse = false);
        void DisplayAnimationAll(const uint32_t* frames, size_t frame_count, float target_fps, bool* loop_until, bool reverse = false);
        void DisplayAnimation(size_t index, const ArrayView<uint32_t>& frames, float target_fps, const std::function<bool()>& loop_until, bool reverse = false);
        void DisplayAnimation(const SegmentDisplaySettings& device, const ArrayView<uint32_t>& frames, float target_fps, const std::function<bool()>& loop_until, bool reverse = false);
        void DisplayAnimation(size_t index, const uint32_t* frames, size_t frame_count, float target_fps, const std::function<bool()>& loop_until, bool reverse = false);
        void DisplayAnimation(const SegmentDisplaySettings& device, const uint32_t* frames, size_t frame_count, float target_fps, const std::function<bool()>& loop_until, bool reverse = false);
        void DisplayAnimation(size_t index, const ArrayView<uint32_t>& frames, float target_fps, bool* loop_until, bool reverse = false);
        void DisplayAnimation(const SegmentDisplaySettings& device, const ArrayView<uint32_t>& frames, float target_fps, bool* loop_until, bool reverse = false);
        void DisplayAnimation(size_t index, const uint32_t* frames, size_t frame_count, float target_fps, bool* loop_until, bool reverse = false);
        void DisplayAnimation(const SegmentDisplaySettings& device, const uint32_t* frames, size_t frame_count, float target_fps, bool* loop_until, bool reverse = false);
        void DisplayAnimationOnceAll(const ArrayView2D<uint32_t>& frames, float target_fps, bool keep_last_frame = false, bool reverse = false);
        void DisplayAnimationOnceAll(const ArrayView<uint32_t>& frames, float target_fps, bool keep_last_frame = false, bool reverse = false);
        void DisplayAnimationOnceAll(const uint32_t** frames, size_t frame_count, size_t display_count, float target_fps, bool keep_last_frame = false, bool reverse = false);
        void DisplayAnimationOnceAll(const uint32_t* frames, size_t frame_count, float target_fps, bool keep_last_frame = false, bool reverse = false);
        void DisplayAnimationOnce(size_t index, const ArrayView<uint32_t>& frames, float target_fps, bool keep_last_frame = false, bool reverse = false);
        void DisplayAnimationOnce(const SegmentDisplaySettings& device, const ArrayView<uint32_t>& frames, float target_fps, bool keep_last_frame = false, bool reverse = false);
        void DisplayAnimationOnce(size_t index, const uint32_t* frames, size_t frame_count, float target_fps, bool keep_last_frame = false, bool reverse = false);
        void DisplayAnimationOnce(const SegmentDisplaySettings& device, const uint32_t* frames, size_t frame_count, float target_fps, bool keep_last_frame = false, bool reverse = false);

        template<class EventType> void DisplayAnimationAll(const ArrayView2D<uint32_t>& frames, float target_fps, EventSource<EventType>& event_device, bool reverse = false)
        {
            bool break_animation = false;
            const char* id = event_device.AddListener([&](const EventType* event){
                break_animation = true;
                event_device.RemoveListener(id);
            });
            DisplayAnimationAll(frames, target_fps, [&](){ return break_animation; }, reverse);
            delete id;
        }
        template<class EventType> void DisplayAnimationAll(const ArrayView<uint32_t>& frames, float target_fps, EventSource<EventType>& event_device, bool reverse = false)
        {
            bool break_animation = false;
            const char* id = event_device.AddListener([&](const EventType* event){
                break_animation = true;
                event_device.RemoveListener(id);
            });
            DisplayAnimationAll(frames, target_fps, [&](){ return break_animation; }, reverse);
            delete id;
        }
        template<class EventType> void DisplayAnimationAll(const uint32_t** frames, size_t frame_count, size_t display_count, float target_fps, EventSource<EventType>& event_device, bool reverse = false)
        {
            bool break_animation = false;
            const char* id = event_device.AddListener([&](const EventType* event){
                break_animation = true;
                event_device.RemoveListener(id);
            });
            DisplayAnimationAll(frames, frame_count, display_count, target_fps, [&](){ return break_animation; }, reverse);
            delete id;
        }
        template<class EventType> void DisplayAnimationAll(const uint32_t* frames, size_t frame_count, float target_fps, EventSource<EventType>& event_device, bool reverse = false)
        {
            bool break_animation = false;
            const char* id = event_device.AddListener([&](const EventType* event){
                break_animation = true;
                event_device.RemoveListener(id);
            });
            DisplayAnimationAll(frames, frame_count, target_fps, [&](){ return break_animation; }, reverse);
            delete id;
        }
        template<class EventType> void DisplayAnimation(size_t index, const ArrayView<uint32_t>& frames, float target_fps, EventSource<EventType>& event_device, bool reverse = false)
        {
            bool break_animation = false;
            const char* id = event_device.AddListener([&](const EventType* event){
                break_animation = true;
                event_device.RemoveListener(id);
            });
            DisplayAnimation(index, frames, target_fps, [&](){ return break_animation; }, reverse);
            delete id;
        }     
        template<class EventType> void DisplayAnimation(const SegmentDisplaySettings& device, const ArrayView<uint32_t>& frames, float target_fps, EventSource<EventType>& event_device, bool reverse = false)
        {
            bool break_animation = false;
            const char* id = event_device.AddListener([&](const EventType* event){
                break_animation = true;
                event_device.RemoveListener(id);
            });
            DisplayAnimation(device, frames, target_fps, [&](){ return break_animation; }, reverse);
            delete id;
        }
        template<class EventType> void DisplayAnimation(size_t index, const uint32_t* frames, size_t frame_count, float target_fps, EventSource<EventType>& event_device, bool reverse = false)
        {
            bool break_animation = false;
            const char* id = event_device.AddListener([&](const EventType* event){
                break_animation = true;
                event_device.RemoveListener(id);
            });
            DisplayAnimation(index, frames, frame_count, target_fps, [&](){ return break_animation; }, reverse);
            delete id;
        }      
        template<class EventType> void DisplayAnimation(const SegmentDisplaySettings& device, const uint32_t* frames, size_t frame_count, float target_fps, EventSource<EventType>& event_device, bool reverse = false)
        {
            bool break_animation = false;
            const char* id = event_device.AddListener([&](const EventType* event){
                break_animation = true;
                event_device.RemoveListener(id);
            });
            DisplayAnimation(device, frames, frame_count, target_fps, [&](){ return break_animation; }, reverse);
            delete id;
        } 
    };

}