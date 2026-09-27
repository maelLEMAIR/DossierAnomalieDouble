import bpy

props = {
    "has_collider": True,
    "geometry": "Cube",
    "has_mesh": False,
    "isLight": False,
    "isDoor": False,
    "typeOfAnomalie": [],
}

selected_objects = bpy.context.selected_objects
#for obj in selected_objects:
#    if "typeOfAnomalie" in obj:
#        del obj["typeOfAnomalie"]

#for obj in selected_objects:
#    for key, value in props.items():
#        if key not in obj:
#            obj[key] = value

for obj in selected_objects:
    for key, value in props.items():
        if "typeOfAnomalie" == key:
            obj[key] = value
        
#for obj in selected_objects:
#    for key, value in props.items():
#        obj[key] = value