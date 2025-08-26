import os
import sys

def convert_obj_to_rt(input_file, output_file):
    """
    Convert a standard Wavefront OBJ file to a simplified OBJ format with triangulated faces
    """
    print(f"Reading from: {input_file}")
    print(f"File exists: {os.path.exists(input_file)}")
    
    with open(input_file, 'r') as f:
        lines = f.readlines()
    
    print(f"Read {len(lines)} lines from input file")
    
    # Collect vertices and faces
    vertices = []
    faces = []
    
    for line in lines:
        line = line.strip()
        if not line or line.startswith('#'):
            continue
            
        parts = line.split()
        if not parts:
            continue
            
        if parts[0] == 'v' and len(parts) >= 4:  # Vertex
            try:
                x, y, z = map(float, parts[1:4])
                vertices.append((x, y, z))
            except (ValueError, IndexError):
                continue
        elif parts[0] == 'f':  # Face
            try:
                # Get vertex indices (1-based)
                face_vertices = []
                for part in parts[1:]:
                    # Handle format: v/vt/vn or v//vn
                    v_part = part.split('/')[0]
                    if v_part:
                        face_vertices.append(int(v_part))
                
                # Convert to triangles
                if len(face_vertices) >= 3:
                    # Simple triangulation: fan from first vertex
                    for i in range(1, len(face_vertices) - 1):
                        # Ensure we don't have more than 3 vertices per face
                        tri = [face_vertices[0], face_vertices[i], face_vertices[i + 1]]
                        if len(set(tri)) == 3:  # Ensure no duplicate vertices
                            faces.append(tri)
            except (ValueError, IndexError) as e:
                print(f"Warning: Could not process face: {line}")
                print(f"Error: {e}")
                continue
    
    if not vertices or not faces:
        print("Error: No valid vertices or faces found in the input file")
        return False
    
    # Calculate center and scale to normalize the model
    min_coords = [min(v[i] for v in vertices) for i in range(3)]
    max_coords = [max(v[i] for v in vertices) for i in range(3)]
    center = [(min_coords[i] + max_coords[i]) / 2 for i in range(3)]
    size = max(max_coords[i] - min_coords[i] for i in range(3))
    
    # Avoid division by zero
    if size == 0:
        size = 1.0
    
    # Write output as OBJ file
    with open(output_file, 'w') as f:
        # Write vertices (normalized and centered)
        for v in vertices:
            # Normalize coordinates
            v_norm = [(v[i] - center[i]) * (2.0/size) for i in range(3)]
            f.write(f"v {v_norm[0]:.6f} {v_norm[1]:.6f} {v_norm[2]:.6f}\n")
        
        # Write faces (1-based indices, triangles only)
        for face in faces:
            # Ensure we have exactly 3 vertices per face
            if len(face) == 3:
                f.write(f"f {face[0]} {face[1]} {face[2]}\n")
    
    print(f"Conversion complete. Wrote {len(vertices)} vertices and {len(faces)} faces to {output_file}")
    return True

if __name__ == "__main__":
    # Get the directory where this script is located
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Set input and output file paths
    input_file = os.path.join(script_dir, "giraffe.obj")
    output_file = os.path.join(script_dir, "giraffe_converted.obj")
    
    # Convert the file
    if convert_obj_to_rt(input_file, output_file):
        print("Conversion successful!")
    else:
        print("Conversion failed.")
        sys.exit(1)
