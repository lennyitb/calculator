# Lenny's virtual RPL runtime rewrite III

it's been a while since anyone has built a good calculator.

this is supposed to be a portable simple thing that takes most RPL commands for input, maybe I'm changing the syntax a little bit here and there.

a neat feature for instance is that a 'hanging delimiter' is allowed to exist alone on the stack, which can later be closed to form a list, or specify the number of arguments in a command.

it's supposed to be able to run on a microcontroller

It uses the GiNaC library for arbitrary precision, and some special numeric types and symbolic expressions

i came up with what I believe is a pretty elegant container to keep track of the tree structure for each expression on the stack

it supports undo, with the oldest changes being lazily deleted as memory becomes scarse

## THIS is the ROADMAP 😎

	- evaluate commands
	- stack commands
	- mix numerics with expressions
	- make undo work
	- add user macros
	- add lists 
	- (which work in commands)