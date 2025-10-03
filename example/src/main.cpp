#include <SegmentDisplayModule.h>
#include <Animations.h>
#include <util/FunctionUtils.h>
#include <hardware/Button.h>
#include <ZipsLib.h>

using namespace uazips;

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

    mod.DisplayTextAll("Testing");

    while (1) {}

    return 0;
}