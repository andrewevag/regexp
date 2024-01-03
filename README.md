# regexp

A library for parsing regular expressions and matching with string by converting the regular expressions to DFAs.

# Build
```Bash
# Builds library libregexp.a 
> make

# Runs tests
> make test

```
# Usage
Link with `libregexp.a` and include `regexp.hpp` in `inc/`
```C++
#include "regexp.hpp"
...
 
    Regexp* r = Regexp::parse("[a-zA-Z]+");	
	Dfa d = r->toNfae().toDfa();
	
	std::cout << d.match("hellopeople") << std::endl;    // Will print 1
	std::cout << d.match("hello123people") << std::endl; // Will print 0
	std::cout << d.match("hello\npeople") << std::endl;  // Will print 0

```
