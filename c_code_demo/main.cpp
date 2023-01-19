// #include <stdio.h>
#include <stack>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <unordered_map>
#include <string>
// void print_binary(uint16_t dec){
//     std::stack<uint16_t> myStack;
//     while (dec > 0) {
//         uint16_t digit = dec %2;
//         myStack.push(digit);
//         dec /= 2;
//     }
//     while (!myStack.empty())
//     {
//         printf("%d", myStack.top());
//         myStack.pop();
//     }
//     return;
// }

// int main(){
//     uint16_t dec = 8;
//     print_binary(dec);
// }
// int main() {
//     int c[10] = {1,2,4,5,10, 11, 100, 200, 330, 4000};
//     int target = 200;
//     int low_idx = 0;
//     int high_idx = 9;
//     while (low_idx <= high_idx)
//     {
//         int mid = (low_idx + high_idx) / 2;
//         if (c[mid] == target){
//             printf("found it yey!\n");
//             return 0;
//         }
//         else if (c[mid] < target){
//             low_idx = mid+1;
//         }
//         else {
//             high_idx = mid-1;
//         }
//     }
//     printf("nothing found\n");
//     return 0;
// }

// int multi_loop(int a, int b)
// {
//     int result = 0;
//     for (int i = 0; i < b; i++)
//     {
//         result += a;
//     }
//     return result;
// }


// // assuming b > 0
// int multi_recursion(int a, int b)
// {
//     if (b == 0)
//     {
//         return 0;
//     }
//     else if (b == 1) {
//         return a;
//     }
//     else {
//         return a + multi_recursion(a, b - 1);
//     }
// }

// int factorial_loop(int n){
//     int result = 1;
//     while (n > 1)
//     {
//         result = result * n;
//         n--;
//     }
//     return result;
// }

// int factorial_rec(uint32_t n){
//     // range: 0 - (2^32 - 1)
//     //n! = n * (n-1)!
//     if (n == 1) {
//         return 1;
//     } else {
//         return n * factorial_rec(n - 1);
//     }
// }

// int main() {
//     std::cout << factorial_loop(5) << std::endl;
//     std::cout << factorial_rec(5) << std::endl;
//     // std::cout << multi_recursion(5, 4) << std::endl;
//     return 0;
// }

// decomposition

// abstraction
// interface


// multi_recursion(5,4): = 20
//     5 + multi_recursion(5, 3):
//     5 + 5 + multi_recursion(5, 2):
//     5 + 5 + 5 + multi_recursion(5, 1):
//     5 + 5 + 5 + 5 = 20


// fibonacci(n) = fibonacci(n - 1) + fibonacci(n - 2), given n > 2
// fibonacci(1) = 0
// fibonacci(2) = 1

// int fibonacci(int n)
// {
//     if (n == 1) {
//         return 0;
//     } else if (n == 2) {
//         return 1;
//     } else {
//         return fibonacci(n - 1) + fibonacci(n - 2);
//     }
//     return 0;
// }

// int fib_faster(int n, std::unordered_map<int, int> &mem) {
//     if (n == 1) {
//         return 0;
//     } else if (n == 2) {
//         return 1;
//     } else {
//         if (mem.find(n) != mem.end()) {
//             return mem[n];
//         }
//         int result = fibonacci(n - 1) + fibonacci(n - 2);
//         mem[n] = result;
//         return result;
//     }
//     return 0;
// }

std::vector<int> combine(std::vector<int> left, std::vector<int> right){
    int i = 0, j = 0;
    std::vector<int> result;
    while (i < left.size() && j < right.size()){
        if (left[i] < right[j]){
            result.push_back(left[i]);
            i++;
        } else {
            result.push_back(right[j]);
            j++;
        }
    }
    for (;i < left.size();i++){
        result.push_back(left[i]);
    }
    for (;j< right.size();j++){
        result.push_back(right[j]);
    }
    return result;
}

std::vector<int> merge_sort(std::vector<int> vec){
    if (vec.size() == 0 || vec.size() == 1){
        return vec;
    }
    int mid = vec.size() / 2;
    // left half
    std::vector<int> left_sub_vec (vec.cbegin(), vec.cbegin() + mid);
    std::vector<int> sorted_left = merge_sort(left_sub_vec);
    //right half
    std::vector<int> right_sub_vec (vec.cbegin() + mid, vec.cend());
    std::vector<int> sorted_right = merge_sort(right_sub_vec);

    return combine(sorted_left, sorted_right);
}

bool isPalimdrone(std::string str)
{
    if (str.size() == 0 || str.size() == 1)
    {
        return true;
    }
    if (str[0] == str[str.size() - 1] && isPalimdrone(str.substr(1, str.size()-2))) {
        return true;
    } else{
        return false;
    }
}

int main() {
    std::string str = "addbdcdba";
    std::cout << str.substr(1, str.size()-2) << std::endl;
    if (isPalimdrone(str)){
        std::cout << "is palimdrome" << std::endl;
    } else{
        std::cout << "not palimdrome" << std::endl;
    }
    return 0;
}

// set = [1,3,5,6,6]
// (2,4) = [6] open
// [2,4] = [5,6,6] closed
// half open (2,4]
// [2,4)

// int main(){
//     std::unordered_map<int, int> mem;
//     mem[5] = 4;
//     mem[1] = 6;
//     std::cout << fib_faster(7, mem) << std::endl;
//     return 0;
// }
// fibonacci(4)
// memoization
// dynamic programming


// int fibonacci_mem

// int main() {
//     std::vector<int> vec = {1, 55, 6, 8, 10, 11, 13, 100, 78, 66};
//     vec = merge_sort(vec);
//     for (auto item : vec){
//         std::cout << item << " ";
//     }
//     std::cout << std::endl;
//     return 0;
// }



