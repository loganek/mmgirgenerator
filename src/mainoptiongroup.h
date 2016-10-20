#ifndef _GIR_GEN_MAIN_OPTION_GROUP_H_
#define _GIR_GEN_MAIN_OPTION_GROUP_H_

#include <glibmm/optiongroup.h>

namespace GirGen {

class MainOptionGroup : public Glib::OptionGroup
{
private:
    const std::string default_gir_path = "/usr/share/gir-1.0";

    bool parse_ok = true;

public:
    MainOptionGroup();

    bool on_post_parse(Glib::OptionContext& context, Glib::OptionGroup& group) override;
    void on_error(Glib::OptionContext& context, OptionGroup& group) override;

    bool is_parse_ok() const { return parse_ok; }

    std::string gir_path;
    Glib::OptionGroup::vecstrings packages;
};

}

#endif /* _GIR_GEN_MAIN_OPTION_GROUP_H_ */
