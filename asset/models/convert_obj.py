import os

def convert_custom_to_rt(input_file, output_file):
	"""
	Convert the custom format to the expected .obj format.
	The input format is: tr x1,y1,z1 x2,y2,z2 x3,y3,z3 r,g,b
	"""
	print(f"Reading from: {input_file}")
	print(f"File exists: {os.path.exists(input_file)}")
	
	with open(input_file, 'r') as f:
		lines = f.readlines()
	
	print(f"Read {len(lines)} lines from input file")
	
	# First pass: collect all vertices to find min/max
	all_vertices = []
	for line in lines:
		line = line.strip()
		if not line or line.startswith('#'):
			continue
			
		parts = line.split()
		if len(parts) >= 4 and parts[0] == 'tr':
			for v in parts[1:4]:
				try:
					x, y, z = map(float, v.split(','))
					all_vertices.append((x, y, z))
				except (ValueError, IndexError):
					continue
	
	if not all_vertices:
		print("Error: No valid vertices found in the input file")
		return
	
	# Find min and max coordinates
	min_x = min(v[0] for v in all_vertices)
	max_x = max(v[0] for v in all_vertices)
	min_y = min(v[1] for v in all_vertices)
	max_y = max(v[1] for v in all_vertices)
	min_z = min(v[2] for v in all_vertices)
	max_z = max(v[2] for v in all_vertices)
	
	# Calculate center and size
	center_x = (min_x + max_x) / 2
	center_y = (min_y + max_y) / 2
	center_z = (min_z + max_z) / 2
	size = max(max_x - min_x, max_y - min_y, max_z - min_z)
	
	# Avoid division by zero
	if size == 0:
		size = 1.0
	
	scale = 2.0 / size  # Normalize to fit in [-1, 1] range
	
	print(f"Model bounds: x[{min_x:.2f}, {max_x:.2f}], y[{min_y:.2f}, {max_y:.2f}], z[{min_z:.2f}, {max_z:.2f}]")
	print(f"Center: ({center_x:.2f}, {center_y:.2f}, {center_z:.2f})")
	print(f"Scale factor: {scale:.6f}")
	
	# Second pass: process the file
	vertices = []
	vertex_map = {}  # To store unique vertices
	
	with open(output_file, 'w') as f:
		# Write header
		f.write("# Converted from custom format to OBJ format\n")
		f.write(f"# Original file: {os.path.basename(input_file)}\n")
		f.write(f"# Center: ({center_x:.2f}, {center_y:.2f}, {center_z:.2f})\n")
		f.write(f"# Scale factor: {scale:.6f}\n\n")
		
		# Process vertices
		f.write("# Vertices\n")
		vertex_count = 0
		for line in lines:
			line = line.strip()
			if not line or line.startswith('#'):
				continue
				
			parts = line.split()
			if len(parts) >= 4 and parts[0] == 'tr':
				for v in parts[1:4]:
					if v not in vertex_map:
						try:
							x, y, z = map(float, v.split(','))
							# Center and normalize the vertex
							x = (x - center_x) * scale
							y = (y - center_y) * scale
							z = (z - center_z) * scale
							f.write(f"v {x:.6f} {y:.6f} {z:.6f}\n")
							vertex_map[v] = vertex_count + 1
							vertex_count += 1
						except (ValueError, IndexError) as e:
							print(f"Error processing vertex '{v}': {e}")
		
		# Process faces
		f.write("\n# Faces\n")
		face_count = 0
		for line in lines:
			line = line.strip()
			if not line or line.startswith('#'):
				continue
				
			parts = line.split()
			if len(parts) >= 4 and parts[0] == 'tr':
				try:
					# Get vertex indices (1-based)
					indices = [str(vertex_map[v]) for v in parts[1:4] if v in vertex_map]
					if len(indices) == 3:  # Only write if we have a complete triangle
						f.write(f"f {' '.join(indices)}\n")
						face_count += 1
				except (ValueError, IndexError) as e:
					print(f"Error processing face in line: {line}")
		
		print(f"Processed {vertex_count} vertices and {face_count} faces")

# Get the directory where this script is located
script_dir = os.path.dirname(os.path.abspath(__file__))

# Set input and output file paths
input_file = os.path.join(script_dir, "cat.obj")
output_file = os.path.join(script_dir, "cat_converted.obj")

# Convert the file
convert_custom_to_rt(input_file, output_file)
print(f"\nConversion complete. Output saved to {output_file}")
print(f"Output file size: {os.path.getsize(output_file) / 1024:.2f} KB")
