import time
from rich.console import Console
from rich.table import Table

def gcd_euclidean(a, b):
    """Computes GCD using the Euclidean algorithm."""
    while b:
        a, b = b, a % b
    return a

def gcd_subtractive(a, b):
    """Computes GCD using the subtractive algorithm."""
    while a != b:
        if a > b:
            a = a - b
        else:
            b = b - a
    return a

def gcd_binary(a, b):
    """Computes GCD using the binary GCD (Stein's) algorithm."""
    if a == 0:
        return b
    if b == 0:
        return a

    shift = 0
    while ((a | b) & 1) == 0:
        a >>= 1
        b >>= 1
        shift += 1

    while (a & 1) == 0:
        a >>= 1

    while b != 0:
        while (b & 1) == 0:
            b >>= 1
        if a > b:
            a, b = b, a
        b = b - a
        
    return a << shift

if __name__ == "__main__":
    inputs = [
        (192, 270),
        (12345, 54321),
        (65537, 1),
        (9876543210, 1234567890),
        (111111111111111111, 222222222222222222),
        (9223372036854775807, 2),
        (12345678901234567890, 98765432109876543210),
        (10**30, 3),
        (115792089237316195423570985008687907853269984665640564039457584007913129639935, 618970019642690137449562111),
        (354224848179261915075, 573147844013817084101)
    ]

    console = Console()
    
    table = Table(show_header=True, header_style="bold magenta")
    table.add_column("Number A", style="dim", width=40)
    table.add_column("Number B", width=40)
    table.add_column("Euclidean", justify="right")
    table.add_column("Subtractive", justify="right")
    table.add_column("Binary", justify="right")

    console.print("GCD Algorithms Comparative Analysis (Python)\n", style="bold green")

    for a, b in inputs:
        # Euclidean
        start_time = time.perf_counter()
        res_euclidean = gcd_euclidean(a, b)
        end_time = time.perf_counter()
        time_euclidean = (end_time - start_time) * 1000
        euclidean_result = f"{res_euclidean} ({time_euclidean:.4f} ms)"

        # Subtractive
        if a > 1000000 or b > 1000000:
            subtractive_result = "Too slow to compute"
        else:
            start_time = time.perf_counter()
            res_subtractive = gcd_subtractive(a, b)
            end_time = time.perf_counter()
            time_subtractive = (end_time - start_time) * 1000
            subtractive_result = f"{res_subtractive} ({time_subtractive:.4f} ms)"
        
        # Binary
        start_time = time.perf_counter()
        res_binary = gcd_binary(a, b)
        end_time = time.perf_counter()
        time_binary = (end_time - start_time) * 1000
        binary_result = f"{res_binary} ({time_binary:.4f} ms)"

        table.add_row(str(a), str(b), euclidean_result, subtractive_result, binary_result)

    console.print(table)

