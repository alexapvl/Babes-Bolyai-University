"""
Euler's Totient Function Implementation

This module provides functions to:
1. Compute Euler's totient function φ(n) for any natural number
2. Find all numbers less than a bound that have a specific Euler function value
"""

import math
from typing import List


def gcd(a: int, b: int) -> int:
    """
    Compute the Greatest Common Divisor using Euclidean algorithm.
    
    Args:
        a: First integer
        b: Second integer
    
    Returns:
        The GCD of a and b
    """
    while b:
        a, b = b, a % b
    return a


def euler_function_naive(n: int) -> int:
    """
    Compute Euler's totient function φ(n) using the naive counting method.
    
    This counts how many numbers from 1 to n are coprime to n.
    Time complexity: O(n * log(n))
    
    Args:
        n: A natural number
    
    Returns:
        The value of φ(n)
    """
    if n == 1:
        return 1
    
    count = 0
    for i in range(1, n + 1):
        if gcd(i, n) == 1:
            count += 1
    return count


def prime_factorization(n: int) -> dict:
    """
    Find the prime factorization of n.
    
    Args:
        n: A natural number
    
    Returns:
        Dictionary mapping prime factors to their exponents
    """
    factors = {}
    
    # Check for factor 2
    while n % 2 == 0:
        factors[2] = factors.get(2, 0) + 1
        n //= 2
    
    # Check for odd factors from 3 onwards
    i = 3
    while i * i <= n:
        while n % i == 0:
            factors[i] = factors.get(i, 0) + 1
            n //= i
        i += 2
    
    # If n is still greater than 1, it's a prime factor
    if n > 1:
        factors[n] = factors.get(n, 0) + 1
    
    return factors


def euler_function(n: int) -> int:
    """
    Compute Euler's totient function φ(n) using the efficient formula.
    
    Uses the formula: φ(n) = n * ∏(1 - 1/p) for all prime factors p of n
    Time complexity: O(√n)
    
    Args:
        n: A natural number
    
    Returns:
        The value of φ(n)
    """
    if n == 1:
        return 1
    
    # Get prime factorization
    prime_factors = prime_factorization(n)
    
    # Apply the formula: φ(n) = n * ∏((p-1)/p) for each prime p
    result = n
    for prime in prime_factors:
        result = result * (prime - 1) // prime
    
    return result


def find_numbers_with_euler_value(v: int, b: int) -> List[int]:
    """
    Find all natural numbers less than b that have v as their Euler function value.
    
    For a given value v and bound b, this function lists all n < b such that φ(n) = v.
    
    Args:
        v: The target Euler function value
        b: The upper bound (exclusive)
    
    Returns:
        A list of all natural numbers n < b such that φ(n) = v
    """
    result = []
    
    for n in range(1, b):
        if euler_function(n) == v:
            result.append(n)
    
    return result


def display_euler_values(start: int, end: int) -> None:
    """
    Display Euler function values for a range of numbers.
    
    Args:
        start: Starting value (inclusive)
        end: Ending value (inclusive)
    """
    print(f"\nEuler's function values from {start} to {end}:")
    print("-" * 40)
    for n in range(start, end + 1):
        phi_n = euler_function(n)
        print(f"φ({n}) = {phi_n}")


def main():
    """
    Main function demonstrating the usage of Euler's function algorithms.
    """
    print("=" * 50)
    print("Euler's Totient Function")
    print("=" * 50)
    
    # Example 1: Compute Euler function for specific numbers
    print("\n1. Computing Euler's function for specific numbers:")
    test_numbers = [1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 36]
    for n in test_numbers:
        print(f"φ({n}) = {euler_function(n)}")
    
    # Example 2: Display range of values
    display_euler_values(1, 20)
    
    # Example 3: Find numbers with specific Euler value
    print("\n" + "=" * 50)
    print("2. Finding numbers with specific Euler values:")
    print("=" * 50)
    
    v = int(input("\nEnter the target Euler function value (v): "))
    b = int(input("Enter the upper bound (b): "))
    
    numbers = find_numbers_with_euler_value(v, b)
    
    print(f"\nNumbers less than {b} with φ(n) = {v}:")
    if numbers:
        print(f"Found {len(numbers)} number(s): {numbers}")
        
        # Verify the results
        print("\nVerification:")
        for n in numbers:
            print(f"  φ({n}) = {euler_function(n)}")
    else:
        print(f"No numbers found with φ(n) = {v} less than {b}")

if __name__ == "__main__":
    main()

