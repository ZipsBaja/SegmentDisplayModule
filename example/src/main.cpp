#include <SegmentDisplayModule.h>
#include <Animations.h>
#include <ZipsLib.h>

int main()
{
    stdio_init_all();
    BEGIN_SETUP();
    printf("Program Start\n");

    uazips::SegmentDisplayModule::SegmentDisplaySettings settings = 
    {
        .pio = pio0,
        .clk_pin = 4,
        .dio_pin = 5,
        .sm = 0,
        .brightness = 7,
        .colon = 0,
        .device_ptr = nullptr
    };

    uazips::SegmentDisplayModule mod(settings);

    uazips::Module::InitAll();

    mod.DisplayTextAll("test");

    mod.DisplayAnimation(settings, uazips::animations::onedisplay::anim_looping, 15.f, [](){return false;});

    BEGIN_LOOP();
    while (1) {
        printf("test\n");
    }

    printf("Program End\n");

    return 0;
}