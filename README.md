# MiniRT - Advanced Ray Tracer with Interactive UI

## 📋 Table of Contents
- [Overview](#-overview)
- [Features](#-features)
  - [Mandatory Requirements](#-mandatory-requirements)
  - [Bonus Features](#-bonus-features)
  - [Advanced Features](#-advanced-features)
- [🏗️ Architecture](#-architecture)
- [🛠️ Installation](#-installation)
- [🚀 Usage](#-usage)
- [📝 Scene File Format](#-scene-file-format)
- [📚 Examples](#-examples)
- [🐛 Troubleshooting](#-troubleshooting)
- [👥 Team](#-team)

## 🚀 Overview
MiniRT is a feature-rich ray tracing engine that extends beyond the 42 curriculum requirements. Built with MLX42, it offers an interactive interface for real-time 3D scene manipulation and rendering. The project demonstrates advanced computer graphics concepts while maintaining excellent performance through optimization techniques.

## ✨ Features

### ✅ Mandatory Requirements

#### Core Functionality
- **Geometric Primitives**:
  - Sphere, Plane, and Cylinder with proper intersection handling
  - Support for object resizing (diameter, width, height)
  - Translation and rotation transformations (except for spheres)
  
- **Lighting System**:
  - Ambient lighting
  - Point lights with configurable brightness
  - Hard shadows
  - Diffuse lighting

- **Rendering**:
  - Real-time window management using MLX42
  - Proper cleanup on window close (ESC or window close button)
  - Scene description via `.rt` configuration files

### 🎁 Bonus Features

#### Enhanced Lighting & Materials
- Full Phong reflection model (ambient, diffuse, specular)
- Colored and multi-spot lights
- Texture support (checkerboard, bump maps)

#### Additional Geometry
- Cone primitive (second-degree object)
- Triangle primitive

### 🚀 Advanced Features

#### Performance
- Multi-threaded rendering
- BVH acceleration
- AABB optimization
- Custom memory management

#### Visual Effects
- Depth of Field (DoF)
- Perlin noise generation
- Advanced materials (metal, glass)
- Procedural textures

#### Scene Management
- OBJ file import
- Complex scene handling
- Resource management

## 🏗️ Architecture

### Core Components
- **Ray Tracing Engine**
  - Multi-threaded rendering using work queues
  - Bounding Volume Hierarchy (BVH) for accelerated ray-object intersection
  - Axis-Aligned Bounding Boxes (AABB) for efficient spatial partitioning
  - Custom memory allocator for improved performance

### Material System
- **Material Types**
  - Phong reflection model (ambient, diffuse, specular)
  - Physically-based rendering (PBR) materials
  - Support for metal and glass materials
  - Texture mapping with UV coordinates
  - Normal/Bump mapping for surface detail

### Geometry Pipeline
- **Primitive Support**
  - Sphere, Plane, Cylinder (mandatory)
  - Cone, Triangle (bonus)
  - OBJ mesh import
  - Smooth shading and normal interpolation

### Rendering Pipeline
- **Ray Tracing**
  - Multi-threaded rendering using work queues
  - Bounding Volume Hierarchy (BVH) for accelerated ray-object intersection
  - Axis-Aligned Bounding Boxes (AABB) for efficient spatial partitioning
  - Custom memory allocator for improved performance

### UI System
- **Immediate Mode UI**
  - Real-time parameter adjustment
  - Interactive debugging tools
  - Loading screen with progress indication

## 🛠️ Installation

### Prerequisites
- GCC or Clang
- Make
- MLX42 graphics library
- GLFW3
- libft (included)

### Compilation

# Clone the repository
git clone [your-repository-url]
cd miniRT

### Build Options
- `make` - High quality (64 samples, depth 8)
- `make low` - Fast preview (2 samples, depth 1)
- `make debug` - Debug build with verbose output

## 🚀 Usage

### Running the Application
```bash
# Build the project
make

# Run with a scene file
./miniRT assets/mandatory/spheres.rt
```

### Controls
- `ESC` or close window: Exit the application
- `+/-`: click +/- button to increment and decrement values
- `Render`: click render button to render the scene

## 📝 Scene File Format

### Basic Elements
```
# Camera (position, orientation, FOV)
C 0,0,0 0,0,1 70

# Ambient light (ratio, color)
A 0.2 255,255,255

# Light (position, brightness, color)
L -40,0,30 0.6 255,255,255
```

### Primitives
```
# Sphere (position, diameter, color)
sp 0,0,20 10 255,0,0

# Plane (position, normal, color)
pl 0,0,0 0,1,0 0,128,255

# Cylinder (position, axis, diameter, height, color)
cy 15,0,20 0,1,0 5 10 255,255,0

# Triangle (3 points, color)
tr 0,0,0 1,0,0 0,1,0 255,0,0
```

## 📚 Examples

### Mandatory Scenes
```bash
# Basic scene with spheres
./miniRT assets/mandatory/spheres.rt

# Scene with multiple primitives
./miniRT assets/mandatory/many_objects.rt
```

### Bonus Scenes
```bash
# Complex scene with advanced features
./miniRT assets/bonus/ocean.rt
```

## 🐛 Troubleshooting

### Performance Optimization
If you experience slow rendering, try these optimizations:
- Use the low-quality preset for faster previews:
  ```bash
  make low
  ./miniRT scene.rt
  ```
- Reduce the number of objects in complex scenes
- Lower the maximum ray depth in the scene configuration

## 👥 Team

This project was developed in collaboration with [Jose](https://github.com/jose-42).


## 🙏 Acknowledgments

- [MLX42](https://github.com/codam-coding-college/MLX42) - Graphics library
