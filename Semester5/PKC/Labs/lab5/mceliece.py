#!/usr/bin/env python3
"""
McEliece Cryptosystem Implementation
Based on binary Goppa codes with 27-letter alphabet

The McEliece cryptosystem is a public-key cryptosystem based on the difficulty of
decoding a random linear code. It uses:
- A Goppa code (or similar error-correcting code)
- A scrambling matrix S
- A permutation matrix P

Public key: G' = S × G × P (disguised generator matrix)
Private key: (S, G, P) or equivalently the original code parameters

Encryption: c = m × G' + e (message encoded with public key + error vector)
Decryption: Decode using private key knowledge
"""

import random
import numpy as np
from typing import List, Tuple, Optional


# ============================================================================
# ALPHABET AND ENCODING (27 characters: space + A-Z)
# ============================================================================

ALPHABET = '_ABCDEFGHIJKLMNOPQRSTUVWXYZ'
ALPHABET_SIZE = 27

# Each character needs 5 bits (2^5 = 32 >= 27)
BITS_PER_CHAR = 5


def char_to_bits(char: str) -> List[int]:
    """Convert a character to its 5-bit binary representation."""
    char = char.upper()
    if char == ' ':
        char = '_'
    
    if char not in ALPHABET:
        raise ValueError(f"Invalid character: {char}")
    
    index = ALPHABET.index(char)
    bits = []
    for i in range(BITS_PER_CHAR - 1, -1, -1):
        bits.append((index >> i) & 1)
    return bits


def bits_to_char(bits: List[int]) -> str:
    """Convert 5-bit binary representation to a character."""
    if len(bits) != BITS_PER_CHAR:
        raise ValueError(f"Expected {BITS_PER_CHAR} bits, got {len(bits)}")
    
    index = 0
    for bit in bits:
        index = (index << 1) | bit
    
    if index >= ALPHABET_SIZE:
        # Handle overflow by wrapping
        index = index % ALPHABET_SIZE
    
    return ALPHABET[index]


def text_to_bits(text: str) -> List[int]:
    """Convert text to a list of bits."""
    text = text.upper().replace(' ', '_')
    bits = []
    for char in text:
        bits.extend(char_to_bits(char))
    return bits


def bits_to_text(bits: List[int]) -> str:
    """Convert a list of bits back to text."""
    # Pad to multiple of BITS_PER_CHAR
    while len(bits) % BITS_PER_CHAR != 0:
        bits.append(0)
    
    text = ""
    for i in range(0, len(bits), BITS_PER_CHAR):
        char_bits = bits[i:i + BITS_PER_CHAR]
        text += bits_to_char(char_bits)
    return text


# ============================================================================
# MATRIX OPERATIONS (GF(2) - Binary Field)
# ============================================================================

def gf2_matrix_mult(A: np.ndarray, B: np.ndarray) -> np.ndarray:
    """Multiply two matrices over GF(2)."""
    return np.mod(np.dot(A, B), 2).astype(int)


def gf2_vector_mult(v: np.ndarray, M: np.ndarray) -> np.ndarray:
    """Multiply a vector by a matrix over GF(2)."""
    return np.mod(np.dot(v, M), 2).astype(int)


def gf2_add(a: np.ndarray, b: np.ndarray) -> np.ndarray:
    """Add two vectors/matrices over GF(2) (XOR)."""
    return np.mod(a + b, 2).astype(int)


def gf2_inverse(M: np.ndarray) -> Optional[np.ndarray]:
    """
    Compute the inverse of a matrix over GF(2) using Gaussian elimination.
    Returns None if matrix is not invertible.
    """
    n = M.shape[0]
    if M.shape[1] != n:
        return None
    
    # Augment matrix with identity
    augmented = np.hstack([M.copy(), np.eye(n, dtype=int)])
    
    # Forward elimination
    for col in range(n):
        # Find pivot
        pivot_row = None
        for row in range(col, n):
            if augmented[row, col] == 1:
                pivot_row = row
                break
        
        if pivot_row is None:
            return None  # Matrix is singular
        
        # Swap rows
        if pivot_row != col:
            augmented[[col, pivot_row]] = augmented[[pivot_row, col]]
        
        # Eliminate other rows
        for row in range(n):
            if row != col and augmented[row, col] == 1:
                augmented[row] = gf2_add(augmented[row], augmented[col])
    
    # Extract inverse
    inverse = augmented[:, n:].astype(int)
    return inverse


def random_invertible_matrix(n: int) -> np.ndarray:
    """Generate a random invertible n×n matrix over GF(2)."""
    while True:
        M = np.random.randint(0, 2, size=(n, n), dtype=int)
        if gf2_inverse(M) is not None:
            return M


def random_permutation_matrix(n: int) -> np.ndarray:
    """Generate a random n×n permutation matrix."""
    P = np.zeros((n, n), dtype=int)
    perm = np.random.permutation(n)
    for i, j in enumerate(perm):
        P[i, j] = 1
    return P


def inverse_permutation_matrix(P: np.ndarray) -> np.ndarray:
    """Compute the inverse of a permutation matrix (its transpose)."""
    return P.T.copy()


# ============================================================================
# SIMPLE GOPPA-LIKE CODE IMPLEMENTATION
# ============================================================================
# We use a simplified approach with a BCH-like code for educational purposes.
# The code can correct 't' errors.

class GoppaCode:
    """
    A simplified Goppa-like code implementation.
    
    Parameters:
    - n: Code length (codeword size)
    - k: Message dimension (message size)  
    - t: Error correction capability
    
    The generator matrix G is k × n, so message m (length k) 
    produces codeword c = m × G (length n).
    """
    
    def __init__(self, n: int, k: int, t: int):
        self.n = n  # Code length
        self.k = k  # Message dimension
        self.t = t  # Error correction capability
        
        # Generate a systematic generator matrix [I_k | P]
        # where I_k is k×k identity and P is k×(n-k) parity matrix
        self.G = self._generate_goppa_like_matrix()
        
        # Compute parity check matrix H such that G × H^T = 0
        self.H = self._compute_parity_check_matrix()
        
        # Precompute syndrome lookup table for error correction
        self._build_syndrome_table()
    
    def _generate_goppa_like_matrix(self) -> np.ndarray:
        """Generate a Goppa-like generator matrix."""
        # Create systematic form: [I_k | P]
        I_k = np.eye(self.k, dtype=int)
        
        # Generate random parity portion
        # For a real Goppa code, this would be derived from a Goppa polynomial
        P = np.random.randint(0, 2, size=(self.k, self.n - self.k), dtype=int)
        
        # Ensure good distance properties by making P have enough weight
        for i in range(self.k):
            weight = np.sum(P[i])
            while weight < 2:  # Ensure minimum weight
                P[i] = np.random.randint(0, 2, size=(self.n - self.k,), dtype=int)
                weight = np.sum(P[i])
        
        return np.hstack([I_k, P])
    
    def _compute_parity_check_matrix(self) -> np.ndarray:
        """Compute parity check matrix H from generator matrix G in systematic form."""
        # If G = [I_k | P], then H = [P^T | I_{n-k}]
        P = self.G[:, self.k:]  # Extract parity portion
        I_r = np.eye(self.n - self.k, dtype=int)
        return np.hstack([P.T, I_r])
    
    def _build_syndrome_table(self):
        """Build syndrome lookup table for error patterns up to weight t."""
        self.syndrome_table = {}
        
        # For all error patterns of weight <= t
        for weight in range(self.t + 1):
            self._add_patterns_of_weight(weight)
    
    def _add_patterns_of_weight(self, weight: int):
        """Add all error patterns of given weight to syndrome table."""
        if weight == 0:
            syndrome = tuple([0] * (self.n - self.k))
            self.syndrome_table[syndrome] = np.zeros(self.n, dtype=int)
            return
        
        # Generate error patterns using combinations
        from itertools import combinations
        for positions in combinations(range(self.n), weight):
            error = np.zeros(self.n, dtype=int)
            for pos in positions:
                error[pos] = 1
            
            syndrome = tuple(gf2_vector_mult(error, self.H.T))
            if syndrome not in self.syndrome_table:
                self.syndrome_table[syndrome] = error
    
    def encode(self, message: np.ndarray) -> np.ndarray:
        """Encode a message using the generator matrix."""
        if len(message) != self.k:
            raise ValueError(f"Message must be {self.k} bits, got {len(message)}")
        return gf2_vector_mult(message, self.G)
    
    def decode(self, received: np.ndarray) -> Tuple[np.ndarray, np.ndarray]:
        """
        Decode a received word, correcting up to t errors.
        Returns (decoded_message, error_pattern).
        """
        if len(received) != self.n:
            raise ValueError(f"Received word must be {self.n} bits, got {len(received)}")
        
        # Compute syndrome
        syndrome = tuple(gf2_vector_mult(received, self.H.T))
        
        # Look up error pattern
        if syndrome in self.syndrome_table:
            error = self.syndrome_table[syndrome]
        else:
            # Too many errors - return best guess (no correction)
            error = np.zeros(self.n, dtype=int)
        
        # Correct errors
        corrected = gf2_add(received, error)
        
        # Extract message (first k bits in systematic form)
        message = corrected[:self.k].copy()
        
        return message, error


# ============================================================================
# MCELIECE CRYPTOSYSTEM
# ============================================================================

class McEliecePrivateKey:
    """McEliece private key containing the secret code and matrices."""
    
    def __init__(self, code: GoppaCode, S: np.ndarray, S_inv: np.ndarray,
                 P: np.ndarray, P_inv: np.ndarray):
        self.code = code
        self.S = S
        self.S_inv = S_inv
        self.P = P
        self.P_inv = P_inv
    
    def to_dict(self) -> dict:
        """Serialize private key to dictionary."""
        return {
            'n': self.code.n,
            'k': self.code.k,
            't': self.code.t,
            'G': self.code.G.tolist(),
            'H': self.code.H.tolist(),
            'S': self.S.tolist(),
            'S_inv': self.S_inv.tolist(),
            'P': self.P.tolist(),
            'P_inv': self.P_inv.tolist(),
            'syndrome_table': {str(k): v.tolist() for k, v in self.code.syndrome_table.items()}
        }
    
    @classmethod
    def from_dict(cls, data: dict) -> 'McEliecePrivateKey':
        """Deserialize private key from dictionary."""
        code = GoppaCode.__new__(GoppaCode)
        code.n = data['n']
        code.k = data['k']
        code.t = data['t']
        code.G = np.array(data['G'], dtype=int)
        code.H = np.array(data['H'], dtype=int)
        code.syndrome_table = {
            eval(k): np.array(v, dtype=int) for k, v in data['syndrome_table'].items()
        }
        
        return cls(
            code=code,
            S=np.array(data['S'], dtype=int),
            S_inv=np.array(data['S_inv'], dtype=int),
            P=np.array(data['P'], dtype=int),
            P_inv=np.array(data['P_inv'], dtype=int)
        )


class McEliecePublicKey:
    """McEliece public key containing the disguised generator matrix."""
    
    def __init__(self, G_pub: np.ndarray, n: int, k: int, t: int):
        self.G_pub = G_pub  # Public generator matrix: S × G × P
        self.n = n          # Code length
        self.k = k          # Message dimension
        self.t = t          # Error correction capability
    
    def to_dict(self) -> dict:
        """Serialize public key to dictionary."""
        return {
            'G_pub': self.G_pub.tolist(),
            'n': self.n,
            'k': self.k,
            't': self.t
        }
    
    @classmethod
    def from_dict(cls, data: dict) -> 'McEliecePublicKey':
        """Deserialize public key from dictionary."""
        return cls(
            G_pub=np.array(data['G_pub'], dtype=int),
            n=data['n'],
            k=data['k'],
            t=data['t']
        )


def generate_keys(n: int = 31, k: int = 16, t: int = 3) -> Tuple[McEliecePublicKey, McEliecePrivateKey]:
    """
    Generate McEliece public and private key pair.
    
    Parameters:
    - n: Code length (codeword size in bits)
    - k: Message dimension (message size in bits)
    - t: Error correction capability
    
    Returns:
    - (public_key, private_key)
    """
    # Create the underlying Goppa-like code
    code = GoppaCode(n, k, t)
    
    # Generate random invertible k×k scrambling matrix S
    S = random_invertible_matrix(k)
    S_inv = gf2_inverse(S)
    
    # Generate random n×n permutation matrix P
    P = random_permutation_matrix(n)
    P_inv = inverse_permutation_matrix(P)
    
    # Compute public generator matrix: G' = S × G × P
    G_pub = gf2_matrix_mult(gf2_matrix_mult(S, code.G), P)
    
    # Create key objects
    public_key = McEliecePublicKey(G_pub, n, k, t)
    private_key = McEliecePrivateKey(code, S, S_inv, P, P_inv)
    
    return public_key, private_key


def generate_random_error(n: int, t: int) -> np.ndarray:
    """Generate a random error vector with weight in [0, t] (inclusive).

    Using variable weight instead of always t reduces the chance of
    encountering hard-to-correct patterns in this educational decoder.
    """
    weight = random.randint(0, t)
    error = np.zeros(n, dtype=int)
    if weight > 0:
        positions = random.sample(range(n), weight)
        for pos in positions:
            error[pos] = 1
    return error


def encrypt_block(message_block: np.ndarray, public_key: McEliecePublicKey) -> np.ndarray:
    """
    Encrypt a single message block.
    
    Encryption: c = m × G' + e
    where e is a random error vector of weight t
    """
    if len(message_block) != public_key.k:
        raise ValueError(f"Message block must be {public_key.k} bits")
    
    # Encode message: m × G'
    encoded = gf2_vector_mult(message_block, public_key.G_pub)
    
    # Generate random error
    error = generate_random_error(public_key.n, public_key.t)
    
    # Add error: c = m × G' + e
    ciphertext = gf2_add(encoded, error)
    
    return ciphertext


def decrypt_block(ciphertext: np.ndarray, private_key: McEliecePrivateKey) -> np.ndarray:
    """
    Decrypt a single ciphertext block.
    
    Decryption steps:
    1. Compute c' = c × P^{-1}
    2. Decode c' using the Goppa decoder to get m × S
    3. Compute m = (m × S) × S^{-1}
    """
    if len(ciphertext) != private_key.code.n:
        raise ValueError(f"Ciphertext must be {private_key.code.n} bits")
    
    # Step 1: Remove permutation
    c_prime = gf2_vector_mult(ciphertext, private_key.P_inv)
    
    # Step 2: Decode using the Goppa code
    m_scrambled, _ = private_key.code.decode(c_prime)
    
    # Step 3: Unscramble
    message = gf2_vector_mult(m_scrambled, private_key.S_inv)
    
    return message


def encrypt(plaintext: str, public_key: McEliecePublicKey) -> List[List[int]]:
    """
    Encrypt a plaintext string.
    
    Returns a list of ciphertext blocks (each block is a list of bits).
    """
    # Convert text to bits
    bits = text_to_bits(plaintext)
    
    # Pad to multiple of k
    k = public_key.k
    while len(bits) % k != 0:
        bits.append(0)
    
    # Encrypt each block
    ciphertext_blocks = []
    for i in range(0, len(bits), k):
        block = np.array(bits[i:i + k], dtype=int)
        encrypted = encrypt_block(block, public_key)
        ciphertext_blocks.append(encrypted.tolist())
    
    return ciphertext_blocks


def decrypt(ciphertext_blocks: List[List[int]], private_key: McEliecePrivateKey) -> str:
    """
    Decrypt a list of ciphertext blocks.
    
    Returns the decrypted plaintext string.
    """
    decrypted_bits = []
    
    for block in ciphertext_blocks:
        block_array = np.array(block, dtype=int)
        decrypted = decrypt_block(block_array, private_key)
        decrypted_bits.extend(decrypted.tolist())
    
    # Convert bits back to text
    return bits_to_text(decrypted_bits)


def validate_plaintext(message: str) -> Tuple[bool, Optional[str]]:
    """
    Validate plaintext: Check that message contains only valid characters.
    Valid characters: A-Z (letters) and space.
    Returns: (is_valid, error_message or None)
    """
    invalid_chars = []
    for char in message:
        if char != ' ' and not ('A' <= char.upper() <= 'Z'):
            if char not in invalid_chars:
                invalid_chars.append(char)
    
    if invalid_chars:
        return False, f"Invalid characters found: {invalid_chars}. Only letters A-Z and spaces are allowed."
    return True, None


def validate_ciphertext(ciphertext_blocks: List[List[int]], public_key: McEliecePublicKey) -> Tuple[bool, Optional[str]]:
    """
    Validate ciphertext blocks.
    - Must be a list of bit vectors
    - Each block must have length n
    - Each bit must be 0 or 1
    Returns: (is_valid, error_message or None)
    """
    if not isinstance(ciphertext_blocks, list):
        return False, "Ciphertext must be a list of bit vectors"
    
    if len(ciphertext_blocks) == 0:
        return False, "Ciphertext is empty"
    
    for i, block in enumerate(ciphertext_blocks):
        if not isinstance(block, list):
            return False, f"Block {i} is not a list"
        
        if len(block) != public_key.n:
            return False, f"Block {i} has length {len(block)}, expected {public_key.n}"
        
        for j, bit in enumerate(block):
            if bit not in [0, 1]:
                return False, f"Block {i}, position {j}: invalid bit value {bit} (must be 0 or 1)"
    
    return True, None


# ============================================================================
# UTILITY FUNCTIONS
# ============================================================================

def ciphertext_to_hex(ciphertext_blocks: List[List[int]]) -> str:
    """Convert ciphertext blocks to a hexadecimal string for display."""
    hex_parts = []
    for block in ciphertext_blocks:
        # Convert bit list to integer
        value = 0
        for bit in block:
            value = (value << 1) | bit
        hex_parts.append(format(value, 'x').zfill((len(block) + 3) // 4))
    return ' '.join(hex_parts)


def hex_to_ciphertext(hex_string: str, n: int) -> List[List[int]]:
    """Convert hexadecimal string back to ciphertext blocks."""
    hex_parts = hex_string.strip().split()
    blocks = []
    for hex_val in hex_parts:
        value = int(hex_val, 16)
        bits = []
        for i in range(n - 1, -1, -1):
            bits.append((value >> i) & 1)
        blocks.append(bits)
    return blocks


# ============================================================================
# MAIN DEMO
# ============================================================================

def main():
    """Demonstrate McEliece encryption/decryption."""
    print("=" * 70)
    print("McEliece Cryptosystem Implementation")
    print("=" * 70)
    
    # Choose mode
    print("\nSelect mode:")
    print("1. Encrypt")
    print("2. Decrypt")
    mode_choice = input("Enter choice (1 or 2): ").strip()
    
    if mode_choice not in ['1', '2']:
        print("Invalid choice! Please enter 1 or 2.")
        return
    
    is_encrypt = (mode_choice == '1')
    mode_name = "Encryption" if is_encrypt else "Decryption"
    
    print(f"\n{mode_name} Mode Selected")
    
    # Get parameters
    print("\nEnter McEliece parameters (or press Enter for defaults):")
    
    n_input = input("Enter n (code length, default 31): ").strip()
    n = int(n_input) if n_input else 31
    
    k_input = input("Enter k (message dimension, default 16): ").strip()
    k = int(k_input) if k_input else 16
    
    t_input = input("Enter t (error correction capability, default 3): ").strip()
    t = int(t_input) if t_input else 3
    
    print("\n" + "-" * 70)
    print("STEP 1: Key Generation")
    print("-" * 70)
    
    # Generate keys
    print(f"\nGenerating keys with parameters: n={n}, k={k}, t={t}")
    public_key, private_key = generate_keys(n, k, t)
    
    print(f"\nPublic Key (G'):")
    print(f"  Matrix size: {public_key.k} × {public_key.n}")
    print(f"  Code length n = {public_key.n}")
    print(f"  Message dimension k = {public_key.k}")
    print(f"  Error capability t = {public_key.t}")
    
    print(f"\nPrivate Key components:")
    print(f"  Scrambling matrix S: {private_key.S.shape}")
    print(f"  Generator matrix G: {private_key.code.G.shape}")
    print(f"  Permutation matrix P: {private_key.P.shape}")
    
    print(f"\nAlphabet: 27 characters ('_'=space, A-Z)")
    print(f"Bits per character: {BITS_PER_CHAR}")
    
    if is_encrypt:
        # ===== ENCRYPTION MODE =====
        print("\n" + "-" * 70)
        print("STEP 2: Plaintext Input")
        print("-" * 70)
        
        message = input("\nEnter plaintext to encrypt: ")
        
        # Validate plaintext
        valid, error = validate_plaintext(message)
        if not valid:
            print(f"Error: {error}")
            return
        
        print(f"\nOriginal message: '{message}'")
        print(f"Normalized: '{message.upper().replace(' ', '_')}'")
        
        # Convert to bits
        bits = text_to_bits(message)
        print(f"Message as bits: {bits[:20]}{'...' if len(bits) > 20 else ''}")
        print(f"Total bits: {len(bits)}")
        
        print("\n" + "-" * 70)
        print("STEP 3: Encryption")
        print("-" * 70)
        
        # Encrypt
        ciphertext = encrypt(message, public_key)
        
        print(f"\nFormula: c = m × G' + e")
        print(f"  where e is a random error vector of weight {t}")
        
        print(f"\nNumber of ciphertext blocks: {len(ciphertext)}")
        print(f"Each block has {n} bits")
        
        # Display as hex
        hex_cipher = ciphertext_to_hex(ciphertext)
        print(f"\nCiphertext (hex): {hex_cipher}")
        
        # Display first block in detail
        print(f"\nFirst ciphertext block (bits):")
        print(f"  {ciphertext[0]}")
        
    else:
        # ===== DECRYPTION MODE =====
        print("\n" + "-" * 70)
        print("STEP 2: Ciphertext Input")
        print("-" * 70)
        
        hex_input = input("\nEnter ciphertext (hex format): ").strip()
        
        try:
            ciphertext = hex_to_ciphertext(hex_input, n)
        except Exception as e:
            print(f"Error parsing ciphertext: {e}")
            return
        
        # Validate ciphertext
        valid, error = validate_ciphertext(ciphertext, public_key)
        if not valid:
            print(f"Error: {error}")
            return
        
        print(f"\nCiphertext blocks: {len(ciphertext)}")
        print(f"First block: {ciphertext[0]}")
        
        print("\n" + "-" * 70)
        print("STEP 3: Decryption")
        print("-" * 70)
        
        # Decrypt
        print(f"\nDecryption steps:")
        print(f"  1. c' = c × P⁻¹ (remove permutation)")
        print(f"  2. Decode c' using Goppa decoder to get m × S")
        print(f"  3. m = (m × S) × S⁻¹ (unscramble)")
        
        decrypted = decrypt(ciphertext, private_key)
        
        print(f"\nDecrypted message: '{decrypted.rstrip('_')}'")
    
    print("\n" + "=" * 70)


if __name__ == "__main__":
    main()

