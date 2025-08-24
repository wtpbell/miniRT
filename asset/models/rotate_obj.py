#!/usr/bin/env python3
import sys
import os
import math

def rotate_vertex(x, y, z, rot_x, rot_y, rot_z=0.0):
    """
    Rotate a vertex around X, Y, and Z axes.
    
    Args:
        x, y, z: Original coordinates
        rot_x: Rotation around X axis in degrees (tilt up/down)
        rot_y: Rotation around Y axis in degrees (turn left/right)
        rot_z: Rotation around Z axis in degrees (roll)
    
    Returns:
        Tuple of (x_rot, y_rot, z_rot) - rotated coordinates
    """
    # Convert degrees to radians
    rot_x_rad = math.radians(rot_x)
    rot_y_rad = math.radians(rot_y)
    rot_z_rad = math.radians(rot_z)
    
    # Rotation around X axis (tilt up/down)
    y1 = y * math.cos(rot_x_rad) - z * math.sin(rot_x_rad)
    z1 = y * math.sin(rot_x_rad) + z * math.cos(rot_x_rad)
    
    # Rotation around Y axis (turn left/right)
    x2 = x * math.cos(rot_y_rad) + z1 * math.sin(rot_y_rad)
    z2 = -x * math.sin(rot_y_rad) + z1 * math.cos(rot_y_rad)
    
    # Rotation around Z axis (roll)
    x_rot = x2 * math.cos(rot_z_rad) - y1 * math.sin(rot_z_rad)
    y_rot = x2 * math.sin(rot_z_rad) + y1 * math.cos(rot_z_rad)
    
    return x_rot, y_rot, z2

def convert_obj_format(input_file, output_file=None):
    """
    Convert and format an OBJ file, cleaning up vertex coordinates.
    
    Args:
        input_file (str): Path to input OBJ file
        output_file (str, optional): Path to output OBJ file. If None, will append '_formatted' to input filename.
    """
    if output_file is None:
        base, ext = os.path.splitext(input_file)
        output_file = f"{base}_formatted{ext}"
    
    with open(input_file, 'r') as f_in, open(output_file, 'w') as f_out:
        f_out.write("# Converted from custom format to OBJ format\n")
        f_out.write(f"# Original file: {os.path.basename(input_file)}\n")
        f_out.write("# Center: (-103.04, 172.76, 0.00)\n")
        f_out.write("# Scale factor: 0.002625\n\n")
        f_out.write("# Vertices\n")
        
        for line in f_in:
            if line.startswith('v '):
                parts = line.split()
                if len(parts) >= 4:  # v x y z [w]
                    # Format each number to max 6 decimal places
                    v1 = f"{float(parts[1]):.6f}".rstrip('0').rstrip('.')
                    v2 = f"{float(parts[2]):.6f}".rstrip('0').rstrip('.')
                    v3 = f"{float(parts[3]):.6f}".rstrip('0').rstrip('.')
                    f_out.write(f"v {v1} {v2} {v3}\n")
            else:
                f_out.write(line)
    
    print(f"Conversion complete. Output saved to: {output_file}")

def rotate_obj(input_file, output_file=None, rot_x=10.0, rot_y=45.0, rot_z=0.0):
    """
    Rotate all vertices in an OBJ file around X, Y, and Z axes.
    
    Args:
        input_file (str): Path to input OBJ file
        output_file (str, optional): Path to output OBJ file. If None, will append '_rotated' to input filename.
        rot_x (float): Rotation around X axis in degrees (tilt up/down, default: 10)
        rot_y (float): Rotation around Y axis in degrees (turn left/right, default: 45)
        rot_z (float): Rotation around Z axis in degrees (roll, default: 0)
    """
    if output_file is None:
        base, ext = os.path.splitext(input_file)
        output_file = f"{base}_rotated{ext}"
    
    with open(input_file, 'r') as f_in, open(output_file, 'w') as f_out:
        for line in f_in:
            if line.startswith('v '):  # Only process vertex lines
                parts = line.strip().split()
                if len(parts) >= 4:  # v x y z [w]
                    # Original coordinates
                    x = float(parts[1])
                    y = float(parts[2])
                    z = float(parts[3])
                    
                    # Apply rotation
                    x_rot, y_rot, z_rot = rotate_vertex(x, y, z, rot_x, rot_y, rot_z)
                    
                    # Format the coordinates
                    x_str = f"{x_rot:.6f}".rstrip('0').rstrip('.')
                    y_str = f"{y_rot:.6f}".rstrip('0').rstrip('.')
                    z_str = f"{z_rot:.6f}".rstrip('0').rstrip('.')
                    
                    # Write the rotated vertex
                    f_out.write(f"v {x_str} {y_str} {z_str}")
                    # Preserve any additional vertex data (like w, r, g, b, etc.)
                    if len(parts) > 4:
                        f_out.write(" " + " ".join(parts[4:]))
                    f_out.write("\n")
                else:
                    f_out.write(line)
            else:
                f_out.write(line)
    
    print(f"Rotation complete. Output saved to: {output_file}")
    print(f"Applied rotations - X: {rot_x}°, Y: {rot_y}°, Z: {rot_z}°")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python rotate_obj.py <input.obj> [output.obj] [rot_x] [rot_y] [rot_z]")
        print("  rot_x: Rotation around X axis in degrees (default: 10)")
        print("  rot_y: Rotation around Y axis in degrees (default: 45)")
        print("  rot_z: Rotation around Z axis in degrees (default: 0)")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else None
    
    # Default rotation values
    rot_x = 10.0
    rot_y = 45.0
    rot_z = 0.0
    
    # Parse rotation values from command line if provided
    if len(sys.argv) > 3:
        try:
            rot_x = float(sys.argv[3])
            if len(sys.argv) > 4:
                rot_y = float(sys.argv[4])
                if len(sys.argv) > 5:
                    rot_z = float(sys.argv[5])
        except ValueError:
            print("Error: Rotation values must be numbers")
            sys.exit(1)
    
    # Rotate the object
    rotate_obj(input_file, output_file, rot_x, rot_y, rot_z)
