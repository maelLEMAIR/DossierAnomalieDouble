import bpy
import json
import mathutils
import os

TEXTURE_ROOT = "C:/Users/mlemaire/Documents/GitHub/lyo-2-amiga-s2p4-04/res/Textures"
#C:/Users/m4w3l/Documents/GitHub/lyo-2-amiga-s2p4-04/res/Textures
#C:/Users/mlemaire/Documents/GitHub/lyo-2-amiga-s2p4-04/res/Textures

def vec(v):
    return [float(v[0]), float(v[1]), float(v[2])]

def export_mesh(obj, apply_world_matrix=False):
    if obj.type != 'MESH':
        return None

    mesh = obj.to_mesh()
    mesh.calc_loop_triangles()

    vertices_flat = []
    indices_flat = []
    uvs_flat = []

    # Vertices (par vertex)
    for v in mesh.vertices:
        pos = v.co
        vertices_flat.extend([float(pos.x), float(pos.y), float(pos.z)])

    # UVs
    if mesh.uv_layers.active:
        uv_layer = mesh.uv_layers.active.data
        has_uv = True
    else:
        has_uv = False

    # Triangles
    for tri in mesh.loop_triangles:

        # Ordre custom
        vert_order = [1, 0, 2]

        # Indices
        indices_flat.extend([
            tri.vertices[vert_order[0]],
            tri.vertices[vert_order[1]],
            tri.vertices[vert_order[2]],
        ])

        loop_order = [tri.loops[i] for i in vert_order]

        # UVs
        if has_uv:
            for loop_index in loop_order:
                uv = uv_layer[loop_index].uv
                uvs_flat.extend([float(uv.x), float(uv.y)])


    if not has_uv:
        uvs_flat = []

    obj.to_mesh_clear()

    return {
        "vertices": vertices_flat,
        "indices": indices_flat,
        "uvs": uvs_flat
    }

def get_material_color(obj):
    mat = obj.active_material
    if not mat or not mat.use_nodes:
        return None

    principled = next((n for n in mat.node_tree.nodes if n.type == 'BSDF_PRINCIPLED'), None)
    if not principled:
        return None

    def resolve_texture_node(node):
        if node.type == 'TEX_IMAGE' and node.image and node.image.filepath:
            full_path = bpy.path.abspath(node.image.filepath)
            try:
                rel_path = os.path.relpath(full_path, TEXTURE_ROOT)
            except ValueError:
                rel_path = os.path.basename(full_path)
            return rel_path.replace("\\", "/")
        return None

    def find_texture_in_node(node):
        tex_path = resolve_texture_node(node)
        if tex_path:
            return tex_path

        if node.type == 'NORMAL_MAP':
            color_input = node.inputs.get("Color")
            if color_input and color_input.is_linked:
                return find_texture_in_node(color_input.links[0].from_node)

        if node.type == 'SEPCOLOR':
            for inp in node.inputs:
                if inp.is_linked:
                    result = find_texture_in_node(inp.links[0].from_node)
                    if result:
                        return result

        return None

    def get_input_value(input_name):
        inp = principled.inputs[input_name]

        if inp.is_linked:
            from_node = inp.links[0].from_node
            tex_path = find_texture_in_node(from_node)

            if tex_path:
                return {"type": "texture", "file": tex_path}

            return {"type": "unsupported_node", "node_type": from_node.type}

        # Pas de lien → valeur directe
        val = inp.default_value
        if hasattr(val, '__len__'):
            return {
                "type": "color",
                "r": round(val[0], 4),
                "g": round(val[1], 4),
                "b": round(val[2], 4)
            }
        else:
            return {"type": "value", "value": round(val, 4)}

    base_color = get_input_value("Base Color")
    metallic   = get_input_value("Metallic")
    roughness  = get_input_value("Roughness")
    normal     = get_input_value("Normal")

    has_texture = any(
        isinstance(v, dict) and v.get("type") == "texture"
        for v in [base_color, metallic, roughness, normal]
    )

    result = {
        "material_name": mat.name,
        "type": "texture" if has_texture else "color",
        "base_color": base_color,
        "metallic":   metallic,
        "roughness":  roughness,
        "normal":     normal,
    }

    return result

def export_scene(filepath):
    scene_data = {"objects": []}
    selected_objects = bpy.context.selected_objects

    for obj in selected_objects:
        item = {}
        item["name"] = obj.name
        item["position"] = [float(x) for x in obj.matrix_world.to_translation()]
        rot = obj.matrix_world.to_quaternion()
        item["rotation"] = [float(rot.x), float(rot.y), float(rot.z), float(rot.w)]
        item["scale"] = [float(x) for x in obj.matrix_world.to_scale()]
        item["mesh"] = export_mesh(obj)
        scene_data["objects"].append(item)

    with open(filepath, "w") as f:
        json.dump(scene_data, f, indent=4)
    print("Exported:", filepath)

# Change the path to wherever you want to place the JSON 
output_path = os.path.join("C:/Users/mlemaire/Documents/GitHub/lyo-2-amiga-s2p4-04/res/JSON", "map_Hall.json") 
#C:/Users/m4w3l/Documents/GitHub/lyo-2-amiga-s2p4-04/res/JSON 
#C:/Users/mlemaire/Documents/GitHub/lyo-2-amiga-s2p4-04/res/JSON 
export_scene(output_path)