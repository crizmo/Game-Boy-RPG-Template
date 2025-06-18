# Game Boy RPG Template

A simple, top-down RPG template for Game Boy, built with GBDK.

## Features

- Top-down movement system with four-directional player animations
- Camera system that follows the player across a large world map
- Tile-based collision system
- Simple title screen
- Optimized for Game Boy hardware
- Compatible with Game Boy emulators and real hardware

## Project Structure

```
.
├── gfx
│   ├── player.c     # Player sprite data (8 sprites for 4 directions)
│   └── tiles.c      # Background tile data (8 basic terrain tiles)
├── maps
│   └── worldmap.c   # 32x32 tile world map data
├── main.c           # Main game logic
├── Makefile         # Build configuration
├── README.md        # This documentation
└── gbrpg.gb         # Compiled Game Boy ROM
```

## Dependencies

- [GBDK](https://github.com/gbdk-2020/gbdk-2020) - Game Boy Development Kit
- Make

## Setup

1. Install GBDK and ensure it's in your path (or update the Makefile with the correct path)
2. Clone this repository:
   ```
   git clone https://github.com/crizmo/Game-Boy-RPG-Template
   cd Game-Boy-RPG-Template
   ```

## Building

```bash
make
```

This will compile the project and create a `gbrpg.gb` file that can be run on a Game Boy emulator or loaded onto compatible hardware.

## Customization

### Adding New Tiles

Edit `gfx/tiles.c` to add new tiles to your game. Each tile is defined as 16 bytes representing an 8x8 pixel graphic.

### Creating Your Own Maps

Edit `maps/worldmap.c` to design your own game world. The map is a 32x32 grid where each value corresponds to a tile in the tileset.

### Modifying Player Sprites

Edit `gfx/player.c` to change the player's appearance. The file contains 8 sprites (2 animation frames for each of the 4 directions).

## Game Controls

- **D-pad**: Move the player
- **Start**: Begin the game from the title screen

## Advanced Features

### Collision System

The collision system is based on tile values. Tiles with values 0-3 are walkable, while others block movement. Modify the `can_move_to()` function in `main.c` to change this behavior:

```c
unsigned char can_move_to(unsigned char x, unsigned char y) {
    // Edit this to change which tiles are walkable
    return worldmap[y*MAP_WIDTH+x] < 4;
}
```

### Camera System

The camera follows the player across the map, keeping them centered on the screen when possible. The camera only moves when the player gets close to the edge of the current view.

### Animation System

Player animations automatically change based on direction and movement state. The animation speed can be adjusted by modifying the `ANIMATION_DELAY` constant in `main.c`.

## Performance Considerations

- The game is optimized for the Game Boy's limited hardware
- Sprites are 8x8 pixels to conserve sprite memory
- The map is loaded in chunks as the player moves to minimize memory usage