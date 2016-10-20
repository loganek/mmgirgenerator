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

    Glib::OptionEntry entry_remaining;
    entry_remaining.set_long_name(G_OPTION_REMAINING);

    add_entry_filename(entry_remaining, packages);
}

bool MainOptionGroup::on_post_parse(Glib::OptionContext&, Glib::OptionGroup&)
{
    if (!Glib::file_test(gir_path, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR))
    {
        std::cerr << "Path " << gir_path << " doesn't exist." << std::endl;
        return false;
    }

    for (const auto &package : packages)
    {
        if (!Glib::file_test(Glib::build_filename(gir_path, package + ".gir"), Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_REGULAR))
        {
            std::cerr << "Package " << package << " doesn't exist in the " << gir_path << " directory." << std::endl;
            return false;
        }
    }

    return true;
}

void MainOptionGroup::on_error(Glib::OptionContext&, Glib::OptionGroup& )
{
    parse_ok = false;
}

}
