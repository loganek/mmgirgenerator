#include "mainoptiongroup.h"

#include <glibmm.h>
#include <iostream>

namespace GirGen {

MainOptionGroup::MainOptionGroup()
    : Glib::OptionGroup("main-group", "main application group", "main application parameters")
{
    Glib::OptionEntry gir_path_entry;
    gir_path_entry.set_long_name("gir-path");
    gir_path_entry.set_short_name('g');
    gir_path_entry.set_description("Path to GIR directory");
    gir_path_entry.set_arg_description("PATH");
    gir_path = default_gir_path;
    add_entry_filename(gir_path_entry, gir_path);

    Glib::OptionEntry namespace_entry;
    namespace_entry.set_long_name("namespace");
    namespace_entry.set_description("Processed namespace");
    namespace_entry.set_arg_description("NAMESPACE");
    add_entry(namespace_entry, req_namespace);

    Glib::OptionEntry mapping_entry;
    mapping_entry.set_long_name("namespace-mapping");
    mapping_entry.set_description("Namespace mapping, MAPPING format: from_namespace:to_namespace[,...]");
    mapping_entry.set_arg_description("MAPPING");
    add_entry(mapping_entry, sigc::mem_fun(*this, &MainOptionGroup::on_option_namespace_mapping));

    Glib::OptionEntry vfunc_entry;
    vfunc_entry.set_long_name("print-vfunc");
    vfunc_entry.set_description("Print definitions of virtual methods");
    add_entry(vfunc_entry, print_vfunc);

    Glib::OptionEntry enum_entry;
    enum_entry.set_long_name("print-enum");
    enum_entry.set_description("Print definitions of enums and flags");
    add_entry(enum_entry, print_enum);

    Glib::OptionEntry signal_entry;
    signal_entry.set_long_name("print-signal");
    signal_entry.set_description("Print definitions of signals");
    add_entry(signal_entry, print_signal);

    Glib::OptionEntry entry_remaining;
    entry_remaining.set_long_name(G_OPTION_REMAINING);
    add_entry_filename(entry_remaining, packages);
}

std::vector<std::string> MainOptionGroup::split(const std::string &str, char delim)
{
    std::stringstream ss(str);
    std::string element;
    std::vector<std::string> tokens;

    while (getline(ss, element, delim))
    {
        tokens.push_back(element);
    }

    return tokens;
}

bool MainOptionGroup::on_option_namespace_mapping(const Glib::ustring& option_name, const Glib::ustring& value, bool has_value)
{
    if (option_name != "--namespace-mapping")
    {
        throw Glib::OptionError(Glib::OptionError::UNKNOWN_OPTION, "on_option_namespace_mapping called with unexpected option_name: " + option_name);
    }

    if (!has_value || value.empty())
    {
        throw Glib::OptionError(Glib::OptionError::BAD_VALUE, "expected value for " + option_name);
    }

    for (auto entry : split(value, ','))
    {
        auto vect = split(entry, ':');

        if (vect.size() != 2)
        {
            throw Glib::OptionError(Glib::OptionError::BAD_VALUE, "Invalid mapping format.");
        }
        namespace_mapping.insert({vect[0], vect[1]});
    }

    return !namespace_mapping.empty();
}

bool MainOptionGroup::on_post_parse(Glib::OptionContext&, Glib::OptionGroup&)
{
    if (!Glib::file_test(gir_path, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR))
    {
        throw Glib::OptionError(Glib::OptionError::BAD_VALUE, "Path " + gir_path + " doesn't exist.");
    }

    if (packages.empty())
    {
        throw Glib::OptionError(Glib::OptionError::BAD_VALUE, "At least one package muts be specified.");
    }

    for (const auto &package : packages)
    {
        if (!Glib::file_test(Glib::build_filename(gir_path, package + ".gir"), Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_REGULAR))
        {
            throw Glib::OptionError(Glib::OptionError::BAD_VALUE, "Package " + package + " doesn't exist in the " + gir_path + " directory.");
        }
    }

    if (req_namespace.empty())
    {
        throw Glib::OptionError(Glib::OptionError::BAD_VALUE, "Namespace can't be empty. See help for the details.");
    }

    return true;
}

}
