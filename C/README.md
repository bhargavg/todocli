# A todo list manager in C
Maintains the list of todo items in binary file.

## Commands

```bash
$ # initalize todo
$ todo init # --dir=/parent/directory/of/todo.bin
$
$ # list all todo items
$ todo # this is same as `todo list`
$ todo list [--summary] [--completed] [--pending] 
$
$ # get help about command
$ todo add -h
$
$ # Add an item to the list
$ todo add "This is a dummy todo item"
$
$ # Tick the items with id 1 as done
$ todo tick 1
$
$ # Untick the items with id 1
$ todo untick 1
$
$ # Delete the item with id 1
$ todo rm 1 # [--all] [--pending] [--completed]
```
