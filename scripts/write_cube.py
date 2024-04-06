import numpy as np
import argparse

def normalize_vector(v):
    norm = np.linalg.norm(v)
    if norm == 0:
        return v
    return v / norm

def calculate_face_normal(v1, v2, v3):
    # Compute the normal of the face defined by vertices v1, v2, and v3
    vector1 = np.subtract(v2, v1)
    vector2 = np.subtract(v3, v1)
    normal = np.cross(vector1, vector2)
    return normalize_vector(normal)

def write_cube_info_to_file(center_x, center_y, center_z, side_length, r, g, b, material_type, filename):
    half_side = side_length / 2.0
    vertices = np.array([
        [center_x - half_side, center_y - half_side, center_z + half_side],  # Front top left
        [center_x + half_side, center_y - half_side, center_z + half_side],  # Front top right
        [center_x + half_side, center_y + half_side, center_z + half_side],  # Front bottom right
        [center_x - half_side, center_y + half_side, center_z + half_side],  # Front bottom left
        [center_x - half_side, center_y - half_side, center_z - half_side],  # Back top left
        [center_x + half_side, center_y - half_side, center_z - half_side],  # Back top right
        [center_x + half_side, center_y + half_side, center_z - half_side],  # Back bottom right
        [center_x - half_side, center_y + half_side, center_z - half_side],  # Back bottom left
    ])
    
    # Define cube faces using vertex indices
    faces = [
        [0, 1, 2, 3],  # Front face
        [1, 5, 6, 2],  # Right face
        [5, 4, 7, 6],  # Back face
        [4, 0, 3, 7],  # Left face
        [4, 5, 1, 0],  # Top face
        [3, 2, 6, 7],  # Bottom face
    ]

    # Calculate face normals
    face_normals = [calculate_face_normal(vertices[face[0]], vertices[face[1]], vertices[face[2]]) for face in faces]
    
    # Map vertices to their adjacent face normals
    vertex_face_mapping = {i: [] for i in range(8)}
    for face_index, face in enumerate(faces):
        for vertex_index in face:
            vertex_face_mapping[vertex_index].append(face_normals[face_index])
    
    # Compute average normal for each vertex
    vertex_normals = [normalize_vector(np.mean(vertex_face_mapping[vertex], axis=0)) for vertex in vertex_face_mapping]

    with open(filename, 'a') as file:
        for face in faces:
            # Each face is made up of two triangles
            triangles = [(face[0], face[1], face[2]), (face[0], face[2], face[3])]
            for tri in triangles:
                line_data = []
                for vertex_index in tri:
                    vertex = vertices[vertex_index]
                    normal = vertex_normals[vertex_index]
                    line_data.extend([*vertex, *normal])
                line_data.extend([r, g, b, material_type])
                file.write(' '.join(map(str, line_data)) + '\n')

def main():
    parser = argparse.ArgumentParser(description="Write cube information to a file.")
    parser.add_argument("center_x", type=float, help="X position of the cube center")
    parser.add_argument("center_y", type=float, help="Y position of the cube center")
    parser.add_argument("center_z", type=float, help="Z position of the cube center")
    parser.add_argument("side_length", type=float, help="Length of the cube side")
    parser.add_argument("color_r", type=float, help="Red color component (0-1)")
    parser.add_argument("color_g", type=float, help="Green color component (0-1)")
    parser.add_argument("color_b", type=float, help="Blue color component (0-1)")
    parser.add_argument("material_type", type=int, help="Material type (0 or 1)")
    parser.add_argument("filename", type=str, help="Filename to write the information to")

    args = parser.parse_args()

    write_cube_info_to_file(args.center_x, args.center_y, args.center_z, args.side_length,
                            args.color_r, args.color_g, args.color_b, args.material_type, args.filename)

if __name__ == "__main__":
    main()
