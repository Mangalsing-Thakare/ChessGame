# Chess Game in C++

A console-based Chess Game implemented in **C++17**, designed to demonstrate core **Object-Oriented Programming (OOP)** concepts through a real-world system.

The project models the chess board, pieces, movement rules, and game state using a clean object-oriented design.

---

## Features

-  Complete chess board representation
-  Individual classes for each chess piece
-  Legal piece movement
-  Piece capturing
-  Turn management
-  Check detection
-  Checkmate detection
-  Stalemate detection
-  Castling
-  En Passant
-  Pawn Promotion
  - Queen
  - Rook
  - Bishop
  - Knight
-  Prevention of moves that leave the player's own King in check

---

##  OOP Concepts Demonstrated

### 1. Encapsulation

Each chess piece manages its own state, including:

- Position
- Color
- Piece type
- Movement state where required

### 2. Abstraction

A common abstract `Piece` class defines the interface for all chess pieces.

```cpp
virtual bool canMoveTo(
    int newX,
    int newY,
    const Board& board
) const = 0;
```

Each derived piece provides its own movement implementation.

### 3. Inheritance

All chess pieces inherit from the base `Piece` class.

```text
                 Piece
                   │
       ┌───────────┼───────────┐
       │           │           │
     Pawn       Knight      Bishop
       │
     Rook       Queen        King
```

### 4. Runtime Polymorphism

The board stores pieces through the base `Piece` type.

```cpp
Piece* piece;
```

The appropriate `canMoveTo()` implementation is selected at runtime depending on the actual piece object.

### 5. Composition

The `Board` class owns and manages chess pieces using:

```cpp
std::unique_ptr<Piece>
```

This provides automatic memory management and clear ownership.

---

##  Project Structure

```text
ChessGame/
│
├── Board.cpp
├── Board.h
├── ChessGame.cpp
├── ChessGame.h
├── main.cpp
├── .gitignore
├── LICENSE
└── README.md
```

### Main Components

**`Piece`**

Base abstract class for all chess pieces.

**`Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`**

Derived classes implementing piece-specific movement rules.

**`Board`**

Responsible for:

- Board representation
- Piece placement
- Movement validation
- Capturing
- Check detection
- Checkmate/stalemate detection
- Special moves

**`ChessGame`**

Responsible for:

- Player turns
- User input
- Chess notation parsing
- Game loop

---

##  How to Run

### Compile

Make sure you have a C++17 compatible compiler installed.

```bash
g++ main.cpp Board.cpp ChessGame.cpp -std=c++17 -o chess
```

### Run

Linux / macOS:

```bash
./chess
```

Windows:

```powershell
.\chess.exe
```

---

##  How to Play

Enter moves using standard chess coordinates:

```text
e2 e4
e7 e5
g1 f3
b8 c6
```

To exit:

```text
quit
```

---

## Supported Chess Rules

| Rule | Status |
|------|--------|
| Pawn Movement | ✅ |
| Knight Movement | ✅ |
| Bishop Movement | ✅ |
| Rook Movement | ✅ |
| Queen Movement | ✅ |
| King Movement | ✅ |
| Capturing | ✅ |
| Check | ✅ |
| Checkmate | ✅ |
| Stalemate | ✅ |
| Castling | ✅ |
| En Passant | ✅ |
| Pawn Promotion | ✅ |
| Self-Check Prevention | ✅ |

---

##  Technologies

- **C++17**
- **Object-Oriented Programming**
- **STL**
- `std::unique_ptr`
- Runtime Polymorphism

---

##  Future Improvements

- Graphical User Interface
- Chess AI opponent
- FEN support
- PGN game notation
- Automated unit testing

---

