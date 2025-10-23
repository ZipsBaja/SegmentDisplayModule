#include <SegmentDisplayModule.h>

#include <util/BitUtils.h>
#include <util/TimeHandler.h>

#include <string.h>

extern "C"
{
    uint32_t num_to_hex(int32_t number, uint32_t bitmask);
}

namespace uazips
{

    SegmentDisplayModule::SegmentDisplayModule(SegmentDisplaySettings& settings)
        : CollectionModule(), device_count(1), digit_count(device_count * digits_per_device), break_animation(false)
    {
        TM1637_device* dev;
        if (settings.device_ptr == nullptr)
            dev = new TM1637_device;
        dev->pio = settings.pio;
        dev->clk_pin = settings.clk_pin;
        dev->dio_pin = settings.dio_pin;
        dev->sm = settings.sm;
        dev->brightness = settings.brightness;
        dev->colon = settings.colon;
        dev->_config = settings.config;

        settings.device_ptr = dev;
        m_elements.push_back(dev);
    }

    SegmentDisplayModule::SegmentDisplayModule(SegmentDisplaySettings* settings, size_t display_amount)
        : CollectionModule(), device_count(display_amount), digit_count(display_amount * digits_per_device), break_animation(false)
    {
        for (size_t i = 0; i < display_amount; i++)
        {
            TM1637_device* dev;
            if (settings[i].device_ptr == nullptr)
                dev = new TM1637_device;
            dev->pio = settings[i].pio;
            dev->clk_pin = settings[i].clk_pin;
            dev->dio_pin = settings[i].dio_pin;
            dev->sm = settings[i].sm;
            dev->brightness = settings[i].brightness;
            dev->colon = settings[i].colon;
            dev->_config = settings[i].config;

            settings[i].device_ptr = dev;
            m_elements.push_back(dev);
        }
    }

    SegmentDisplayModule::~SegmentDisplayModule()
    {
        for (auto&& dev : m_elements)
            delete dev;
    }

    bool SegmentDisplayModule::Initialize()
    {
        for (auto&& dev : m_elements)
        {
            TM1637_init(dev);
            TM1637_clear(dev);
        }

        return true;
    }
    
    void SegmentDisplayModule::DisplayClearAll()
    {
        for (auto&& device : m_elements)
            TM1637_clear(device);
    }

    void SegmentDisplayModule::DisplayClear(size_t index)
    {
        index = index >= device_count ? device_count - 1 : index;
        TM1637_clear(m_elements[index]);
        
    }

    void SegmentDisplayModule::DisplayClear(const SegmentDisplaySettings& device)
    {
        DisplayClear(FindElement(device.device_ptr));
    }

    void SegmentDisplayModule::DisplayTextAll(const char* text, bool align_left)
    {
        int digits_left = (int)digit_count;
        for (size_t i = 0; i < device_count; i++)
        {
            digits_left -= digits_per_device;
            if (digits_left < 0)
                break;
            TM1637_display_word(m_elements[i], text + i * digits_per_device, !align_left ? (i > 0) : true);
        }
    }

    void SegmentDisplayModule::DisplayText(size_t index, const char* text, bool align_left)
    {
        index = index >= device_count ? device_count - 1 : index;
        TM1637_display_word(m_elements[index], text, align_left);
    }

    void SegmentDisplayModule::DisplayText(const SegmentDisplaySettings& device, const char* text, bool align_left)
    {
        DisplayText(FindElement(device.device_ptr), text, align_left);
    }

    void SegmentDisplayModule::DisplayMovingTextAll(const char* text, uint32_t interval_ms, bool right_incoming)
    {
        TM1637_device* arr[device_count];
        std::copy(m_elements.begin(), m_elements.end(), arr);
        TM1637_display_moving_text(arr, device_count, text, interval_ms, right_incoming);
    }

    void SegmentDisplayModule::DisplayMovingText(size_t index, const char* text, uint32_t interval_ms, bool right_incoming)
    {
        index = index >= device_count ? device_count - 1 : index;
        TM1637_display_moving_text(&m_elements[index], 1, text, interval_ms, right_incoming);
    }

    void SegmentDisplayModule::DisplayMovingText(const SegmentDisplaySettings& device, const char* text, uint32_t interval_ms, bool right_incoming)
    {
        DisplayMovingText(FindElement(device.device_ptr), text, interval_ms, right_incoming);
    }

    void SegmentDisplayModule::DisplayFlashingTextAll(const char* text, uint32_t interval_ms, uint32_t loops, bool align_left)
    {
        for (uint32_t i = 0; i < loops; i++)
        {
            DisplayTextAll(text, align_left);
            sleep_ms(interval_ms);
            DisplayClearAll();
            sleep_ms(interval_ms);
        }
    }

    void SegmentDisplayModule::DisplayFlashingText(size_t index, const char* text, uint32_t interval_ms, uint32_t loops, bool align_left)
    {
        index = index >= device_count ? device_count - 1 : index;
        for (uint32_t i = 0; i < loops; i++)
        {
            DisplayText(index, text, align_left);
            sleep_ms(interval_ms);
            DisplayClear(index);
            sleep_ms(interval_ms);
        }
    }

    void SegmentDisplayModule::DisplayFlashingText(const SegmentDisplaySettings& device, const char* text, uint32_t interval_ms, uint32_t loops, bool align_left)
    {
        DisplayFlashingText(FindElement(device.device_ptr), text, interval_ms, loops, align_left);
    }

    void SegmentDisplayModule::DisplaySetAll(char value)
    {
        char buff[digit_count];
        memset(buff, value, digit_count);
        DisplayTextAll(buff);
    }

    void SegmentDisplayModule::DisplaySetSingle(size_t digit_index, char value)
    {
        digit_index = digit_index >= digit_count ? digit_count - 1 : digit_index;

        TM1637_put_2_bytes(m_elements[digit_index / digits_per_device], digit_index % digits_per_device, num_to_hex(value, 0x00));
    }

    void SegmentDisplayModule::DisplaySet(size_t index, char value)
    {
        index = index >= device_count ? device_count - 1 : index;
        char buff[digits_per_device];
        memset(buff, value, digits_per_device);
        TM1637_display_word(m_elements[index], buff, false);
    }

    void SegmentDisplayModule::DisplaySet(const SegmentDisplaySettings& device, char value)
    {
        DisplaySet(FindElement(device.device_ptr), value);
    }

    uint32_t SegmentDisplayModule::GetHexFromSegments(uint32_t segments)
    {
        ByteSplitter<uint32_t, uint8_t> splitter;
        splitter.single = segments;
        uint8_t digit4 = splitter.many[3];
        uint8_t digit3 = splitter.many[2];
        uint8_t digit2 = splitter.many[1];
        uint8_t digit1 = splitter.many[0];

        uint32_t hex = 0x00;
        hex = digit1 + (hex << 8);
        hex = digit2 + (hex << 8);
        hex = digit3 + (hex << 8);
        hex = digit4 + (hex << 8);

        return hex;
    }

    void SegmentDisplayModule::DisplaySegmentsAll(uint32_t segments)
    {
        uint32_t hex = GetHexFromSegments(segments);

        for (size_t i = 0; i < device_count; i++)
        {
            TM1637_put_4_bytes(m_elements[i], 0, hex);
        }
    }

    void SegmentDisplayModule::DisplaySegmentsAll(const uint32_t* segments_array, size_t arr_size)
    {
        arr_size = arr_size > device_count ? device_count : arr_size;

        for (size_t i = 0; i < device_count; i++)
        {
            uint32_t hex;

            if (i < arr_size)
                hex = GetHexFromSegments(segments_array[i]);
            else
                hex = GetHexFromSegments(segments_array[arr_size]);

            TM1637_put_4_bytes(m_elements[i], 0, hex);
        }
    }

    void SegmentDisplayModule::DisplaySegmentsSingle(size_t digit_index, uint8_t segments)
    {
        digit_index = digit_index >= digit_count ? digit_count - 1 : digit_index;

        TM1637_put_2_bytes(m_elements[digit_index / digits_per_device], digit_index % digits_per_device, segments);
    }

    void SegmentDisplayModule::DisplaySegments(size_t index, uint32_t segments)
    {
        index = index >= device_count ? device_count - 1 : index;
        
        uint32_t hex = GetHexFromSegments(segments);

        TM1637_put_4_bytes(m_elements[index], 0, hex);
    }

    void SegmentDisplayModule::DisplaySegments(const SegmentDisplaySettings& device, uint32_t segments)
    {
        DisplaySegments(FindElement(device.device_ptr), segments);
    }

    void SegmentDisplayModule::DisplayAnimationAll(const ArrayView2D<uint32_t>& frames, float target_fps, const std::function<bool()>& loop_until, bool reverse)
    {
        uint64_t us_interval = static_cast<uint64_t>(1.0f / target_fps * 1e6f);
        uint64_t cumulative_us = 0;

        TimeHandler th;

        const size_t display_count = frames.rows;
        const size_t frame_count = frames.cols;

        while (!loop_until())
        {
            for (size_t current_frame = reverse ? frame_count - 1 : 0; reverse ? current_frame >= 0 : current_frame < frame_count; reverse? current_frame-- : current_frame++)
            {
                if (loop_until())
                {
                    DisplayClearAll();
                    return;
                }

                uint32_t frames_to_show[display_count];
                for (size_t i = 0; i < display_count; i++)
                    frames_to_show[i] = (frames.data + frame_count * i)[current_frame];

                DisplaySegmentsAll(frames_to_show, display_count);

                while (cumulative_us < us_interval)
                {
                    if (loop_until())
                    {
                        DisplayClearAll();
                        return;
                    }
                    th.Update();
                    cumulative_us += th.DeltaTime;
                }
                cumulative_us -= us_interval;
            }
        }
        DisplayClearAll();
    }

    void SegmentDisplayModule::DisplayAnimationAll(const ArrayView<uint32_t>& frames, float target_fps, const std::function<bool()>& loop_until, bool reverse)
    {
        uint64_t us_interval = static_cast<uint64_t>(1.0f / target_fps * 1e6f);
        uint64_t cumulative_us = 0;

        TimeHandler th;

        const size_t frame_count = frames.length;

        while (!loop_until())
        {
            for (size_t current_frame = reverse ? frame_count - 1 : 0; reverse ? current_frame >= 0 : current_frame < frame_count; reverse? current_frame-- : current_frame++)
            {
                if (loop_until())
                {
                    DisplayClearAll();
                    return;
                }
                DisplaySegmentsAll(frames.data[current_frame]);

                while (cumulative_us < us_interval)
                {
                    if (loop_until())
                    {
                        DisplayClearAll();
                        return;
                    }
                    th.Update();
                    cumulative_us += th.DeltaTime;
                }
                cumulative_us -= us_interval;
            }
        }
        DisplayClearAll();
    }

    void SegmentDisplayModule::DisplayAnimationAll(const uint32_t** frames, size_t frame_count, size_t display_count, float target_fps, const std::function<bool()>& loop_until, bool reverse)
    {
        display_count = display_count > device_count ? device_count : display_count;
        uint64_t us_interval = static_cast<uint64_t>(1.0f / target_fps * 1e6f);
        uint64_t cumulative_us = 0;

        TimeHandler th;

        while (!loop_until())
        {
            for (size_t current_frame = reverse ? frame_count - 1 : 0; reverse ? current_frame >= 0 : current_frame < frame_count; reverse? current_frame-- : current_frame++)
            {
                if (loop_until())
                {
                    DisplayClearAll();
                    return;
                }

                uint32_t frames_to_show[display_count];
                for (size_t i = 0; i < display_count; i++)
                    frames_to_show[i] = frames[i][current_frame];

                DisplaySegmentsAll(frames_to_show, display_count);

                while (cumulative_us < us_interval)
                {
                    if (loop_until())
                    {
                        DisplayClearAll();
                        return;
                    }
                    th.Update();
                    cumulative_us += th.DeltaTime;
                }
                cumulative_us -= us_interval;
            }
        }
        DisplayClearAll();
    }

    void SegmentDisplayModule::DisplayAnimationAll(const uint32_t* frames, size_t frame_count, float target_fps, const std::function<bool()>& loop_until, bool reverse)
    {
        uint64_t us_interval = static_cast<uint64_t>(1.0f / target_fps * 1e6f);
        uint64_t cumulative_us = 0;

        TimeHandler th;

        while (!loop_until())
        {
            th.Update();
            for (size_t current_frame = reverse ? frame_count - 1 : 0; reverse ? current_frame >= 0 : current_frame < frame_count; reverse? current_frame-- : current_frame++)
            {
                if (loop_until())
                {
                    DisplayClearAll();
                    return;
                }
                DisplaySegmentsAll(frames[current_frame]);
                
                while (cumulative_us < us_interval) // Sleep semantics voids the break-out-of-loop idea given in parameters
                {
                    if (loop_until())
                    {
                        DisplayClearAll();
                        return;
                    }
                    th.Update();
                    cumulative_us += th.DeltaTime;
                }
                cumulative_us -= us_interval;
            }
        }
        DisplayClearAll();
    }

    void SegmentDisplayModule::DisplayAnimation(size_t index, const ArrayView<uint32_t>& frames, float target_fps, const std::function<bool()>& loop_until, bool reverse)
    {
        index = index >= device_count ? device_count - 1 : index;
        uint64_t us_interval = static_cast<uint64_t>(1.0f / target_fps * 1e6f);
        uint64_t cumulative_us = 0;

        TimeHandler th;

        while (!loop_until())
        {
            th.Update();
            for (size_t current_frame = reverse ? frames.length - 1 : 0; reverse ? current_frame >= 0 : current_frame < frames.length; reverse? current_frame-- : current_frame++)
            {
                if (loop_until())
                {
                    DisplayClearAll();
                    return;
                }
                DisplaySegments(index, frames.data[current_frame]);

                while (cumulative_us < us_interval)
                {
                    if (loop_until())
                    {
                        DisplayClear(index);
                        return;
                    }
                    th.Update();
                    cumulative_us += th.DeltaTime;
                }
                cumulative_us -= us_interval;
            }
        }
        DisplayClear(index);
    }

    void SegmentDisplayModule::DisplayAnimation(const SegmentDisplaySettings& device, const ArrayView<uint32_t>& frames, float target_fps, const std::function<bool()>& loop_until, bool reverse)
    {
        DisplayAnimation(FindElement(device.device_ptr), frames, target_fps, loop_until, reverse);
    }

    void SegmentDisplayModule::DisplayAnimation(size_t index, const uint32_t* frames, size_t frame_count, float target_fps, const std::function<bool()>& loop_until, bool reverse)
    {
        index = index >= device_count ? device_count - 1 : index;
        uint64_t us_interval = static_cast<uint64_t>(1.0f / target_fps * 1e6f);
        uint64_t cumulative_us = 0;

        TimeHandler th;

        while (!loop_until())
        {
            th.Update();
            for (size_t current_frame = reverse ? frame_count - 1 : 0; reverse ? current_frame >= 0 : current_frame < frame_count; reverse? current_frame-- : current_frame++)
            {
                if (loop_until())
                {
                    DisplayClearAll();
                    return;
                }
                DisplaySegments(index, frames[current_frame]);

                while (cumulative_us < us_interval)
                {
                    if (loop_until())
                    {
                        DisplayClear(index);
                        return;
                    }
                    th.Update();
                    cumulative_us += th.DeltaTime;
                }
                cumulative_us -= us_interval;
            }
        }
        DisplayClear(index);
    }

    void SegmentDisplayModule::DisplayAnimation(const SegmentDisplaySettings& device, const uint32_t* frames, size_t frame_count, float target_fps, const std::function<bool()>& loop_until, bool reverse)
    {
        DisplayAnimation(FindElement(device.device_ptr), frames, frame_count, target_fps, loop_until, reverse);
    }

    void SegmentDisplayModule::DisplayAnimationOnceAll(const ArrayView2D<uint32_t>& frames, float target_fps, bool keep_last_frame, bool reverse)
    {
        uint64_t us_interval = static_cast<uint64_t>(1.0f / target_fps * 1e6f);
        uint64_t cumulative_us = 0;

        TimeHandler th;

        const size_t display_count = frames.rows;
        const size_t frame_count = frames.cols;

        for (size_t current_frame = reverse ? frame_count - 1 : 0; reverse ? current_frame >= 0 : current_frame < frame_count; reverse? current_frame-- : current_frame++)
        {
            uint32_t frames_to_show[display_count];
            for (size_t i = 0; i < display_count; i++)
                frames_to_show[i] = (frames.data + frame_count * i)[current_frame];

            DisplaySegmentsAll(frames_to_show, display_count);

            while (cumulative_us < us_interval)
            {
                th.Update();
                cumulative_us += th.DeltaTime;
            }
            cumulative_us -= us_interval;
        }
        if (!keep_last_frame)
            DisplayClearAll();
    }

    void SegmentDisplayModule::DisplayAnimationOnceAll(const ArrayView<uint32_t>& frames, float target_fps, bool keep_last_frame, bool reverse)
    {
        uint64_t us_interval = static_cast<uint64_t>(1.0f / target_fps * 1e6f);
        uint64_t cumulative_us = 0;

        TimeHandler th;

        for (size_t current_frame = reverse ? frames.length - 1 : 0; reverse ? current_frame >= 0 : current_frame < frames.length; reverse? current_frame-- : current_frame++)
        {
            DisplaySegmentsAll(frames[current_frame]);

            while (cumulative_us < us_interval)
            {
                th.Update();
                cumulative_us += th.DeltaTime;
            }
            cumulative_us -= us_interval;
        }
        if (!keep_last_frame)
            DisplayClearAll();
    }

    void SegmentDisplayModule::DisplayAnimationOnceAll(const uint32_t** frames, size_t frame_count, size_t display_count, float target_fps, bool keep_last_frame, bool reverse)
    {

    }

    void SegmentDisplayModule::DisplayAnimationOnceAll(const uint32_t* frames, size_t frame_count, float target_fps, bool keep_last_frame, bool reverse)
    {

    }

    void SegmentDisplayModule::DisplayAnimationOnce(size_t index, const ArrayView<uint32_t>& frames, float target_fps, bool keep_last_frame, bool reverse)
    {

    }

    void SegmentDisplayModule::DisplayAnimationOnce(const SegmentDisplaySettings& device, const ArrayView<uint32_t>& frames, float target_fps, bool keep_last_frame, bool reverse)
    {

    }

    void SegmentDisplayModule::DisplayAnimationOnce(size_t index, const uint32_t* frames, size_t frame_count, float target_fps, bool keep_last_frame, bool reverse)
    {

    }

    void SegmentDisplayModule::DisplayAnimationOnce(const SegmentDisplaySettings& device, const uint32_t* frames, size_t frame_count, float target_fps, bool keep_last_frame, bool reverse)
    {

    }

}