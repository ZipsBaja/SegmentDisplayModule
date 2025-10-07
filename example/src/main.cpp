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

    CountdownTimer timer;
    Button button = 13;

    uazips::Module::InitAll();

    multicore_launch_core1(&core1);

    mod.DisplayTextAll("display");
    sleep_ms(2000);
    timer.Begin(5000);
    mod.DisplayAnimationAll(animations::twodisplay::anim_looping, 15, timer);

    BEGIN_LOOP();
    while (1) {}

    return 0;
}