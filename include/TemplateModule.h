#pragma once

#include <Module.h> // Assuming ZipsPicoLib is included in project.

namespace uazips
{

    class TemplateModule : public Module
    {
    public:
        TemplateModule();
        virtual ~TemplateModule();

        virtual void Initialize() override;
    };

}