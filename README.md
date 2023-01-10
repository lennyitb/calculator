
# Lenny's virtual RPL runtime rewrite III

## Written by: Lenny

It's been a while since anyone has built a good calculator.

The code is a little hack but I'm aiming for very high performance. It's written in 100% C++11. I started this project in C a number of months ago and got a good way into it before I gave up and decided that I need polymorphism. I didn't know a solitary lick of C++ at the time, but I've been learning it since then and I'm pretty happy with the result. I'm still learning, but I'm pretty happy with the result so far.

It's mostly based on the idea of algebraic expressions being broken down into a tree, which is stored in a special container I devised. I'm a little new to this, but I'm pretty sure it was necessary to roll my own— and I'm honestly proud of the outcome. The key feature here is that the empty Nodes are all linked together, making it super fast to construct and delete Nodes.

When nodes are no longer needed, they're marked as 'deletable' and kept track of on the undo table. if the memory fills up, the oldest nodes will then be deleted fully and made available for reallocation.

this is supposed to be a portable simple thing that takes most RPL commands for input, maybe I'm changing the syntax a little bit here and there.

    1 2 plus 3 plus
      >> plus(plus(1, 2), 3)

    eval
      >> 6

A neat feature for instance is that a 'hanging delimiter' is allowed to exist alone on the stack, which can later be closed to form a list or specify the number of arguments in a command.

    ( 1 2 3 4 5 6 times
      >> times(1, 2, 3, 4, 5, 6)

    eval
      >> 720

It's meant to ultimately be a very simple API with some sort of user interface built around it; either a webpage, mobile app or even a real-life calculator. In the future, the parser may support configuration commands to do things like output levels of the stack with JSON, or LaTeX.

I haven't quite nailed the interface to a T, but it's meant to be capable of being easily ported to stream to and from a wide range of interfaces from web assembly to an I2C connection. It's supposed to be able to run on a microcontroller. My whole inspiration for this project is I suppose a lightweight and portable codebase that can be easily tested on most machines, then be easily ported to the embedded domain. Under the hood, it uses the GiNaC library for arbitrary precision, and some special numeric types and symbolic expressions. It's meant to be a very powerful calculator.

This is still highly experimental and broken.

When nodes are collapsing in an evaluation, this is the hierarchy that the result will assume:

- TYPE_ERROR
- TYPE_EX
- TYPE_NUMERIC
- TYPE_SYMBOL
- TYPE_HOLD
- TYPE_DELIM
- TYPE_CMD
- TYPE_EMPTY
- TYPE_DELETABLE, TYPE_CUTCARD should both cause a crash.

## THIS is the ROADMAP 😎

- [x] evaluate commands
- [ ] stack commands
  - THE stack commands I wish to support descending order:
    - [x] `swap`
    - [x] `drop`
    - [ ] `rolln` / `unrolln` //haha swap, drop & roll lmao
    - [ ] `explode` / `assemble` this is the best idea behind the dream of this project
      - alright so when we get an explode we take the root node of level one, if it's a command, we push all the operands onto the stack, then we assemble a macro with its only instruction being the command and push that one the stack last. It will look something like `<< + >>`. an `eval` will evaluate the macro and revert the stack to its original state.
      - of course i will need macros in order to do this...
      - also, i'm thinking of an `explodeall` command that recursively breaks an entire expression into their own levels of the stack. this command would leave a special delimeter on the stack to mark the end of the expression, so that it can be collected back into a single expression with `assemble`.
    - [ ] `roll` / `unroll`
    - [ ] `rot` / `unrot`
    - [ ] `dup`
    - [ ] `dupn`
    - [ ] `yank`
    - [ ] and guys just um let me know in the comments down below if you think of any more. don't forget to like and subscribe.
- [x] recognize and parse 'stuck' delimiters like `(1 2`
- [ ] symbols
- [ ] directories that can be navigated, and which defined symbols can be stored to
- [ ] patch allll the memory leaks
  - I believe we're making excellent progress in regards to this issue.
- [x] mix numerics with expressions //weird thing for past Lenny to write- it pretty much works out of the box right now and there's pretty much no way to test it without having symbols
- [ ] make undo work
- [ ] rethink all that nativecmd b***shit smh my head It's already pretty much obsolete. I can \*pretty much\* just get rid of it
- [ ] add user macros
- [ ] implement dynamic resizing of NodeContainer
- [ ] add lists (which work in commands)
- [ ] allow partial evaluation for strange types like sequences
- [ ] so I went with the 'vernacular' of referring to things in the code as 'commands' but I'm not sure if that's the best choice. I'm thinking of changing it to 'functions'.
- [ ] there's a fair amount of refactoring to be done. My classes have far too many public declarations.
- [ ] error interface.
  - outputs errors in a standard way
  - describes the nature of the error alongside the discarded input which caused the error
- [ ] support special parser configuration commands
  - to evaluate automatically
  - recall variables and available memory and things
  - change input/output syntax/format
  - global numeric mode
- [ ] add make targets for web assembly and embedded compilation. Also build GiNaC from source.
- [ ] ability to save definitions (like the 50g 'filesystem') and write that to file or export as json to front end
- [ ] don't quit without saving changes??

## Current Known Bugs

- [x] `0 17 times eval` results in `17`
  - I think I might know this one. `17 0 times eval` works like a charm- there's something about my inject function that doesn't like setting `result->data` to `{0}`...
- [ ] deeply nested trees crash in segmentation faults all the time
  - I haven't been able to observe this behavior lately, but I haven't really gone out of my way to elicit it either.
- [x] `7 2 minus eval` results in `-2`
  - i really have no idea how this suddenly popped up after i made symbols a little closer to working. probably top priority to fix.
- [ ] when i eval something, then apply a command to it, it has this weird undo effect. not the hugest bug in the world since it re-evaluates but it's one of those tricky pointer arithmetic things i haven't worked all the way out yet.
