#ifndef _GIR_GEN_MAIN_OPTION_GROUP_H_
#define _GIR_GEN_MAIN_OPTION_GROUP_H_

#include <glibmm/optiongroup.h>

namespace GirGen {

class MainOptionGroup : public Glib::OptionGroup
{
private:
    const std::string default_gir_path = "/usr/share/gir-1.0";

    bool on_option_namespace_mapping(const Glib::ustring& option_name, const Glib::ustring& value, bool has_value);

    static std::vector<std::string> split(const std::string &str, char delim);

public:
    MainOptionGroup();

    bool on_post_parse(Glib::OptionContext& context, Glib::OptionGroup& group) override;

    std::string gir_path;
    bool print_vfunc = false;
    bool print_enum = false;
    bool print_signal = false;
    Glib::ustring req_namespace;
    std::map<std::string, std::string> namespace_mapping;
    Glib::OptionGroup::vecstrings packages;
};

}

#endif /* _GIR_GEN_MAIN_OPTION_GROUP_H_ */
