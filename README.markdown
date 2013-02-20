Nimble Programming Language
========

1. [Introduction](https://github.com/majestic53/nimble-lang#introduction)
2. [Features](https://github.com/majestic53/nimble-lang#features)
	* [Dynamically typed variables](https://github.com/majestic53/nimble-lang#dynamically-typed-variables)
	* [Statically scoped variables](https://github.com/majestic53/nimble-lang#statically-scoped-variables)
	* [Nested function support](https://github.com/majestic53/nimble-lang#nested-function-support)
	* [Multiple inline variable assignments](https://github.com/majestic53/nimble-lang#multiple-inline-variable-assignments)
	* [Native list support](https://github.com/majestic53/nimble-lang#native-list-support)
3. [Grammar](https://github.com/majestic53/nimble-lang#grammar)
	* [Lexer Grammar](https://github.com/majestic53/nimble-lang#lexer-grammar)
	* [Parser Grammar](https://github.com/majestic53/nimble-lang#parser-grammar)
4. [Examples](https://github.com/majestic53/nimble-lang#examples)
5. [Build](https://github.com/majestic53/nimble-lang#build)
6. [License](https://github.com/majestic53/nimble-lang#license)

Introduction
========

Nimble is a new and exciting general-purpose scripting language.

Features
======

Nimble contains a variety of interesting features. With these features included, Nimble can be thought of as a language that spans multiple paradigms.

###Dynamically typed variables

Variables in Nimble can be assigned to any type, at any time during execution, by using the set keyword. This includes the types: integer, float, string, function pointer, and list, interchangeably.

```
set: foo, 10;						# integer assignment
set: foo, 3.14;						# float assignment
set: foo, 'bar';					# string assignment
set: foo(bar) { … };				# function pointer assignment
```

###Statically scoped variables

Variables in Nimble are statically scoped by default. This means that if a variable is not found in the current scope, all scopes up to the global scope will be searched. To access a variable in a higher scope, use the glob keyword.

```
set: foo, glob: bar;				# foo assigned to global variable bar
```

###Nested function support

Nimble supports nested functions. That is, functions contained in other function scopes.

```
set: bar() {
	set: nested_bar() {				# nested function
		set: ret, 'bar';
	} ret;

	set: ret, nested_bar();
} ret;

out: 'foo' + bar();					# print “foobar” to output stream
```

###Multiple inline variable assignments

Multiple variables can be assigned at once using the set keyword. To leverage this feature, functions in Nimble can return multiple values at once. The only requirement is that the return value count must match the assignment count.

```
set: foobar() {
	set: foo, 'foo';
	set: bar, 'bar';
} foo, bar;


set: (foo, bar), foobar();			# function call assignments

set: (foo, bar), ['foo', 'bar'];	# multiple assignments inline
```

###Native list support

Nimble has support for a native list data-type. A variable will automatically become a list if any list operation is performed upon it. With the built-in list operators, lists can perform stack and queue related tasks, as well as be indexed into like a normal array.
	
```
fpush: a, 2 + 3;					# push expression (2 + 3) onto list a
set: b, fpop: a;					# pop front of list a, assign to b
out: size: a;						# print size of list a to output stream
out: b[0];							# print value at index 0 in list b
```

Grammar
========

###Lexer Grammar

The nimble language contains a very simple lexical grammar, with only a handful of primitive types.

* Primitives

```
comment ::= #([^\n].*)
float ::= [0-9]+\.[0-9]*
identifier ::= [a-zA-Z_]+[a-zA-Z0-9_]*
integer ::= [0-9]+
string ::= \'.*\'
symbol ::= ? | , | : | ( | ) | [ | ] | { | } | + | - | * | / | % | ~ | ;
```

* Keywords

```
access operator ::= glob
argument operator ::= arg | args
assignment operator ::= set
binary operator ::= and | or | xor
comparator ::= eq | gt | gte | land | lor | lt | lte | lxor |neq
conditional ::= if | ifelse | range | until | while
constant ::= true | end | false | null
conversion operator ::= int | float
control ::= break | continue | exit
in-list operator ::= fpush | lpush
io operator ::= exec | in | out
iterator ::= posd | posi | pred | prei
logical operator ::= ceil | flr | rnd | shl | shr
out-list operator ::= fpop | lpop | size
using operator ::= use
```

###Parser Grammar

The Nimble language parser grammar begins at the statement tag.

```
compound_assignment ::= [ <expression_list> ]
conditional_statement ::= <comparator> ? <expression> , <expression>
expression_depth0_terminal ::= <binary_operator> <expression_depth1> <expression_depth0_terminal> | ɛ
expression_depth1 ::= <expression_depth2> <expression_depth1_terminal>
expression_depth1_terminal ::= - <expression_depth2> <expression_depth1_terminal> | ɛ
expression_depth2 ::= <expression_depth3> <expression_depth2_terminal>
expression_depth2_terminal ::= + <expression_depth3> <expression_depth2_terminal> | ɛ
expression_depth3 ::= <expression_depth4> <expression_depth3_terminal>
expression_depth3_terminal ::= / <expression_depth4> <expression_depth3_terminal> 
			| % <expression_depth4> <expression_depth3_terminal> | ɛ
expression_depth4 ::= <expression_leaf> <expression_depth4_terminal>
expression_depth4_terminal ::= * <expression_leaf> <expression_depth4_terminal> | ɛ
expression_leaf ::= <identifier>
			| <identifier> <function_parameter_list>
			| <identifier> <list_index>
			| <access_operator> : <identifier>
			| <iterator> : <identifier>
			| <out-list_operator> : <identifier>
			| <logical_operator> : <expression>
			| <conversion_operator> : <expression>
			| ~ <expression_leaf>
			| ( <expression> )
			| <integer>
			| <float>
			| <string>
			| <constant>
			| in
			| args
			| arg : <integer>
expression_list ::= <expression> | <expression> , <expression_list>
expression_parameter_list ::= <expression_list> | ɛ
expression_root ::= <expression_depth1> <expression_depth0_terminal>
function_assignment ::= ( <identifier_parameter_list> ) <statement_list> <expression_parameter_list>
function_parameter_list ::= ( <expression_parameter_list> )
identifier_list ::= <identifier> | ( <identifier_list_helper> )
identifier_list_helper ::= <identifier> | <identifier> , <identifier_list_helper>
identifier_parameter_list ::= <identifier_list_helper> | ɛ
list_index ::= [ <expression> ]
range_statement ::= <conditional_statement> , <expression_list>
simple_assignment ::= <expression>
statement ::= set : <identifier_list> , <compound_assignment> ;
		| set : <identifier_list> , <simple_assignment> ;
		| set : <identifier_list> <function_assignment> ;
		| if : <conditional_statement> < statement_list>
		| ifelse : <conditional_statement> <statement_list> <statement_list_else>
		| range : <range_statement> <statement_list>
		| until : <statement_list> <condition_statement> ;
		| while : <conditional_statement> <statement_list>
		| out : <expression> ;
		| exec : <expression> ;
		| use : <string> ;
		| <iterator> : <identifier_list> ;
		| <in-list_operator> : <identifier_list> , <expression> ;
		| <out-list_operator> : <identifier_list> ;
		| <control> ;
statement_list ::= { <statement_list_helper> }
statement_list_else ::= <statement_list_helper> }
statement_list_helper ::= <statement> <statement_list_helper> | ɛ
```

Examples
======

Below is an example of a Nimble script that determines which of the values 2 – 10 are prime. The results are printed to the output stream.

```
# Nimble sample code -- test.nb
# David Jolly 2013

use: 'math.nb';

# Determine if a number is prime
# @param value number
# @return true if prime, false otherwise
set: is_prime(value) {
	set: (stat, trial), [true, 2];
		
	ifelse: lt? value, 2 { 
		set: stat, false; 
	}
	
		range: lte? trial, sqrt(value), prei: trial { # sqrt() imported from math.nb
		
			if: eq? value % trial, 0 {
				set: stat, false;
				break;
			}
		}
	}
} stat;

# Main
set: value, 2;

range: lte? value, 10, prei: value {

	ifelse: eq? is_prime(value), true { 
		out: value + ' is PRIME!'; 
	}
		out: value + ' is NOT PRIME!'; 
	}
}
```

Build
======

More coming soon...

License
======

Copyright(C) 2013 David Jolly <majestic53@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
