weak-modules
============

```text
Usage: weak-modules [options] {--add-modules|--remove-modules}
weak-modules [options] {--add-kernel|--remove-kernel} {kernel-release}
--add-modules
        Add a list of modules read from standard input. Create
        symlinks in compatible kernel's weak-updates/ directory.
        The list of modules is read from standard input.

--remove-modules
        Remove compatibility symlinks from weak-updates/ directories
        for a list of modules.  The list of modules is read from
        standard input. Note: it doesn't attempt to locate any
        compatible modules to replace those being removed.

--add-kernel
        Add compatibility symlinks for all compatible modules to the
        specified or running kernel.

--remove-kernel
        Remove all compatibility symlinks for the specified or current
        kernel.

--no-initramfs
        Do not generate an initramfs.

--verbose
        Print the commands executed.

--dry-run
        Do not create/remove any files.
```

# 参考
 * []()
