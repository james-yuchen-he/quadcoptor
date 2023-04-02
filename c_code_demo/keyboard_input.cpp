#include <iostream>

using namespace std;
// int main()
// {
//     cout << "Please enter your fir st name (followed by 'enter'):" << endl;
//     // first_name is a variable of type string
//     string first_name;
//     cin >> first_name;
//     cout << "Hello, " << first_name << "!" << endl;
//     return 0;
// }


// A variable is a region of memory with a name and a type.
// The type specifies what kind of data can be store in that region of memory
// A value is the specific data that is put into the region of memory.
// definition: a statement that defines a variable, usually with an intial value


// int main()
// {
//     cout << "Please enter your first name (followed by 'enter'):" << endl;
//     string first_name;
//     cin >> first_name;
//     cout << "Hello, " << first_name << "! Now what is your age?" << endl;
//     int age;
//     cin >> age;
//     cout << "Hello, " << first_name << " (age: " << age << ")" << endl;
//     return 0;
// }

// typing "hh" for the age won't work. because the type of "hh" doesn't match
// the type of the variable age (int)

// Now try it yourself. outputing the age in month. Allow half a year (double)
// int main()
// {
//     cout << "Please enter your first name (followed by 'enter'):" << endl;
//     string first_name;
//     cin >> first_name;
//     cout << "Hello, " << first_name << "! Now what is your age in years?" << endl;
//     double age;
//     cin >> age;
//     cout << "Hello, " << first_name << " (age: " << age *  12<< " in months)" << endl;
//     return 0;
// }

// More on types
// addition operation, "+" is the operation. the types of the operand determines
// decides what the operator does.
// int main()
// {
//     int a = 5;
//     int b = 6;
//     cout << "a+b = " << a + b << endl;
//     string first_name = "James";
//     string last_name = "He";
//     cout << "first_name+last_name=" << first_name+last_name << endl;
//     // the following line won't work. the compiler doesn't know how to add a string
//     // to an integer.
//     // cout << "first_name+last_name=" << first_name+a << endl;
//     return 0;
// }

// Operators exercise
// int main()
// {
//     cout << "Please enter a floating point value:" << endl;
//     double n;
//     cin >> n;
//     cout << "n == " << n << endl
//          << "n + 1 == " << n + 1 << endl
//          << "3 x n == " << 3 * n << endl
//          << "n x n == " << n * n << endl
//          << "square root of n == " << sqrt(n) << endl;
// }

// // Assignment and initialization
// int main()
// {
//     int a = 3; // initialization
//     a = 4 // assignment

//     int b = a; // b starts out with a copy of a's value
//     b = a + 5; // b gets the value of a + 5

//     a = a + 7
//     return 0;
// }

// white board the values
// int main()
// {
//     int a = 4;
//     int b = 7;
//     cout << "a=" << a << " b=" << b << endl;

//     int temp;
//     temp = a;
//     a = b;
//     b = temp;
//     cout << "a=" << a << " b=" << b << endl;
// }

// int main()
// {
//     string prev = "";
//     string current;
//     while (cin >> current)
//     {
//         if (prev == current)
//         {
//             cout << "repeated words: " << current << endl;
//         }
//         prev = current;
//     }
//     return 0;
// }

// type casting
// int main()
// {
//     int number_65 = 65;
//     cout << "The value of variable number_48 is: " << number_65 << endl;
//     char letter_A = (char)(number_65);
//     cout << "The value of variable letter_A is: " << letter_A << endl;
//     return 0;
// }