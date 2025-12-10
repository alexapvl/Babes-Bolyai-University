# Lab 5: McEliece Cryptosystem

## Overview

The **McEliece cryptosystem** is a public-key cryptosystem developed by Robert McEliece in 1978. Unlike RSA which relies on the difficulty of factoring large integers, McEliece is based on the **NP-hard problem of decoding a random linear code**. This makes it a promising candidate for post-quantum cryptography, as it is believed to be resistant to attacks by quantum computers.

## Theoretical Background

### Linear Codes

A linear code \( C \) is a subspace of \( \mathbb{F}\_2^n \) (the vector space of binary n-tuples). It is characterized by:

- **n**: The code length (size of codewords)
- **k**: The dimension (number of information bits)
- **d**: The minimum distance (determines error-correction capability)

A code with these parameters is denoted as \([n, k, d]\) and can correct up to \( t = \lfloor(d-1)/2\rfloor \) errors.

### Generator and Parity-Check Matrices

- **Generator matrix G** (k × n): Encodes messages. For message \( m \in \mathbb{F}\_2^k \), the codeword is \( c = m \times G \).
- **Parity-check matrix H** ((n-k) × n): Used for syndrome decoding. For valid codeword \( c \), \( c \times H^T = 0 \).

### Goppa Codes

McEliece uses **Goppa codes**, a class of algebraic codes with:

- Efficient decoding algorithms (Patterson's algorithm)
- Good error-correction capabilities
- Structure that can be hidden through linear transformations

## McEliece Cryptosystem

### Key Generation

1. **Generate a Goppa code** with generator matrix \( G \) (k × n), capable of correcting \( t \) errors
2. **Generate random invertible matrix** \( S \) (k × k) - the scrambling matrix
3. **Generate random permutation matrix** \( P \) (n × n)
4. **Compute public key**: \( G' = S \times G \times P \)

**Public Key**: \( (G', t) \) - the disguised generator matrix and error capability
**Private Key**: \( (S, G, P) \) - the original components

### Encryption

To encrypt a message \( m \in \mathbb{F}\_2^k \):

1. Generate a random error vector \( e \in \mathbb{F}\_2^n \) with Hamming weight exactly \( t \)
2. Compute ciphertext: \( c = m \times G' + e \)

The ciphertext is a vector of \( n \) bits.

### Decryption

To decrypt ciphertext \( c \):

1. **Remove permutation**: Compute \( c' = c \times P^{-1} \)
2. **Decode**: Use the Goppa decoder to decode \( c' \), obtaining \( m \times S \)
3. **Unscramble**: Compute \( m = (m \times S) \times S^{-1} \)

## Security Analysis

### Why It's Secure

1. **Hidden code structure**: The public key \( G' \) looks like a random matrix
2. **Decoding problem**: Without knowing the structure, decoding requires exponential time
3. **Quantum resistance**: No known efficient quantum algorithm for the decoding problem

### Known Attacks

- **Information Set Decoding**: The best known attack, but still exponential
- **Structural attacks**: Attempt to recover the private key from \( G' \)
- **Side-channel attacks**: Implementation-dependent vulnerabilities

## Implementation Details

### Parameters Used (Educational)

| Parameter | Value | Description                      |
| --------- | ----- | -------------------------------- |
| n         | 31    | Code length (codeword bits)      |
| k         | 16    | Message dimension (message bits) |
| t         | 3     | Error correction capability      |

### Alphabet Encoding

- **27 characters**: blank ('\_') + 26 letters (A-Z)
- **5 bits per character**: Since \( 2^5 = 32 \geq 27 \)
- Each character is encoded as its position in the alphabet (0-26)

### Message Processing

1. Text is converted to uppercase and spaces to '\_'
2. Each character is encoded as 5 bits
3. Message bits are grouped into k-bit blocks
4. Each block is encrypted separately using McEliece

## Files

| File          | Description                                     |
| ------------- | ----------------------------------------------- |
| `mceliece.py` | Core cryptosystem implementation                |
| `server.py`   | Chat server that generates and distributes keys |
| `client.py`   | Chat client that encrypts/decrypts messages     |
| `README.md`   | This documentation                              |

## Usage

### Starting the Server

```bash
python server.py
```

The server will:

1. Generate McEliece key pair
2. Listen on port 5556
3. Distribute keys to connecting clients
4. Relay encrypted messages between clients

### Starting a Client

```bash
python client.py [server_ip] [port]
```

Default: `python client.py 127.0.0.1 5556`

### Standalone Encryption/Decryption

```bash
python mceliece.py
```

This allows manual encryption and decryption with custom parameters.

## Example Session

### Key Generation Output

```
[KEY INFO]
  Code length (n): 31
  Message dimension (k): 16
  Error correction (t): 3

  Public Key: 16×31 matrix (G')
  Private Key: Original code + scrambling matrices
```

### Encryption Process

```
Original message: 'HELLO WORLD'
Normalized: 'HELLO_WORLD'
Message as bits: [0, 1, 0, 0, 0, 0, 0, 1, 0, 1, ...] (55 bits)

Formula: c = m × G' + e
  where e is a random error vector of weight 3
```

### Decryption Process

```
Decryption steps:
  1. c' = c × P⁻¹ (remove permutation)
  2. Decode c' using Goppa decoder to get m × S
  3. m = (m × S) × S⁻¹ (unscramble)

Decrypted message: 'HELLO WORLD'
```

## Comparison with RSA

| Aspect            | RSA                   | McEliece                     |
| ----------------- | --------------------- | ---------------------------- |
| Based on          | Integer factorization | Decoding random linear codes |
| Key size          | Smaller               | Larger (matrices)            |
| Encryption speed  | Slower                | Faster                       |
| Decryption speed  | Slower                | Faster                       |
| Quantum resistant | No                    | Yes                          |
| Standardized      | Yes                   | Being standardized (NIST)    |

## Validation

### Plaintext Validation

- Only characters A-Z and space are allowed
- Invalid characters trigger an error

### Ciphertext Validation

- Must be a list of bit vectors
- Each block must have length n
- Each bit must be 0 or 1

## References

1. McEliece, R. J. (1978). "A Public-Key Cryptosystem Based On Algebraic Coding Theory"
2. Bernstein, D. J., et al. (2017). "Classic McEliece" - NIST PQC submission
3. Goppa, V. D. (1970). "A New Class of Linear Error-Correcting Codes"
4. Patterson, N. J. (1975). "The Algebraic Decoding of Goppa Codes"

## Notes

This implementation is for **educational purposes**. For production use:

- Use larger parameters (n ≥ 3488, k ≥ 2720, t ≥ 64)
- Implement constant-time operations to prevent timing attacks
- Use proper random number generation
- Consider the Classic McEliece specification
