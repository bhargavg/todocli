# A todo list manager in various languages
Maintains the list of todo items in `~/.todo/todo.txt` file.

## Commands

```bash
$ # initalize todo
$ todo init
$
$ # list all todo items
$ todo # this is same as `todo list`
$ todo list [--pending]
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
$ todo rm 1
```

## TODO
- [ ] Implement CRUD on todo items
- [ ] Keep track of changes in time stamp suffixed files under `~/.todo` directory
