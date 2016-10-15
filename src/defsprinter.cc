#include "defsprinter.h"

#include <iostream>

namespace GirGen {

void DefsPrinter::print_enums() const
{
    for (std::shared_ptr<EnumInfo> enum_info : nspace->enumerations)
    {
        if (enum_info->is_bitfield)
        {
            std::cout << "(define-flags-extended " << enum_info->name << std::endl;
        }
        else
        {
            std::cout << "(define-enum-extended " << enum_info->name << std::endl;
        }
        std::cout << "  (in-module \"Gst\")" << std::endl;
        std::cout << "  (c-name \"" << enum_info->c_type << "\")" << std::endl;
        std::cout << "  (values" << std::endl;

        for (std::shared_ptr<EnumInfo::MemberInfo> member : enum_info->members)
        {
            std::cout << "    '(\"" << member->name << "\" \"" << member->c_identifier << "\" \"" << member->value << "\")" << std::endl;
        }

        std::cout << "  )" << std::endl;
        std::cout << ")" << std::endl;
        std::cout << std::endl;
    }
}

void DefsPrinter::print_virtual_methods() const
{
    for (std::shared_ptr<ClassInfo> class_info : nspace->classes)
    {
        print_virtual_methods_of_structure(class_info, class_info->c_type);
    }

    for (std::shared_ptr<InterfaceInfo> interface_info : nspace->interfaces)
    {
        print_virtual_methods_of_structure(interface_info, interface_info->c_type);
    }
}

void DefsPrinter::print_virtual_methods_of_structure(const std::shared_ptr<StructureInfo> &structure, const std::string &parent_c_type) const
{
    for (std::shared_ptr<FunctionInfo> fnc : structure->methods)
    {
        if (!fnc->is_virtual)
        {
            continue;
        }

        std::cout << "(define-vfunc " << fnc->name << std::endl;
        std::cout << "  (of-object \"" << parent_c_type << "\")" << std::endl;
        std::cout << "  (return-type \"" << fnc->return_value->type->c_type << "\")" << std::endl;

        // > 1, because we don't consider instance-parameter
        if (fnc->parameters.size() > 1 || fnc->throws)
        {
            std::cout << "  (parameters" << std::endl;
            for (std::shared_ptr<FunctionInfo::ParameterInfo> parameter : fnc->parameters)
            {
                if (parameter->is_instance_param)
                {
                    continue;
                }

                std::cout << "   '(\"" << parameter->type->c_type << "\" \"" << parameter->name << "\")" << std::endl;
            }
            if (fnc->throws)
            {
                std::cout << "   '(\"GError**\" \"error\")" << std::endl;
            }

            std::cout << "  )" << std::endl;
        }

        std::cout << ")" << std::endl << std::endl;
    }
}

}
