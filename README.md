# HOW TO USE

```
 <RFAE> ::= <Number>
        |  {+ <RFAE> <RFAE>}
        |  {- <RFAE> <RFAE>}
        |  {* <RFAE> <RFAE>}
        |  {if <RFAE> <RFAE> <RFAE>}
        |  {= <RFAE> <RFAE>}
        |  {or <RFAE> <RFAE>}
        |  <Id>
        |  {with {<Id> <RFAE>} <RFAE>}
        |  {fun <Id> <RFAE>}
        |  {<RFAE> <RFAE>}
```

### 1. unzip it

unzip the zip file 

<br>

### 2. compile  


compile the cpp file with makefile 
> $ make

<br>

### 3. use 

you can use parser and interpretor with this way:


* for parsing


> $ ./my_interpretor -p [expression]


* for interpreting


> $ ./my_interpretor [expression]


<br><br>
---

(notice) you sholud write expression with blank! 

`{+ 1 1}`   (o)

`{+1 1}`    (x)