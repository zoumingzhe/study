python str
==========

# str.capitalize()
返回原字符串的副本，其首个字符大写，其余为小写。

在 3.8 版更改: 第一个字符现在被放入了 titlecase 而不是 uppercase。 这意味着复合字母类字符将只有首个字母改为大写，而再不是全部字符大写。

# str.casefold()
返回原字符串消除大小写的副本。 消除大小写的字符串可用于忽略大小写的匹配。

消除大小写类似于转为小写，但是更加彻底一些，因为它会移除字符串中的所有大小写变化形式。 例如，德语小写字母 'ß' 相当于 "ss"。 由于它已经是小写了，lower() 不会对 'ß' 做任何改变；而 casefold() 则会将其转换为 "ss"。

消除大小写算法的描述请参见 Unicode 标准的 3.13 节。

3.3 新版功能.

# str.center(width[, fillchar])
返回长度为 width 的字符串，原字符串在其正中。 使用指定的 fillchar 填充两边的空位（默认使用 ASCII 空格符）。 如果 width 小于等于 len(s) 则返回原字符串的副本。

# str.count(sub[, start[, end]])
返回子字符串 sub 在 [start, end] 范围内非重叠出现的次数。 可选参数 start 与 end 会被解读为切片表示法。

# str.encode(encoding='utf-8', errors='strict')
返回原字符串编码为字节串对象的版本。 默认编码为 'utf-8'。 可以给出 errors 来设置不同的错误处理方案。 errors 的默认值为 'strict'，表示编码错误会引发 UnicodeError。 其他可用的值为 'ignore', 'replace', 'xmlcharrefreplace', 'backslashreplace' 以及任何其他通过 codecs.register_error() 注册的值，请参阅 错误处理方案 小节。 要查看可用的编码列表，请参阅 标准编码 小节。

默认情况下，为了获得最佳性能，不会检测 errors 参数，而只在首次编码错误时用到它。若要检测 errors ，请启用 Python开发模式 或用 调试版本 。

在 3.1 版更改: 加入了对关键字参数的支持。

在 3.9 版更改: 现在，仅在开发模式和 调试模式 下才会检测 errors。

# str.endswith(suffix[, start[, end]])
如果字符串以指定的 suffix 结束返回 True，否则返回 False。 suffix 也可以为由多个供查找的后缀构成的元组。 如果有可选项 start，将从所指定位置开始检查。 如果有可选项 end，将在所指定位置停止比较。

# str.expandtabs(tabsize=8)
返回字符串的副本，其中所有的制表符会由一个或多个空格替换，具体取决于当前列位置和给定的制表符宽度。 每 tabsize 个字符设为一个制表位（默认值 8 时设定的制表位在列 0, 8, 16 依次类推）。 要展开字符串，当前列将被设为零并逐一检查字符串中的每个字符。 如果字符为制表符 (\t)，则会在结果中插入一个或多个空格符，直到当前列等于下一个制表位。 （制表符本身不会被复制。） 如果字符为换行符 (\n) 或回车符 (\r)，它会被复制并将当前列重设为零。 任何其他字符会被不加修改地复制并将当前列加一，不论该字符在被打印时会如何显示。

>>>
'01\t012\t0123\t01234'.expandtabs()
'01      012     0123    01234'
'01\t012\t0123\t01234'.expandtabs(4)
'01  012 0123    01234'
# str.find(sub[, start[, end]])
返回子字符串 sub 在 s[start:end] 切片内被找到的最小索引。 可选参数 start 与 end 会被解读为切片表示法。 如果 sub 未被找到则返回 -1。

备注 find() 方法应该只在你需要知道 sub 所在位置时使用。 要检查 sub 是否为子字符串，请使用 in 操作符:
>>>
'Py' in 'Python'
True
# str.format(*args, **kwargs)
执行字符串格式化操作。 调用此方法的字符串可以包含字符串字面值或者以花括号 {} 括起来的替换域。 每个替换域可以包含一个位置参数的数字索引，或者一个关键字参数的名称。 返回的字符串副本中每个替换域都会被替换为对应参数的字符串值。

>>>
"The sum of 1 + 2 is {0}".format(1+2)
'The sum of 1 + 2 is 3'
请参阅 格式字符串语法 了解有关可以在格式字符串中指定的各种格式选项的说明。

备注 当使用 n 类型 (例如: '{:n}'.format(1234)) 来格式化数字 (int, float, complex, decimal.Decimal 及其子类) 的时候，该函数会临时性地将 LC_CTYPE 区域设置为 LC_NUMERIC 区域以解码 localeconv() 的 decimal_point 和 thousands_sep 字段，如果它们是非 ASCII 字符或长度超过 1 字节的话，并且 LC_NUMERIC 区域会与 LC_CTYPE 区域不一致。 这个临时更改会影响其他线程。
在 3.7 版更改: 当使用 n 类型格式化数字时，该函数在某些情况下会临时性地将 LC_CTYPE 区域设置为 LC_NUMERIC 区域。

# str.format_map(mapping)
类似于 str.format(**mapping)，不同之处在于 mapping 会被直接使用而不是复制到一个 dict。 适宜使用此方法的一个例子是当 mapping 为 dict 的子类的情况：

>>>
class Default(dict):
    def __missing__(self, key):
        return key

'{name} was born in {country}'.format_map(Default(name='Guido'))
'Guido was born in country'
3.2 新版功能.

# str.index(sub[, start[, end]])
类似于 find()，但在找不到子字符串时会引发 ValueError。

# str.isalnum()
如果字符串中的所有字符都是字母或数字且至少有一个字符，则返回 True ， 否则返回 False 。 如果 c.isalpha() ， c.isdecimal() ， c.isdigit() ，或 c.isnumeric() 之中有一个返回 True ，则字符``c``是字母或数字。

# str.isalpha()
如果字符串中的所有字符都是字母，并且至少有一个字符，返回 True ，否则返回 False 。字母字符是指那些在 Unicode 字符数据库中定义为 "Letter" 的字符，即那些具有 "Lm"、"Lt"、"Lu"、"Ll" 或 "Lo" 之一的通用类别属性的字符。 注意，这与 Unicode 标准中定义的"字母"属性不同。

# str.isascii()
如果字符串为空或字符串中的所有字符都是 ASCII ，返回 True ，否则返回 False 。ASCII 字符的码点范围是 U+0000-U+007F 。

3.7 新版功能.

# str.isdecimal()
如果字符串中的所有字符都是十进制字符且该字符串至少有一个字符，则返回 True ， 否则返回 False 。十进制字符指那些可以用来组成10进制数字的字符，例如 U+0660 ，即阿拉伯字母数字0 。 严格地讲，十进制字符是 Unicode 通用类别 "Nd" 中的一个字符。

# str.isdigit()
如果字符串中的所有字符都是数字，并且至少有一个字符，返回 True ，否则返回 False 。 数字包括十进制字符和需要特殊处理的数字，如兼容性上标数字。这包括了不能用来组成 10 进制数的数字，如 Kharosthi 数。 严格地讲，数字是指属性值为 Numeric_Type=Digit 或 Numeric_Type=Decimal 的字符。

# str.isidentifier()
如果字符串是有效的标识符，返回 True ，依据语言定义， 标识符和关键字 节。

调用 keyword.iskeyword() 来检测字符串 s 是否为保留标识符，例如 def 和 class。

示例：

>>>
from keyword import iskeyword

'hello'.isidentifier(), iskeyword('hello')
(True, False)
'def'.isidentifier(), iskeyword('def')
(True, True)
# str.islower()
如果字符串中至少有一个区分大小写的字符 4 且此类字符均为小写则返回 True ，否则返回 False 。

# str.isnumeric()
如果字符串中至少有一个字符且所有字符均为数值字符则返回 True ，否则返回 False 。 数值字符包括数字字符，以及所有在 Unicode 中设置了数值特性属性的字符，例如 U+2155, VULGAR FRACTION ONE FIFTH。 正式的定义为：数值字符就是具有特征属性值 Numeric_Type=Digit, Numeric_Type=Decimal 或 Numeric_Type=Numeric 的字符。

# str.isprintable()
如果字符串中所有字符均为可打印字符或字符串为空则返回 True ，否则返回 False 。 不可打印字符是在 Unicode 字符数据库中被定义为 "Other" 或 "Separator" 的字符，例外情况是 ASCII 空格字符 (0x20) 被视作可打印字符。 （请注意在此语境下可打印字符是指当对一个字符串发起调用 repr() 时不必被转义的字符。 它们与字符串写入 sys.stdout 或 sys.stderr 时所需的处理无关。）

# str.isspace()
如果字符串中只有空白字符且至少有一个字符则返回 True ，否则返回 False 。

空白 字符是指在 Unicode 字符数据库 (参见 unicodedata) 中主要类别为 Zs ("Separator, space") 或所属双向类为 WS, B 或 S 的字符。

# str.istitle()
如果字符串中至少有一个字符且为标题字符串则返回 True ，例如大写字符之后只能带非大写字符而小写字符必须有大写字符打头。 否则返回 False 。

# str.isupper()
如果字符串中至少有一个区分大小写的字符 4 且此类字符均为大写则返回 True ，否则返回 False 。

>>>
'BANANA'.isupper()
True
'banana'.isupper()
False
'baNana'.isupper()
False
' '.isupper()
False
# str.join(iterable)
返回一个由 iterable 中的字符串拼接而成的字符串。 如果 iterable 中存在任何非字符串值包括 bytes 对象则会引发 TypeError。 调用该方法的字符串将作为元素之间的分隔。

# str.ljust(width[, fillchar])
返回长度为 width 的字符串，原字符串在其中靠左对齐。 使用指定的 fillchar 填充空位 (默认使用 ASCII 空格符)。 如果 width 小于等于 len(s) 则返回原字符串的副本。

# str.lower()
返回原字符串的副本，其所有区分大小写的字符 4 均转换为小写。

所用转换小写算法的描述请参见 Unicode 标准的 3.13 节。

# str.lstrip([chars])
返回原字符串的副本，移除其中的前导字符。 chars 参数为指定要移除字符的字符串。 如果省略或为 None，则 chars 参数默认移除空白符。 实际上 chars 参数并非指定单个前缀；而是会移除参数值的所有组合:

>>>
'   spacious   '.lstrip()
'spacious   '
'www.example.com'.lstrip('cmowz.')
'example.com'
参见 str.removeprefix() ，该方法将删除单个前缀字符串，而不是全部给定集合中的字符。 例如:

>>>
'Arthur: three!'.lstrip('Arthur: ')
'ee!'
'Arthur: three!'.removeprefix('Arthur: ')
'three!'
static str.maketrans(x[, y[, z]])
此静态方法返回一个可供 str.translate() 使用的转换对照表。

如果只有一个参数，则它必须是一个将 Unicode 码位序号（整数）或字符（长度为 1 的字符串）映射到 Unicode 码位序号、（任意长度的）字符串或 None 的字典。 字符键将会被转换为码位序号。

如果有两个参数，则它们必须是两个长度相等的字符串，并且在结果字典中，x 中每个字符将被映射到 y 中相同位置的字符。 如果有第三个参数，它必须是一个字符串，其中的字符将在结果中被映射到 None。

# str.partition(sep)
在 sep 首次出现的位置拆分字符串，返回一个 3 元组，其中包含分隔符之前的部分、分隔符本身，以及分隔符之后的部分。 如果分隔符未找到，则返回的 3 元组中包含字符本身以及两个空字符串。

# str.removeprefix(prefix, /)
如果字符串以 prefix 字符串开头，返回 string[len(prefix):]。 否则，返回原始字符串的副本：

>>>
'TestHook'.removeprefix('Test')
'Hook'
'BaseTestCase'.removeprefix('Test')
'BaseTestCase'
3.9 新版功能.

# str.removesuffix(suffix, /)
如果字符串以 suffix 字符串结尾，并且 suffix 非空，返回 string[:-len(suffix)]。 否则，返回原始字符串的副本:

>>>
'MiscTests'.removesuffix('Tests')
'Misc'
'TmpDirMixin'.removesuffix('Tests')
'TmpDirMixin'
3.9 新版功能.

# str.replace(old, new[, count])
返回字符串的副本，其中出现的所有子字符串 old 都将被替换为 new。 如果给出了可选参数 count，则只替换前 count 次出现。

# str.rfind(sub[, start[, end]])
返回子字符串 sub 在字符串内被找到的最大（最右）索引，这样 sub 将包含在 s[start:end] 当中。 可选参数 start 与 end 会被解读为切片表示法。 如果未找到则返回 -1。

# str.rindex(sub[, start[, end]])
类似于 rfind()，但在子字符串 sub 未找到时会引发 ValueError。

# str.rjust(width[, fillchar])
返回长度为 width 的字符串，原字符串在其中靠右对齐。 使用指定的 fillchar 填充空位 (默认使用 ASCII 空格符)。 如果 width 小于等于 len(s) 则返回原字符串的副本。

# str.rpartition(sep)
在 sep 最后一次出现的位置拆分字符串，返回一个 3 元组，其中包含分隔符之前的部分、分隔符本身，以及分隔符之后的部分。 如果分隔符未找到，则返回的 3 元组中包含两个空字符串以及字符串本身。

# str.rsplit(sep=None, maxsplit=- 1)
返回一个由字符串内单词组成的列表，使用 sep 作为分隔字符串。 如果给出了 maxsplit，则最多进行 maxsplit 次拆分，从 最右边 开始。 如果 sep 未指定或为 None，任何空白字符串都会被作为分隔符。 除了从右边开始拆分，rsplit() 的其他行为都类似于下文所述的 split()。

# str.rstrip([chars])
返回原字符串的副本，移除其中的末尾字符。 chars 参数为指定要移除字符的字符串。 如果省略或为 None，则 chars 参数默认移除空白符。 实际上 chars 参数并非指定单个后缀；而是会移除参数值的所有组合:

>>>
'   spacious   '.rstrip()
'   spacious'
'mississippi'.rstrip('ipz')
'mississ'
要删除单个后缀字符串，而不是全部给定集合中的字符，请参见 str.removesuffix() 方法。 例如:

>>>
'Monty Python'.rstrip(' Python')
'M'
'Monty Python'.removesuffix(' Python')
'Monty'
# str.split(sep=None, maxsplit=- 1)
返回一个由字符串内单词组成的列表，使用 sep 作为分隔字符串。 如果给出了 maxsplit，则最多进行 maxsplit 次拆分（因此，列表最多会有 maxsplit+1 个元素）。 如果 maxsplit 未指定或为 -1，则不限制拆分次数（进行所有可能的拆分）。

如果给出了 sep，则连续的分隔符不会被组合在一起而是被视为分隔空字符串 (例如 '1,,2'.split(',') 将返回 ['1', '', '2'])。 sep 参数可能由多个字符组成 (例如 '1<>2<>3'.split('<>') 将返回 ['1', '2', '3'])。 使用指定的分隔符拆分空字符串将返回 ['']。

例如：

>>>
'1,2,3'.split(',')
['1', '2', '3']
'1,2,3'.split(',', maxsplit=1)
['1', '2,3']
'1,2,,3,'.split(',')
['1', '2', '', '3', '']
如果 sep 未指定或为 None，则会应用另一种拆分算法：连续的空格会被视为单个分隔符，其结果将不包含开头或末尾的空字符串，如果字符串包含前缀或后缀空格的话。 因此，使用 None 拆分空字符串或仅包含空格的字符串将返回 []。

例如：

>>>
'1 2 3'.split()
['1', '2', '3']
'1 2 3'.split(maxsplit=1)
['1', '2 3']
'   1   2   3   '.split()
['1', '2', '3']
# str.splitlines(keepends=False)
返回由原字符串中各行组成的列表，在行边界的位置拆分。 结果列表中不包含行边界，除非给出了 keepends 且为真值。

此方法会以下列行边界进行拆分。 特别地，行边界是 universal newlines 的一个超集。

表示符

描述

\n

换行

\r

回车

\r\n

回车 + 换行

\v 或 \x0b

行制表符

\f 或 \x0c

换表单

\x1c

文件分隔符

\x1d

组分隔符

\x1e

记录分隔符

\x85

下一行 (C1 控制码)

\u2028

行分隔符

\u2029

段分隔符

在 3.2 版更改: \v 和 \f 被添加到行边界列表

例如：

>>>
'ab c\n\nde fg\rkl\r\n'.splitlines()
['ab c', '', 'de fg', 'kl']
'ab c\n\nde fg\rkl\r\n'.splitlines(keepends=True)
['ab c\n', '\n', 'de fg\r', 'kl\r\n']
不同于 split()，当给出了分隔字符串 sep 时，对于空字符串此方法将返回一个空列表，而末尾的换行不会令结果中增加额外的行:

>>>
"".splitlines()
[]
"One line\n".splitlines()
['One line']
作为比较，split('\n') 的结果为:

>>>
''.split('\n')
['']
'Two lines\n'.split('\n')
['Two lines', '']
# str.startswith(prefix[, start[, end]])
如果字符串以指定的 prefix 开始则返回 True，否则返回 False。 prefix 也可以为由多个供查找的前缀构成的元组。 如果有可选项 start，将从所指定位置开始检查。 如果有可选项 end，将在所指定位置停止比较。

# str.strip([chars])
返回原字符串的副本，移除其中的前导和末尾字符。 chars 参数为指定要移除字符的字符串。 如果省略或为 None，则 chars 参数默认移除空白符。 实际上 chars 参数并非指定单个前缀或后缀；而是会移除参数值的所有组合:

>>>
'   spacious   '.strip()
'spacious'
'www.example.com'.strip('cmowz.')
'example'
最外侧的前导和末尾 chars 参数值将从字符串中移除。 开头端的字符的移除将在遇到一个未包含于 chars 所指定字符集的字符时停止。 类似的操作也将在结尾端发生。 例如:

>>>
comment_string = '#....... Section 3.2.1 Issue #32 .......'
comment_string.strip('.#! ')
'Section 3.2.1 Issue #32'
# str.swapcase()
返回原字符串的副本，其中大写字符转换为小写，反之亦然。 请注意 s.swapcase().swapcase() == s 并不一定为真值。

# str.title()
返回原字符串的标题版本，其中每个单词第一个字母为大写，其余字母为小写。

例如：

>>>
'Hello world'.title()
'Hello World'
该算法使用一种简单的与语言无关的定义，将连续的字母组合视为单词。 该定义在多数情况下都很有效，但它也意味着代表缩写形式与所有格的撇号也会成为单词边界，这可能导致不希望的结果:

>>>
"they're bill's friends from the UK".title()
"They'Re Bill'S Friends From The Uk"
string.capwords() 函数没有此问题，因为它只用空格来拆分单词。

作为替代，可以使用正则表达式来构造针对撇号的变通处理:

>>>
import re
def titlecase(s):
    return re.sub(r"[A-Za-z]+('[A-Za-z]+)?",
                  lambda mo: mo.group(0).capitalize(),
                  s)

titlecase("they're bill's friends.")
"They're Bill's Friends."
# str.translate(table)
返回原字符串的副本，其中每个字符按给定的转换表进行映射。 转换表必须是一个使用 __getitem__() 来实现索引操作的对象，通常为 mapping 或 sequence。 当以 Unicode 码位序号（整数）为索引时，转换表对象可以做以下任何一种操作：返回 Unicode 序号或字符串，将字符映射为一个或多个字符；返回 None，将字符从结果字符串中删除；或引发 LookupError 异常，将字符映射为其自身。

你可以使用 str.maketrans() 基于不同格式的字符到字符映射来创建一个转换映射表。

另请参阅 codecs 模块以了解定制字符映射的更灵活方式。

# str.upper()
返回原字符串的副本，其中所有区分大小写的字符 4 均转换为大写。 请注意如果 s 包含不区分大小写的字符或者如果结果字符的 Unicode 类别不是 "Lu" (Letter, uppercase) 而是 "Lt" (Letter, titlecase) 则 s.upper().isupper() 有可能为 False。

所用转换大写算法的描述请参见 Unicode 标准的 3.13 节。

# str.zfill(width)
返回原字符串的副本，在左边填充 ASCII '0' 数码使其长度变为 width。 正负值前缀 ('+'/'-') 的处理方式是在正负符号 之后 填充而非在之前。 如果 width 小于等于 len(s) 则返回原字符串的副本。

例如：

>>>
"42".zfill(5)
'00042'
"-42".zfill(5)
'-0042'


# 参考
 - [Python 字符串](https://www.runoob.com/python/python-strings.html)
 - [文本序列类型 --- str](https://docs.python.org/zh-cn/3/library/stdtypes.html#text-sequence-type-str)
