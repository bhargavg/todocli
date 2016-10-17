# A todo list manager in C
Maintains the list of todo items in `~/.todo/todo.txt` file.

## Commands

```bash
$ # initalize todo
$ todo init
$ # list all todo items
$ todo # this is same as `todo list`
$ todo list [--pending]
$ # Mark the items with id 1 as done
$ todo mark 1
$ # Add a new todo item
$ todo add "This is a dummy todo item"
$ # Delete the item with id 1
$ todo rm 1
```


## TODO
- [ ] Implement CRUD on todo items
- [ ] Keep track of changes in time stamp suffixed files under `~/.todo` directory
