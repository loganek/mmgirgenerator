# mmgirgenerator
mmgirgenerator is a new generator for [gtkmm](http://www.gtkmm.org) projects. In constrast to [h2def](https://git.gnome.org/browse/glibmm/tree/tools/defs_gen), which is currently used for generating .defs files, mmgirgenerator generates code based on information coming from [GObjec-Introspection](https://wiki.gnome.org/action/show/Projects/GObjectIntrospection) GIR files.

# Build instructions
#### Requirements:
 * glibmm-2.4
 * libxml\+\+-3.0

### CMake build
```shell
$ git clone git@github.com:loganek/mmgirgenerator.git
$ cd mmgirgenerator
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Generate definitions
```
 $ ./src/mmgirgenerator --help
Usage:
  mmgirgenerator [OPTION...] [GIR_PACKAGE...]

Help Options:
  -h, --help                      Show help options

Application Options:
  -g, --gir-path=PATH             Path to GIR directory
  --namespace=NAMESPACE           Processed namespace
  --namespace-mapping=MAPPING     Namespace mapping, MAPPING format: from_namespace:to_namespace[,...]
  --print-vfunc                   Print definitions of virtual methods
  --print-enum                    Print definitions of enums and flags
  --print-signal                  Print definitions of signals
  --print-property                Print definitions of properties
  --print-function                Print definitions of functions
```
### Example usage
Command below generates definitions for enumerations and flags from packages: GModule-2.0, GLib-2.0 and GObject-2.0.
```shell
./src/mmgirgenerator GModule-2.0 GLib-2.0 GObject-2.0 \
                     --namespace=GLib \
                     --namespace-mapping=GModule:GLib,GObject:GLib \
                     --print-enum
```
# Contributing
* [Open issue](https://github.com/loganek/mmgirgenerator/issues)
* [Create pull request](https://github.com/loganek/mmgirgenerator/pulls)

# Licensing
This project is developed under LGPL license. See [LICENSE](https://github.com/loganek/mmgirgenerator/blob/master/LICENSE) file for more details.
