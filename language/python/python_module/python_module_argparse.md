python module argparse
======================
Parser for command-line options, arguments and sub-commands

# ArgumentParser objects
class argparse.ArgumentParser(prog=None, usage=None, description=None, epilog=None, parents=[], formatter_class=argparse.HelpFormatter, prefix_chars='-', fromfile_prefix_chars=None, argument_default=None, conflict_handler='error', add_help=True)

Create a new ArgumentParser object. All parameters should be passed as keyword arguments. Each parameter has its own more detailed description below, but in short they are:

 - prog - The name of the program (default: sys.argv[0])
 - usage - The string describing the program usage (default: generated from arguments added to parser)
 - description - Text to display before the argument help (default: none)
 - epilog - Text to display after the argument help (default: none)
 - parents - A list of ArgumentParser objects whose arguments should also be included
 - formatter_class - A class for customizing the help output
 - prefix_chars - The set of characters that prefix optional arguments (default: ‘-‘)
 - fromfile_prefix_chars - The set of characters that prefix files from which additional arguments should be read (default: None)
 - argument_default - The global default value for arguments (default: None)
 - conflict_handler - The strategy for resolving conflicting optionals (usually unnecessary)
 - add_help - Add a -h/--help option to the parser (default: True)

The following sections describe how each of these are used.

# add_argument() method
ArgumentParser.add_argument(name or flags...[, action][, nargs][, const][, default][, type][, choices][, required][, help][, metavar][, dest])

Define how a single command-line argument should be parsed. Each parameter has its own more detailed description below, but in short they are:

 - name or flags - Either a name or a list of option strings, e.g. foo or -f, --foo.
 - action - The basic type of action to be taken when this argument is encountered at the command line.
 - nargs - The number of command-line arguments that should be consumed.
 - const - A constant value required by some action and nargs selections.
 - default - The value produced if the argument is absent from the command line.
 - type - The type to which the command-line argument should be converted.
 - choices - A container of the allowable values for the argument.
 - required - Whether or not the command-line option may be omitted (optionals only).
 - help - A brief description of what the argument does.
 - metavar - A name for the argument in usage messages.
 - dest - The name of the attribute to be added to the object returned by parse_args().

The following sections describe how each of these are used.

# parse_args() method
ArgumentParser.parse_args(args=None, namespace=None)

Convert argument strings to objects and assign them as attributes of the namespace. Return the populated namespace.

Previous calls to add_argument() determine exactly what objects are created and how they are assigned. See the documentation for add_argument() for details.

 - args - List of strings to parse. The default is taken from sys.argv.
 - namespace - An object to take the attributes. The default is a new empty Namespace object.

# 参考
 - [argparse](https://docs.python.org/2/library/argparse.html#)
