python module argparse
======================
Parser for command-line options, arguments and sub-commands

命令行选项、参数和子命令解析器。



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

创建一个新的 `ArgumentParser` 对象。所有的参数都应当作为关键字参数传入。每个参数在下面都有它更详细的描述，但简而言之，它们是：

 - prog - 程序的名称（默认值：`os.path.basename(sys.argv[0])`）
 - usage - 描述程序用途的字符串（默认值：从添加到解析器的参数生成）
 - description - 在参数帮助文档之前显示的文本（默认值：无）
 - epilog - 在参数帮助文档之后显示的文本（默认值：无）
 - parents - 一个 `ArgumentParser` 对象的列表，它们的参数也应包含在内
 - formatter_class - 用于自定义帮助文档输出格式的类
 - prefix_chars - 可选参数的前缀字符集合（默认值：'-'）
 - fromfile_prefix_chars - 当需要从文件中读取其他参数时，用于标识文件名的前缀字符集合（默认值：`None`）
 - argument_default - 参数的全局默认值（默认值：`None`）
 - conflict_handler - 解决冲突选项的策略（通常是不必要的）
 - add_help - 为解析器添加一个 `-h`/`--help` 选项（默认值：`True`）
 - allow_abbrev - 如果缩写是无歧义的，则允许缩写长选项（默认值：`True`）
 - exit_on_error - 决定当错误发生时是否让 `ArgumentParser` 附带错误信息退出。（默认值: `True`）

在 3.5 版更改: 增加了 allow_abbrev 参数。
在 3.8 版更改: 在之前的版本中，allow_abbrev 还会禁用短旗标分组，例如 `-vv` 表示为 `-v -v`。
在 3.9 版更改: 添加了 exit_on_error 形参。

以下部分描述这些参数如何使用。



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

定义单个的命令行参数应当如何解析。每个形参都在下面有它自己更多的描述，长话短说有：

 - name or flags - 一个命名或者一个选项字符串的列表，例如 `foo` 或 `-f`, `--foo`。
 - action - 当参数在命令行中出现时使用的动作基本类型。
 - nargs - 命令行参数应当消耗的数目。
 - const - 被一些 `action` 和 `nargs` 选择所需求的常数。
 - default - 当参数未在命令行中出现并且也不存在于命名空间对象时所产生的值。
 - type - 命令行参数应当被转换成的类型。
 - choices - 可用的参数的容器。
 - required - 此命令行选项是否可省略（仅选项可用）。
 - help - 一个此选项作用的简单描述。
 - metavar - 在使用方法消息中使用的参数值示例。
 - dest - 被添加到 `parse_args()` 所返回对象上的属性名。

以下部分描述这些参数如何使用。

## name or flags
`add_argument()` 方法必须知道它是否是一个选项，例如 `-f` 或 `--foo`，或是一个位置参数，例如一组文件名。 第一个传递给 `add_argument()` 的参数必须是一系列旗标或者是一个简单的参数名。

例如，可选参数可以被这样创建:
```python
>>> parser.add_argument('-f', '--foo')
```

而位置参数可以这么创建:
```python
>>> parser.add_argument('bar')
```

当 parse_args() 被调用，选项会以 `-` 前缀识别，剩下的参数则会被假定为位置参数:
```python
>>> parser = argparse.ArgumentParser(prog='PROG')
>>> parser.add_argument('-f', '--foo')
>>> parser.add_argument('bar')
>>> parser.parse_args(['BAR'])
Namespace(bar='BAR', foo=None)
>>> parser.parse_args(['BAR', '--foo', 'FOO'])
Namespace(bar='BAR', foo='FOO')
>>> parser.parse_args(['--foo', 'FOO'])
usage: PROG [-h] [-f FOO] bar
PROG: error: the following arguments are required: bar
```



# parse_args() method
ArgumentParser.parse_args(args=None, namespace=None)

Convert argument strings to objects and assign them as attributes of the namespace. Return the populated namespace.

Previous calls to add_argument() determine exactly what objects are created and how they are assigned. See the documentation for add_argument() for details.

 - args - List of strings to parse. The default is taken from sys.argv.
 - namespace - An object to take the attributes. The default is a new empty Namespace object.

将参数字符串转换为对象并将其设为命名空间的属性。 返回带有成员的命名空间。

之前对 `add_argument()` 的调用决定了哪些对象被创建以及它们如何被赋值。 请参阅 `add_argument()` 的文档了解详情。

 - args - 要解析的字符串列表。 默认值是从 `sys.argv` 获取。
 - namespace - 用于获取属性的对象。 默认值是一个新的空 `Namespace` 对象。



# 参考
 - [argparse](https://docs.python.org/2/library/argparse.html)
 - [argparse --- 命令行选项、参数和子命令解析器](https://docs.python.org/zh-cn/3/library/argparse.html)
