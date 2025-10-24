#include <SegmentDisplayModule.h>
#include <Animations.h>
#include <util/FunctionUtils.h>
#include <hardware/Button.h>
#include <hardware/TimeDevice.h>
#include <ZipsLib.h>
#include <pico/multicore.h>

using namespace uazips;

static SegmentDisplayModule::SegmentDisplaySettings settings[3] = {
    SegmentDisplayModule::Create(pio0, 5, 4, 0, 7, 0, 0),
    SegmentDisplayModule::Create(pio0, 7, 6, 1, 7, 0, 0),
    SegmentDisplayModule::Create(pio0, 9, 8, 2, 7, 0, 0)
};

int main()
{
    BEGIN_SETUP();

    SegmentDisplayModule mod(settings, 3);

    Module::InitAll();

    mod.DisplayTextAll("test12345678");
    
    BEGIN_LOOP();
    while (1) {
        tight_loop_contents();
        LOG("step\n");
    }

    return 0;
}