# Lenny's virtual RPL runtime rewrite III
it's been a while since anyone has built a good calculator.

this is supposed to be a portable simple thing that takes most RPL commands for input, maybe I'm changing the syntax a little bit here and there.

it's supposed to be able to run on a microcontroller

It uses the GiNaC library for arbitrary precision, and some special numeric types and symbolic expressions

i came up with what I believe is a pretty elegant container to keep track of the tree structure for each expression on the stack

it supports undo, with the oldest changes being lazily deleted as memory becomes scarse