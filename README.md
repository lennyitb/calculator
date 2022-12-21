# Lenny's virtual RPL runtime rewrite III

it's been a while since anyone has built a good calculator.

the code is a little hack but i'm aiming for very high performance

it's mostly based around the idea of algebraic expressions being broken down into a tree, stored in a special container i devised. the key feature here is that the empty nodes nodes are all linked together, making it super fast to construct and delete nodes

when nodes are no longer needed, they're marked as 'deletable', and kept track of on the undo table. if memory fills up, the oldest nodes will then be deleted fully and made availible for reallocation

this is supposed to be a portable simple thing that takes most RPL commands for input, maybe I'm changing the syntax a little bit here and there.

a neat feature for instance is that a 'hanging delimiter' is allowed to exist alone on the stack, which can later be closed to form a list, or specify the number of arguments in a command.

it's meant to ultimately be a very simple API with some sort of user interface built around it; either a webpage, mobile app, or even a real life calculator. in the future the parser may support configuration commands to do things like output levels of the stack with JSON, or LaTeX.

it's supposed to be able to run on a microcontroller. Under the hood, it uses the GiNaC library for arbitrary precision, and some special numeric types and symbolic expressions. It's meant to be a very powerful calculator.

## THIS is the ROADMAP 😎

	[x] evaluate commands
	[ ] stack commands
	[ ] symbols
	[ ] mix numerics with expressions
	[ ] make undo work
	[ ] add user macros
	[ ] add lists 
	[ ] (which work in commands)
	[ ] error interface.
		- outputs errors in a standard way
		- describes the nature of the error alongside the discarded input which caused the error
	[ ] support special parser configuration commands
		- to evaluate automatically
		- recall variables and availible memory and things
		- change input/output syntax/format