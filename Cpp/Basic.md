# Basic

## The Basics

+ The type of every entity(e.g., object, value, name and expression) must be known to the compiler at its point of use. The type of an object determines the set of operations applicable to it.

+ use {}-list to initialization which saves you from losing information.

+ When defining a variable, you don't actually need to state its type explicitly when it can be deduced from the initializer:

  ```cpp
  auto b = true; // a bool
  auto d = 2.3;  // a double
  auto z = sqrt(y); // z has the type of whatever sqrt(y) returns
  ```

  with **auto**, we use the = syntax because there is no type conversion involved that might cause problems.

+ In declarations, [] means "array of" and * means "pointer to" and & means "reference to". All arrays have 0 as their lower bound, e.g. a[0] to a[9].

+ null pointer : **nullptr**

+ In an expression, prefix unary * means "contents of" and prefix unary & means "address of".

+ look at the code 

  ```cpp
  for (auto x : v)      // this can be read as "for every element of v, from the first to the last,
  	cout << x << endl;   //  place a copy in x and print it"
  // if we didn't want to copy the values from v into the variable x, 
  // but rather just have x refer to an element, we can
  for (auto &x : v)
   	cout << x << endl;
  ```

+ an example of leaving out initializer in a for-statement:

  ```cpp
  int count_x(char *p, char x)
    // count the number of occurrences of x in p[]
    // p is assumed to point to a zero-terminated array of char(or to nothing)
   {
     	if (p == nullptr) return 0;
    	int count = 0;
    	// leave out the initializer because we don't need it.
    	for (; *p != 0; ++p)
        	if (*p == x)
            	++count;
    	return count
   }
  // using nullptr eliminates potential confusion between integers(such as 0 or NULL) 
  // and pointers (such as nullptr)
  ```

  ​

