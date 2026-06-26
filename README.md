# SOLID C++ Chess Engine & GUI

A modular, event-driven 2D Chess Engine written in modern C++ utilizing the **Simple and Fast Multimedia Library (SFML)** for hardware-accelerated graphics rendering. Designed following **SOLID design principles**, this project cleanly separates the chess engine, game logic, and graphical user interface into independent modules for maintainability and scalability.

---

## 🎨 Features

- **Strict SOLID Architecture**
  - Clean separation between the core `GameEngine` logic and the `InteractiveBoard` GUI layer.

- **Event-Driven UI**
  - Replaced blocking terminal input (`std::cin`) with responsive mouse-driven interaction using SFML events.

- **Complete Chess Rules**
  - ♟️ Standard piece movement
  - ⚔️ En Passant
  - 🏰 Castling
  - 👑 Pawn Promotion

- **Game State Validation**
  - Check detection
  - Checkmate detection
  - Illegal move prevention
  - King safety validation

- **Optimized Graphics**
  - 128×128 textures rendered within 120×120 board tiles for clean spacing and alignment.

---

## 📐 Project Structure

```text
project-chess/
│
├── GameEngine/                      # Core chess engine
│   ├── Pieces/                      # Piece implementations
│   │   ├── Bishop.cpp
│   │   ├── King.cpp
│   │   ├── Knight.cpp
│   │   ├── Pawn.cpp
│   │   ├── Piece.cpp                # Abstract base class
│   │   ├── Queen.cpp
│   │   └── Rook.cpp
│   │
│   ├── Board.cpp                    # Board representation
│   ├── BoardInitializer.cpp         # Initial board setup
│   ├── GameController.cpp           # Turn management & move handling
│   ├── GameState.cpp                # Check, checkmate & stalemate detection
│   └── SpecialOperations.cpp        # Castling, En Passant & Promotion
│
├── InteractiveBoard/
│   ├── assets/                      # Piece textures
│   ├── GraphicsRenderer.cpp         # Rendering engine
│   └── main_gui.cpp                 # Application entry point
│
└── README.md
```

---

## 🛠️ Prerequisites

### Windows (MSYS2 / UCRT64)

Install GCC and SFML:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-sfml
```

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install g++ libsfml-dev
```

---

## 🚀 Building and Running

Compile the project:

### Windows (MSYS2)

```bash
g++ main_gui.cpp -o chess_gui ^
-I"C:\msys64\ucrt64\include" ^
-L"C:\msys64\ucrt64\lib" ^
-lsfml-graphics ^
-lsfml-window ^
-lsfml-system
```

Run the executable:

```bash
./chess_gui
```

---

## 🎮 How to Play

1. **Select a Piece**
   - Left-click one of your own pieces.

2. **Choose Destination**
   - Left-click the destination square.

3. **Move Validation**
   - If the move is legal, the piece moves.
   - Illegal moves are rejected automatically.
   - Moves that leave your king in check are prevented.

4. **Special Moves**
   - 🏰 Castling
   - ⚔️ En Passant
   - 👑 Pawn Promotion

---

## ⚙️ Event-Driven Move Flow

Unlike console-based chess programs that pause execution waiting for user input, this project uses an event-driven architecture.

```text
Mouse Click
      │
      ▼
main_gui.cpp
      │
      ▼
Pixel Coordinates
      │
      ▼
Board Coordinates (row, column)
      │
      ▼
GameController::handleMove()
      │
      ▼
Special Move Validation
      │
      ▼
Standard Move Validation
      │
      ▼
Board Update
      │
      ▼
GraphicsRenderer
      │
      ▼
Display Updated Board
```

The validation pipeline first checks for special moves such as **En Passant**, **Castling**, and **Pawn Promotion** before executing standard movement rules. Successful moves update the board state immediately, triggering a fresh render of the game.

---

## 🏗️ Design Philosophy

This project follows the **SOLID** principles of object-oriented design:

- **Single Responsibility Principle (SRP)** – Each class has one well-defined responsibility.
- **Open/Closed Principle (OCP)** – New functionality can be added without modifying existing code.
- **Liskov Substitution Principle (LSP)** – Every chess piece derives from a common abstract base class.
- **Interface Segregation Principle (ISP)** – Classes expose only the interfaces they require.
- **Dependency Inversion Principle (DIP)** – Core game logic remains independent of the rendering layer.

This architecture makes the engine easier to extend with features such as AI opponents, move history, networking, or alternative rendering backends.

---

## 🚧 Future Improvements

- AI opponent using Minimax with Alpha-Beta Pruning
- Move history with Undo/Redo
- PGN import/export
- FEN position support
- Multiplayer over network
- Legal move highlighting
- Game timers
- Sound effects and animations

---

## 📜 License

This project was developed for educational purposes to demonstrate modern C++ programming, object-oriented design, event-driven architecture, and implementation of a complete chess engine with a graphical user interface.
