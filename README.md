# TLLibrary
The TemplateListLibrary, which misleadinly isn't just about template lists.

# Foreword
The TLLibrary was a C++ library I developed for personal use over many years, spanning from 2010 to 2016. Because of real life, I no longer have the time to use TLLibrary, which is largely in an incomplete state of development. The TLLibrary relies on SFML for specific operations (audio, graphical, etc), the Mars library (used for cross-compatible directory interfaces), but a lot of it is made from scratch, designed where possible to work cross platform.

The TLLibrary is free to use, modify, utilise as needed, with a very few note exceptions:
In any large project (EG C++ standards library etc), name credit for any ideas or designs *must* be supplied. That is all that needs to be supplied. It can be anywhere, but it must be human accessible to the public.


# Warnings
The TLLibrary does not conform to any *known* standard of programming, and actually violates every held notion of how to write C++. This is exclusively for speed of writing. Do not be surprised to find macros everywhere (part of TLLibrary's internal traceroute), C++ code in .h files (shorter than .hpp), and Class methods declared inside the class itself (bypasses the need to redundantly declare a function's name twice). It was never designed with public release in mind. That said, it can be a *very powerful* library if used correctly.

# TLL Standard

The TLLibrary follows it's own set of standards, specific to the Revision 1 version, these are:

1) It must be capable of being modified to work in C (this standard was later dropped, but you will find C compatible memory allocation macros that can replace the C++ compatible memory allocation macros, files are .h, printf usage etc)

2) All classes, methods and certain functions MUST have an internal traceroute functionality if the function is capable of returning an error or failure state. These messages must also be suppressable, and codewise, even removeable to reduce code bloat. As a result, there is two macros that handle traceroute coding.

3) TLLibrary follows a 'quit on first error' approach. If it's going wrong, continuing to run it will just make it even more wrong.

4) Functions/methods must be classified into two types: non-error returning, and error returning.

5) Error returning functions/methods must provide a clear 'success' and 'failure' state.

6) Functions/methods that call error returning methods/functions must handle the error and return the error  (IE quit on first error) if it can't. In short: Function/methods that call error returning function/methods become error returning function/methods themselves.

7) Classes, in the event of an error, *must* be left in a 'graceful state', IE if a change attempt on an array of chars fails, it must roll back to before the change. Classes that cannot do this, must either be denoted as such (IE 'unsafe', 'not TLL compliant') or must leave the class in a void or clearly invalid state.

8) TLL Classes should independent and self-sustaining (IE not relying on external libraries) where possible, however duplicating functions/code is frowned upon. If functions must be duplicated (IE it has a unique/special behaviour), it must either be incorporated into the class, or given a very unique, non-conflicting name.

9) Despite 8, TLL Classes should be able to work with each other, and should be able to accept similar types of classes (EG CharArray can accept std::string, vectors of chars).

10) Classes must have Open(), Close() and Reset() functions (which is the C equal to constructors/destructors). These are the only public facing 'dangerous' functions (can leak memory if used incorrectly).

11) Classes with constructors must call Open() [unless the constructor does nothing], classes with destructors must call Close() [unless the destructor does nothing]. If there are POD variables that need to be reset, it must be declared in a function called Clear(), which can only be a void function.

12) Open, Close and Reset can only either return void, or bool.

13) Big three declaration rules apply: if a constructor, destructor are declared, it's strongly expected the equals and assignment operators are also declared (even if they are just disabled). TLL encourages that unusual operators, like [] are disabled unless explicitly needed.

14) All Class methods MUST be declared either public (unless it's a Class integrity issue) or protected. Private can only be reserved for functions that must never be directly called. IE all TLL Classes should be geared to be highly inheritable and highly accessible, without compromising Class data integrity.

15) If the Class does any destructive activity (EG writing/modification) it must take a copy unless explicitly told not to. If it does any reading (IE non-destructive), it must take a reference unless explicitly told not to.

16) All variables in a class must be declared protected, and must all be accessible via Get/Set type setups, unless security, data integrity or specific situations dictate otherwise.

17) TLL Classes can only interact vertically with each other (IE via inheritance), they cannot interact with each other horizontally (this might appear to contradict 9, but if the classes declare each other, this can cause a prototyping mess/nightmare. Instead, they interact with each other using standardised containers or common formats, so they don't need to know the other's existence).

18) Classes, functions, methods follow the following priorities in design: data integrity (EG rollback), efficiency, readability. The only time efficiency trumps data integrity is in very bulk tasks called very often (IE iterating over a container), in which case the called class/function/method presumes everything supplied is correct and does minimal validation. See 3) for justification.

19) TLL Classes should have a 'prototyping' format, as well as an error checking version. For example, the line: 'FileProcEquals("Txt.txt","w") = CharArray("Write this to file")' is valid prototyping, but this can be expanded to check that memory allocation for chars succeeded, that accessing the file succeeded etc.

20) Prototyping code must never be used in a fully fledged TLLibrary class, and must be converted into it's full syntax for error checking. Experimental libraries waive this rule as prototyping allows for fast redesigns, but once finalised, must be converted.

21) All classes, where possible, must support the 'IsValid()' and 'IsEmpty()' functions, even if they always return true. Why both? Say you initialised an array full of garbage, IsEmpty would argue that it's 'not empty', but is it valid? No, it's garbage. Although a lot of classes make Valid/Empty polar opposites, some do pretty in-depth checking of Valid (IE making sure all mem allocations are valid/correct), where-as Empty just finds the first piece of evidence a class isn't empty. Empty is faster, but Valid is more safe.

# Why would I use TLL if standard libraries provide the same functionality?

I personally found standardised library syntax to be chunky, counter-intuitive and obtuse. Sometimes even dangerous (C++ iterators can escape memory and require horrible for loops). Consider the example TLL code for a doubly linked list:

- TemplateList<int> Temp;

- Temp.AddNodeEnd(10);
- Temp.AddNodeStart(11);

- if(Temp.IsValid())
- {
- Temp.CurrToStart();
- Do
- {
- printf("%d\n",Temp.GetCurrItem());
- }while(Temp.CurrNext());

- }


You probably already understand what the above code is doing *without explanation*. At no stage are you required to check an error, because you know TLL will quit on first error. Your only requirement is to check that the list is valid to call a variable from. If there are errors, TLLs traceroute mandates it informs you, including what file, what line and why.

What if you wanted to read the list without modifying it? Easy:

- TemplateIterator<int> TempIter(Temp);

The above TempIter basically runs exactly like a TemplateList. And if you wanted a TemplateIterator where you can modify variables in a list (without changing the list)? Even easier:

- TemplateIteratorNonConst<int> TempIter2(Temp);

What's not to like?

# Recommendations

I think the best classes to try to demostrate TLL's unique abilities are CharArray, FileProcEquals, StringTokeniser and TemplateList.

# Bugs

TemplateList class itself suffers from a bug in it's node swapping abilities which hasn't been tracked down. Despite this, it works very stably.

Experimental libraries will be buggy. Very buggy or non-functional libraries will not be uploaded. Use at own risk.
