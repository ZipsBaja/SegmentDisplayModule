#include <TemplateModule.h>

#include <pico/stdlib.h>
#include <stdio.h>


int main()
{
    stdio_init_all();
    
    printf("Program Start\n");

    uazips::TemplateModule mod;
    mod.Initialize();

    printf("Program End\n");

    return 0;
}