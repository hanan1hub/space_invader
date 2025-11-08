# 🚀 Space Invaders Game

> Classic 2D arcade shooter built with C++ and SFML

**End Semester Project** | National University of Sciences and Technology (NUST), Islamabad

[![SFML](https://img.shields.io/badge/SFML-2.5+-green.svg)](https://www.sfml-dev.org/)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)

---

## 🎮 Overview

Space Invaders is an action-packed 2D shooter where you control a spaceship defending against waves of alien invaders. Features progressive difficulty, shield mechanics, persistent high scores, and immersive audio.

## ✨ Features

- **Progressive Difficulty** - Increasing challenge with more enemies and faster gameplay
- **Shield System** - Absorb enemy damage in higher levels
- **High Score Tracking** - Persistent file-based scoring system
- **Audio Effects** - Shooting, collision sounds, and background music
- **Smooth Controls** - Responsive keyboard-based gameplay

---

## 🚀 Quick Start

### Prerequisites
- C++ Compiler (GCC 7.0+ or MSVC 2017+)
- SFML 2.5+
- Visual Studio (recommended)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/hanan1hub/space_invader.git
   cd space-invaders
   ```

2. **Install SFML**
   - **Windows**: Download from [sfml-dev.org](https://www.sfml-dev.org/)
   - **macOS**: `brew install sfml`
   - **Linux**: `sudo apt-get install libsfml-dev`

3. **Configure Resource Paths**
   
   ⚠️ **Important**: Update file paths starting at line 590:
   
   ```cpp
   // Update these 5 paths to match your setup:
   backgroundTexture.loadFromFile("path/to/space.jpg");
   hitBuffer.loadFromFile("path/to/hit.wav");
   gunfireBuffer.loadFromFile("path/to/gunfire.wav");
   backgroundMusic.openFromFile("path/to/background.wav");
   font.loadFromFile("path/to/Roboto-Regular.ttf");
   ```

4. **Build and Run**
   - Open in Visual Studio
   - Build solution (Ctrl+Shift+B)
   - Run (F5)

---

## 🎯 How to Play

### Controls
| Key | Action |
|-----|--------|
| `←` `→` | Move spaceship |
| `Space` | Fire bullet |
| `R` | Restart (game over) |
| `L` | Level up (when available) |
| `Q` | Quit |
| `M` | Main menu |

### Objective
- Destroy alien enemies while avoiding their fire
- Survive as long as possible to increase your score
- Progress through levels for higher difficulty and rewards

---

## 🏗️ Architecture

### Core Classes
- **Spaceship** - Player control and shooting
- **Alien** - Enemy behavior and AI
- **Bullet / EnemyBullet** - Projectile management
- **Shield** - Damage absorption system
- **Button** - UI components

### Key Features
- Vector-based entity management
- FloatRect collision detection
- File I/O for high score persistence
- SFML rendering and audio systems

---

## 📁 Project Structure

```
space-invaders/
├── src/           # Source files (.cpp/.h)
├── resources/     # Audio, fonts, images
├── docs/          # Documentation
└── README.md
```

---

## 👥 Team

End Semester Project developed at NUST, Islamabad
- Member 1: Input, Rendering, Audio
- Member 2: Game Logic, AI, Collisions
- Member 3: Data Management, UI
- Member 4: Testing, Optimization

---

## 🔮 Future Enhancements

- [ ] Multiple levels with unique environments
- [ ] Power-ups (rapid fire, shield regeneration)
- [ ] Multiplayer mode
- [ ] Enhanced graphics and animations
- [ ] Online leaderboard

---

## 📄 License

MIT License - see [LICENSE](LICENSE) for details


⭐ Star if you found this helpful!

</div>
