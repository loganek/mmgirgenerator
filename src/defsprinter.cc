#include "defsprinter.h"

#include <iostream>
#include <algorithm>

namespace GirGen {

static std::string prepare_c_type(std::string c_type)
{
    std::string const_str = "const ";
    auto pos = c_type.find(const_str);
    if (pos != std::string::npos)
    {
        c_type[pos + const_str.size()-1] = '-';
    }

    c_type.erase(std::remove_if(c_type.begin(), c_type.end(), ::isspace), c_type.end());
    return c_type;
}

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

        std::string module = namespace_mapping.find(nspace->name) != namespace_mapping.end() ?
                    namespace_mapping.at(nspace->name) : nspace->name;

        std::cout << "  (in-module \"" << module << "\")" << std::endl;
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
        print_virtual_methods(class_info->functions, class_info->c_type);
    }

    for (std::shared_ptr<InterfaceInfo> interface_info : nspace->interfaces)
    {
        print_virtual_methods(interface_info->functions, interface_info->c_type);
    }
}

void DefsPrinter::print_signals() const
{
    for (std::shared_ptr<ClassInfo> class_info : nspace->classes)
    {
        print_signals(class_info->glib_signals, class_info->c_type);
    }

    for (std::shared_ptr<InterfaceInfo> interface_info : nspace->interfaces)
    {
        print_signals(interface_info->glib_signals, interface_info->c_type);
    }
}

void DefsPrinter::print_virtual_methods(const std::vector<std::shared_ptr<FunctionInfo>> &functions, const std::string &parent_c_type) const
{
    for (std::shared_ptr<FunctionInfo> fnc : functions)
    {
        if (!fnc->is_virtual)
        {
            continue;
        }

        std::cout << "(define-vfunc " << fnc->name << std::endl;
        std::cout << "  (of-object \"" << parent_c_type << "\")" << std::endl;
        std::cout << "  (return-type \"" << prepare_c_type(fnc->return_value->type->c_type) << "\")" << std::endl;

        print_callable_parameters(fnc);

        std::cout << ")" << std::endl << std::endl;
    }
}

static std::string to_string(EmissionStage when)
{
    switch (when)
    {
    case EmissionStage::Cleanup:
        return "cleanup";
    case EmissionStage::First:
        return "first";
    case EmissionStage::Last:
        return "last";
    }

    throw std::runtime_error("unexpected emission stage value");
}

void DefsPrinter::print_signals(const std::vector<std::shared_ptr<SignalInfo>> &signal_objects, const std::string &parent_c_type) const
{
    for (std::shared_ptr<SignalInfo> sgnl : signal_objects)
    {
        std::cout << "(define-signal " << sgnl->name << std::endl;
        std::cout << "  (of-object \"" << parent_c_type << "\")" << std::endl;
        std::cout << "  (return-type \"" << sgnl->return_value->type->c_type << "\")" << std::endl;
        std::cout << "  (when \"" << to_string(sgnl->when) << "\")" << std::endl;

        // All signal parameters that are registered as GTK_TYPE_STRING are actually const gchar*.
        print_callable_parameters(sgnl, true);

        std::cout << ")" << std::endl << std::endl;
    }
}

std::string DefsPrinter::get_c_type_name(const std::shared_ptr<TypeInfo>& type_info) const
{
    if (!type_info->c_type.empty())
    {
        return type_info->c_type;
    }

    auto enum_info = nspace_collection->find_enum_by_name(type_info->name, nspace->name);
    if (enum_info)
    {
        return enum_info->c_type;
    }

    auto structure_info = nspace_collection->find_structure_by_name(type_info->name, nspace->name);
    if (structure_info)
    {
        return structure_info->c_type + "*";
    }

    auto alias_info = nspace_collection->find_alias_by_name(type_info->name, nspace->name);
    if (alias_info)
    {
        return alias_info->c_type;
    }

    // TODO is array
    if (type_info->name == "utf8")
    {
        return "gchar*";
    }

    throw std::runtime_error("unknown type " + type_info->name); // TODO warning instead of exception
}

void DefsPrinter::print_callable_parameters(const std::shared_ptr<CallableInfo> &callable, bool force_conts_string) const
{
    // TODO might instance_param appear as non-first parameter?
    bool has_instance_param = callable->parameters.size() > 0 && callable->parameters[0]->is_instance_param;

    if (callable->parameters.size() > has_instance_param || callable->throws)
    {
        std::cout << "  (parameters" << std::endl;
        for (std::shared_ptr<FunctionInfo::ParameterInfo> parameter : callable->parameters)
        {
            if (parameter->is_instance_param)
            {
                continue;
            }

            std::string c_type;
            auto type = parameter->type;
            bool is_array = false;
            if (auto arr_type = std::dynamic_pointer_cast<ArrayInfo>(type))
            {
                is_array = true;
                if (!arr_type->c_type.empty()) c_type = arr_type->c_type;
                else if (!arr_type->name.empty()) c_type = get_c_type_name(arr_type);
                else type = arr_type->underlying_type;
            }

            if (c_type.empty())
            {
                c_type = type->c_type.empty() ?
                        get_c_type_name(type) : parameter->type->c_type;
                if (is_array) c_type += "*";
            }

            if (force_conts_string && c_type == "gchar*")
                c_type = "const-gchar*";

            std::cout << "   '(\"" << prepare_c_type(c_type) << "\" \"" << parameter->name << "\")" << std::endl;
        }
        if (callable->throws)
        {
            std::cout << "   '(\"GError**\" \"error\")" << std::endl;
        }

        std::cout << "  )" << std::endl;
    }
}

std::string DefsPrinter::get_property_type(const std::shared_ptr<TypeInfo>& type_info) const
{
    // TODO this function have to be re-implemented, if is used in the future.

    static std::map<std::string, std::string> type_map = {
        { "boolean", "GParamBoolean" },
        { "double", "GParamDouble" },
        { "float", "GParamFloat" },
        { "GType", "GParamGType" },
        { "int", "GParamInt" },
        { "gint64", "GParamInt64" },
        { "unsigned", "GParamUInt" },
        { "guint64", "GParamUInt64" },
        { "unsigned long", "GParamULong" },
        { "GVariant", "GParamVariant" },
        { "gchar*", "GParamString" },

    };

    std::string c_type = get_c_type_name(type_info);

    auto it = type_map.find(c_type);
    if (it != type_map.end())
    {
        return it->second;
    }

    /*
    "GParamUnichar"
    "GstParamFraction"
    "GParamEnum"
    "GParamFlags"
    "GParamBoxed"
    "GParamObject
    */

    if (c_type.back() == '*')
    {
        return "GParamPointer";
    }

    throw std::runtime_error("Don't know how to map property type " + c_type);
}

void DefsPrinter::print_properties() const
{
    for (auto class_info : nspace->classes)
    {
        print_properties(class_info->properties, class_info->c_type);
    }
    for (auto interface_info : nspace->interfaces)
    {
        print_properties(interface_info->properties, interface_info->c_type);
    }
}

void DefsPrinter::print_properties(const std::vector<std::shared_ptr<PropertyInfo>>& properties, const std::string& parent_ctype) const
{
    for (auto property : properties)
    {
        std::string doc = property->documentation;
        std::replace(doc.begin(), doc.end(), '\n', ' '), doc.end();
        std::replace(doc.begin(), doc.end(), '"', '\''), doc.end();

        std::cout << "(define-property " << property->name << std::endl;
        std::cout << "  (of-object \"" << parent_ctype << "\")" << std::endl;

        // TODO does the generator use this line?
        //std::cout << "  (prop-type\"" << get_property_type(property->type) << "\")" << std::endl;

        if (!doc.empty())
        {
            std::cout << "  (docs \"" << doc << "\")" << std::endl;
        }

        std::cout << "  (readable #" << (property->readable ? 't' : 'f') << ")" << std::endl;
        std::cout << "  (writable #" << (property->writable ? 't' : 'f') << ")" << std::endl;
        std::cout << "  (construct-only #" << (property->construct_only ? 't' : 'f') << ")" << std::endl;

        if (property->deprecated)
        {
            std::cout << "  (deprecated #t)" << std::endl;
        }
        std::cout << ")" << std::endl << std::endl;
    }
}

void DefsPrinter::print_free_functions() const
{
    for (const auto& fnc : nspace->functions)
    {
        std::cout << "(define-function " << fnc->c_identifier << std::endl;
        std::cout << "  (c-name \"" << fnc->c_identifier << "\")" << std::endl;

        std::cout << "  (return-type \"" << prepare_c_type(fnc->return_value->type->c_type) << "\")" << std::endl;

        print_callable_parameters(fnc);

        std::cout << ")" << std::endl << std::endl;
    }
}

}
