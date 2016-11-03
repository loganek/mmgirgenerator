#include "repositoryloader.h"

#include <algorithm>

namespace GirGen {

template<typename T>
static T read_numeric_attribute(const xmlpp::Element* element, const std::string attribute_name, T default_value)
{
    if (auto attribute = element->get_attribute(attribute_name))
    {
        return static_cast<T>(std::stoll(attribute->get_value()));
    }
    else
    {
        return default_value;
    }
}

const xmlpp::Element* RepositoryLoader::to_element(const xmlpp::Node *node)
{
    return dynamic_cast<const xmlpp::Element*>(node);
}

TransferOwnership RepositoryLoader::transfer_ownership_from_string(const std::string &str)
{
    return (str == "full") ? TransferOwnership::Full : TransferOwnership::None;
}

EmissionStage RepositoryLoader::emission_stage_from_string(const std::string &str)
{
    if (str == "first")
    {
        return EmissionStage::First;
    }
    if (str == "last")
    {
        return EmissionStage::Last;
    }
    if (str == "cleanup")
    {
        return EmissionStage::Cleanup;
    }

    throw std::runtime_error("unknown emission stage '" + str + "'");
}

void RepositoryLoader::load_namespace(const xmlpp::Element *element, const std::shared_ptr<NamespaceInfo> &nspace)
{
    nspace->c_prefix = element->get_attribute_value("identifier-prefixes", "c");

    for (const auto& child : element->get_children())
    {
        if (child->get_name() == "alias")
        {
            nspace->aliases.push_back(load_alias(to_element(child)));
        }
        else if (child->get_name() == "constant")
        {
            nspace->constants.push_back(load_constant(to_element(child)));
        }
        else if (child->get_name() == "enumeration" || child->get_name() == "bitfield")
        {
            nspace->enumerations.push_back(load_enumeration(to_element(child)));
        }
        else if (child->get_name() == "function")
        {
            nspace->functions.push_back(load_function(to_element(child)));
        }
        else if (child->get_name() == "class" || child->get_name() == "record")
        {
            nspace->classes.push_back(load_class(to_element(child)));
        }
        else if (child->get_name() == "interface")
        {
            nspace->interfaces.push_back(load_interface(to_element(child)));
        }
    }
}

std::shared_ptr<AliasInfo> RepositoryLoader::load_alias(const xmlpp::Element *element)
{
    auto alias = std::make_shared<AliasInfo>();

    alias->name = element->get_attribute_value("name");
    alias->c_type = element->get_attribute_value("type", "c");

    alias->type = read_type(element);

    read_documentation(element, alias);

    return alias;
}

void RepositoryLoader::read_documentation(const xmlpp::Node *parent_node, const std::shared_ptr<IDocumentatable> &documentable)
{
    for (const auto& child : parent_node->get_children())
    {
        if (child->get_name() == "doc")
        {
            documentable->documentation = to_element(child)->get_first_child_text()->get_content();
        }
        if (child->get_name() == "doc-deprecated")
        {
            documentable->documentation_deprecated = to_element(child)->get_first_child_text()->get_content();
        }
    }
}

std::shared_ptr<ConstantInfo> RepositoryLoader::load_constant(const xmlpp::Element *element)
{
    auto constant = std::make_shared<ConstantInfo>();

    constant->name = element->get_attribute_value("name");
    constant->c_type = element->get_attribute_value("type", "c");

    read_documentation(element, constant);
    constant->type = read_type(element);

    return constant;
}

std::shared_ptr<EnumInfo> RepositoryLoader::load_enumeration(const xmlpp::Element *element)
{
    auto enumeration = std::make_shared<EnumInfo>();

    enumeration->name = element->get_attribute_value("name");
    enumeration->c_type = element->get_attribute_value("type", "c");
    enumeration->is_bitfield = element->get_name() == "bitfield";

    read_documentation(element, enumeration);

    for (const auto& child : element->get_children())
    {
        if (child->get_name() == "member")
        {
            auto member = std::make_shared<EnumInfo::MemberInfo>();

            read_documentation(child, member);

            auto child_element = to_element(child);
            member->name = child_element->get_attribute_value("name");
            member->value = child_element->get_attribute_value("value");
            member->c_identifier = child_element->get_attribute_value("identifier", "c");

            enumeration->members.push_back(member);
        }
    }

    return enumeration;
}

std::shared_ptr<FunctionInfo> RepositoryLoader::load_function(const xmlpp::Element *element)
{
    auto function = std::make_shared<FunctionInfo>();

    function->c_identifier = element->get_attribute_value("identifier", "c");

    load_callable(element, function);

    return function;
}

std::shared_ptr<SignalInfo> RepositoryLoader::load_signal(const xmlpp::Element *element)
{
    auto signal = std::make_shared<SignalInfo>();

    if (element->get_attribute("when"))
    {
        signal->when = emission_stage_from_string(element->get_attribute_value("when"));
    }

    load_callable(element, signal);

    return signal;
}

std::shared_ptr<ClassInfo> RepositoryLoader::load_class(const xmlpp::Element *element)
{
    auto class_info = std::make_shared<ClassInfo>();

    load_structure(element, class_info);

    for (const auto& child : element->get_children())
    {
        if (child->get_name() == "implements")
        {
            class_info->implemented_interfaces.push_back(to_element(child)->get_attribute_value("name"));
        }
        else if (child->get_name() == "property")
        {
            class_info->properties.push_back(load_property(to_element(child)));
        }
    }

    return class_info;
}

std::shared_ptr<InterfaceInfo> RepositoryLoader::load_interface(const xmlpp::Element *element)
{
    auto interface_info = std::make_shared<InterfaceInfo>();

    load_structure(element, interface_info);

    for (const auto& child : element->get_children())
    {
        if (child->get_name() == "prerequisite")
        {
            interface_info->prerequisites.push_back(to_element(child)->get_attribute_value("name"));
        }
        else if (child->get_name() == "property")
        {
            interface_info->properties.push_back(load_property(to_element(child)));
        }
    }

    return interface_info;
}

std::shared_ptr<PropertyInfo> RepositoryLoader::load_property(const xmlpp::Element *element)
{
    auto property_info = std::make_shared<PropertyInfo>();

    property_info->name = element->get_attribute_value("name");

    read_documentation(element, property_info);

    property_info->deprecated = read_numeric_attribute(element, "deprecated", false);
    property_info->writable = read_numeric_attribute(element, "writable", false);
    property_info->readable = read_numeric_attribute(element, "readable", true);
    property_info->construct_only = read_numeric_attribute(element, "construct-only", false);
    property_info->transfer_ownership = transfer_ownership_from_string(element->get_attribute_value("transfer-ownership"));
    property_info->type = read_type(element);

    return property_info;
}

void RepositoryLoader::load_callable(const xmlpp::Element *element, const std::shared_ptr<CallableInfo> &callable)
{
    read_documentation(element, callable);

    // TODO moved-to attribute should probably have precendence here
    callable->name = element->get_attribute_value("name");
    callable->throws = read_numeric_attribute<bool>(element, "throws", false);

    for (const auto& child : element->get_children())
    {
        if (child->get_name() == "return-value")
        {
            callable->return_value = load_return_value(child);
        }
        else if (child->get_name() == "parameters")
        {
            callable->parameters = load_parameters(child);
        }
    }
}

void RepositoryLoader::load_structure(const xmlpp::Element *element, const std::shared_ptr<StructureInfo> &structure)
{
    structure->name = element->get_attribute_value("name");

    if (element->get_attribute("type", "c"))
    {
        structure->c_type = element->get_attribute_value("type", "c");
    }
    else
    {
        structure->c_type = element->get_attribute_value("type-name", "glib");
    }

    structure->parent_name = element->get_attribute_value("parent");
    structure->is_abstract = read_numeric_attribute(element, "abstract", false);

    read_documentation(element, structure);

    for (const auto& child : element->get_children())
    {
        if (child->get_name() == "method" || child->get_name() == "virtual-method" || child->get_name() == "constructor")
        {
            auto fnc = load_function(to_element(child));

            fnc->is_virtual = child->get_name() == "virtual-method";
            fnc->is_constructor = child->get_name() == "constructor";

            structure->methods.push_back(fnc);
        }
        else if (child->get_name() == "function")
        {
            structure->functions.push_back(load_function(to_element(child)));
        }
        else if (child->get_name() == "signal")
        {
            structure->glib_signals.push_back(load_signal(to_element(child)));
        }
    }
}

std::vector<std::shared_ptr<CallableInfo::ParameterInfo>> RepositoryLoader::load_parameters(const xmlpp::Node *node)
{
    std::vector<std::shared_ptr<CallableInfo::ParameterInfo>> parameters;

    for (const auto& child : node->get_children())
    {
        if (child->get_name() == "parameter" || child->get_name() == "instance-parameter")
        {
            auto element = to_element(child);

            auto parameter = std::make_shared<CallableInfo::ParameterInfo>();

            parameter->transfer_ownership = transfer_ownership_from_string(element->get_attribute_value("transfer-ownership"));
            parameter->name = element->get_attribute_value("name");
            read_documentation(child, parameter);
            parameter->type = read_type(child);
            parameter->is_instance_param = child->get_name() == "instance-parameter";

            parameters.push_back(parameter);
        }
    }

    return parameters;
}

std::shared_ptr<CallableInfo::ReturnValue> RepositoryLoader::load_return_value(const xmlpp::Node *node)
{
    auto return_value = std::make_shared<CallableInfo::ReturnValue>();

    read_documentation(node, return_value);

    return_value->type = read_type(node);

    return return_value;
}

std::shared_ptr<TypeInfo> RepositoryLoader::read_type(const xmlpp::Node *parent_node)
{
    for (const auto& child : parent_node->get_children())
    {
        std::shared_ptr<TypeInfo> type;
        auto element = to_element(child);

        if (child->get_name() == "type")
        {
            type = std::make_shared<TypeInfo>();
            type->name = element->get_attribute_value("name");
        }
        else if (child->get_name() == "array")
        {
            auto element = to_element(child);

            auto arr_type = std::make_shared<ArrayInfo>();

            arr_type->name = element->get_attribute_value("name");
            arr_type->c_type = element->get_attribute_value("type", "c");
            arr_type->underlying_type = read_type(child);

            arr_type->is_zero_terminated = read_numeric_attribute(element, "length", 0);
            arr_type->is_zero_terminated = read_numeric_attribute(element, "fixed-size", 0);
            arr_type->is_zero_terminated = read_numeric_attribute(element, "zero-terminated", false);

            type = arr_type;
        }

        if (type)
        {
            std::string const_prefix("const");
            type->is_constant = !type->c_type.compare(0, const_prefix.size(), const_prefix);
            type->c_type = element->get_attribute_value("type", "c");

            return type;
        }

        if (child->get_name() == "varargs")
        {
            auto type = std::make_shared<TypeInfo>();

            type->name = "...";
            type->c_type = "...";

            return type;
        }
    }

    // TODO is it always the case?
    throw std::runtime_error("expected type node");
}

void RepositoryLoader::parse_gir_file(const std::string &gir_file, const std::shared_ptr<NamespaceCollection>& nspace_collection)
{
    parser.parse_file(gir_file);

    const auto node = parser.get_document()->get_root_node();

    for (const auto& child : node->get_children())
    {
        if (child->get_name() == "namespace")
        {
            std::string nspace_name = to_element(child)->get_attribute_value("name");

            if (nspace_collection->find(nspace_name) == nspace_collection->end())
            {
                (*nspace_collection)[nspace_name] = std::make_shared<NamespaceInfo>();
                (*nspace_collection)[nspace_name]->name = nspace_name;
            }

            load_namespace(to_element(child), (*nspace_collection)[nspace_name]);
        }
    }
}

}
