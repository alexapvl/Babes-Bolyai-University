"""
Example usage of Euler's function implementation
"""

from euler_function import euler_function, find_numbers_with_euler_value, prime_factorization


def example_1():
    """Example: Computing Euler's function for various numbers"""
    print("=" * 60)
    print("Example 1: Computing φ(n) for various numbers")
    print("=" * 60)
    
    numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 24, 30, 36, 50, 100]
    
    for n in numbers:
        phi_n = euler_function(n)
        factors = prime_factorization(n)
        print(f"φ({n:3d}) = {phi_n:3d}  (prime factorization: {factors})")


def example_2():
    """Example: Finding all numbers with φ(n) = v for specific v values"""
    print("\n" + "=" * 60)
    print("Example 2: Finding all n < b where φ(n) = v")
    print("=" * 60)
    
    test_cases = [
        (1, 10),    # φ(n) = 1, n < 10
        (2, 20),    # φ(n) = 2, n < 20
        (4, 20),    # φ(n) = 4, n < 20
        (6, 30),    # φ(n) = 6, n < 30
        (10, 50),   # φ(n) = 10, n < 50
        (12, 50),   # φ(n) = 12, n < 50
        (16, 100),  # φ(n) = 16, n < 100
    ]
    
    for v, b in test_cases:
        numbers = find_numbers_with_euler_value(v, b)
        print(f"\nφ(n) = {v:2d}, n < {b:3d}:")
        print(f"  Found {len(numbers)} number(s): {numbers}")


def example_3():
    """Example: Properties of Euler's function"""
    print("\n" + "=" * 60)
    print("Example 3: Properties of Euler's function")
    print("=" * 60)
    
    print("\n1. For prime numbers p: φ(p) = p - 1")
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31]
    for p in primes:
        phi_p = euler_function(p)
        print(f"   φ({p:2d}) = {phi_p:2d} = {p} - 1")
    
    print("\n2. For prime powers p^k: φ(p^k) = p^k - p^(k-1) = p^(k-1) * (p - 1)")
    prime_powers = [(2, 2, 4), (2, 3, 8), (3, 2, 9), (3, 3, 27), (5, 2, 25)]
    for p, k, pk in prime_powers:
        phi_pk = euler_function(pk)
        expected = pk - (pk // p)
        print(f"   φ({p}^{k}) = φ({pk:2d}) = {phi_pk:2d} = {pk} - {pk // p} = {p}^{k-1} * ({p} - 1)")
    
    print("\n3. Euler's function is multiplicative: if gcd(m, n) = 1, then φ(m*n) = φ(m) * φ(n)")
    coprime_pairs = [(3, 4), (5, 7), (8, 9), (15, 16)]
    for m, n in coprime_pairs:
        phi_m = euler_function(m)
        phi_n = euler_function(n)
        phi_mn = euler_function(m * n)
        print(f"   φ({m}) * φ({n}) = {phi_m} * {phi_n} = {phi_m * phi_n}")
        print(f"   φ({m} * {n}) = φ({m * n}) = {phi_mn}")
        print(f"   Equal: {phi_mn == phi_m * phi_n}")
        print()


def interactive_mode():
    """Interactive mode for user queries"""
    print("\n" + "=" * 60)
    print("Interactive Mode")
    print("=" * 60)
    
    while True:
        print("\nChoose an option:")
        print("1. Compute φ(n) for a specific number n")
        print("2. Find all numbers n < b where φ(n) = v")
        print("3. Show φ(n) for a range of numbers")
        print("4. Exit")
        
        choice = input("\nEnter your choice (1-4): ").strip()
        
        if choice == '1':
            try:
                n = int(input("Enter n: "))
                if n < 1:
                    print("Please enter a positive integer.")
                    continue
                phi_n = euler_function(n)
                factors = prime_factorization(n)
                print(f"\nResult: φ({n}) = {phi_n}")
                print(f"Prime factorization of {n}: {factors}")
            except ValueError:
                print("Invalid input. Please enter a valid integer.")
        
        elif choice == '2':
            try:
                v = int(input("Enter the target Euler value (v): "))
                b = int(input("Enter the upper bound (b): "))
                if v < 1 or b < 1:
                    print("Please enter positive integers.")
                    continue
                
                print(f"\nSearching for all n < {b} where φ(n) = {v}...")
                numbers = find_numbers_with_euler_value(v, b)
                
                if numbers:
                    print(f"\nFound {len(numbers)} number(s): {numbers}")
                    print("\nVerification:")
                    for n in numbers:
                        print(f"  φ({n}) = {euler_function(n)}")
                else:
                    print(f"\nNo numbers found with φ(n) = {v} less than {b}")
            except ValueError:
                print("Invalid input. Please enter valid integers.")
        
        elif choice == '3':
            try:
                start = int(input("Enter start value: "))
                end = int(input("Enter end value: "))
                if start < 1 or end < start:
                    print("Invalid range.")
                    continue
                
                print(f"\nEuler's function values from {start} to {end}:")
                print("-" * 40)
                for n in range(start, end + 1):
                    phi_n = euler_function(n)
                    print(f"φ({n:3d}) = {phi_n:3d}")
            except ValueError:
                print("Invalid input. Please enter valid integers.")
        
        elif choice == '4':
            print("\nExiting. Goodbye!")
            break
        
        else:
            print("Invalid choice. Please enter 1, 2, 3, or 4.")


if __name__ == "__main__":
    # Run examples
    example_1()
    example_2()
    example_3()
    
    # Interactive mode
    interactive_mode()

