#include <SegmentDisplayModule.h>
#include <Animations.h>
#include <util/FunctionUtils.h>
#include <hardware/Button.h>
#include <hardware/TimeDevice.h>
#include <ZipsLib.h>
#include <pico/multicore.h>

using namespace uazips;

void core1()
{
    Event::HandleAllEvents(true);
}

int main()
{
    BEGIN_SETUP();

    uazips::SegmentDisplayModule::SegmentDisplaySettings settings[3] = 
    {
        {
            .pio = pio0,
            .clk_pin = 5,
            .dio_pin = 4,
            .sm = 0,
            .brightness = 7,
            .colon = 0,
            .is_upside_down = false,
            .device_ptr = nullptr
        },
        {
            .pio = pio0,
            .clk_pin = 7,
            .dio_pin = 6,
            .sm = 1,
            .brightness = 7,
            .colon = 0,
            .is_upside_down = false,
            .device_ptr = nullptr
        },
        {
            .pio = pio0,
            .clk_pin = 9,
            .dio_pin = 8,
            .sm = 2,
            .brightness = 7,
            .colon = 0,
            .is_upside_down = false,
            .device_ptr = nullptr
        }
    };

    uazips::SegmentDisplayModule mod(settings, 3);

    CountdownTimer timer;
    Button button = 13;

    uazips::Module::InitAll();

    multicore_launch_core1(&core1);

    mod.DisplayTextAll("123456789000");
    sleep_ms(2000);
    timer.Begin(5000);
    mod.DisplayAnimationAll(animations::threedisplay::anim_looping, 15, timer);

    BEGIN_LOOP();
    while (1) {}

    return 0;
}