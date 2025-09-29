#include <SegmentDisplayModule.h>
#include <Animations.h>
#include <ZipsLib.h>

uint32_t current_loop = 0;
constexpr uint32_t max_loop = 5000000;

bool loop_break()
{
    current_loop++;
    if (current_loop >= max_loop)
    {
        current_loop = 0;
        return 1;
    }
    return 0;
}

int main()
{
    BEGIN_SETUP();

    uazips::SegmentDisplayModule::SegmentDisplaySettings settings[2] = 
    {
        {
            .pio = pio0,
            .clk_pin = 4,
            .dio_pin = 5,
            .sm = 0,
            .brightness = 7,
            .colon = 0,
            .device_ptr = nullptr
        },
        {
            .pio = pio1,
            .clk_pin = 6,
            .dio_pin = 7,
            .sm = 1,
            .brightness = 7,
            .colon = 0,
            .device_ptr = nullptr
        }
    };

    uazips::SegmentDisplayModule mod(settings, 2);

    uazips::Module::InitAll();

    while (1)
    {
    mod.DisplayTextAll("test");
    LOG("Displaying \"test\"\n");
    sleep_ms(1000);
    mod.DisplayAnimationAll(uazips::animations::onedisplay::anim_looping, 5.f, &loop_break);
    mod.DisplayAnimationAll(uazips::animations::onedisplay::anim_fall, 1.f, &loop_break);
    
    
    sleep_ms(1000);
    }
    return 0;
}