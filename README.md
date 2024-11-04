## Sqrt2
This is a C-program developed by Chris Krinitsin, Leon Baptist Kniffki and Noah Schlenker on behalf of the course GRA (Basics of Computer Architecture) at TU Munich.
The program allows you to compute any number of decimal places of the square root of two and display it in decimal / hexadecimal.

### Implementation
To actually compute the square root, we implemented our own big_num struct, which is able to represent a number at any given precision and according arithmetic functions. 
Additionally, we optimized our program further by the use of SIMD, Karazuba-Multiplication and Fast Exponentation. 
Details on our implementation can be found in `Ausarbeitung`.


