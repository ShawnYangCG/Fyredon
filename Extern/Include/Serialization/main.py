import random
from utils import meta_write, meta_read
from EngineClass import Rotation, Transform,Character,Texture,Audio

def set_character(src,parent=None,x=None,y=None,z=None,Rx=None,Ry=None,Rz=None,scale=None,item_path=None):
    meta=meta_read(src=src)
    character=Character(parent=meta["Parent"],src=src,transform=meta["Transform"],rotation=meta["Rotation"],scale=meta["Scale"],id=meta["id"],item_path=meta["item_path"])
    if(parent):
        character.set(parent=parent)
    if(x and y and z):
        transform = Transform(x=x, y=y, z=z)
        character.set(transform=transform)
    else:
        flag=False
        if (x and y):
            transform = Transform(x=x, y=y, z=meta["z"])
            character.set(transform=transform)
            flag=True
        if (x and z):
            transform = Transform(x=x, y=meta["y"], z=z)
            character.set(transform=transform)
            flag = True
        if (y and z):
            transform = Transform(x=meta["x"], y=y, z=z)
            character.set(transform=transform)
            flag = True
        if(not flag):
            if (x):
                transform=Transform(x=x,y=meta["y"],z=meta["z"])
                character.set(transform=transform)
            if (y):
                transform = Transform(x=meta["x"], y=y, z=meta["z"])
                character.set(transform=transform)
            if (z):
                transform = Transform(x=x, y=meta["y"], z=meta["z"])
                character.set(transform=transform)


    if (Rx and Ry and Rz):
        rotation = Rotation(x=Rx, y=Ry, z=Rz)
        character.set(rotation=rotation)
    else:
        flag = False
        if (Rx and Ry):
            rotation = Rotation(x=Rx, y=Ry, z=meta["Rz"])
            character.set(rotation=rotation)
            flag = True
        if (Rx and Rz):
            rotation = Rotation(x=Rx, y=meta["Ry"], z=Rz)
            character.set(rotation=rotation)
            flag = True
        if (Ry and Rz):
            rotation = Rotation(x=meta["Rx"], y=Ry, z=Rz)
            character.set(rotation=rotation)
            flag = True
        if (not flag):
            if (Rx):
                rotation = Rotation(x=Rx, y=meta["Ry"], z=meta["Rz"])
                character.set(rotation=rotation)
            if (Ry):
                rotation = Rotation(x=meta["Rx"], y=Ry, z=meta["Rz"])
                character.set(rotation=rotation)
            if (Rz):
                rotation = Rotation(x=Rx, y=meta["Ry"], z=meta["Rz"])
                character.set(rotation=rotation)
    if(scale):
        character.set(scale=scale)
    if(item_path):
        character.set(item_path=item_path)
    meta = {"type": "Character", "src": character.get()["src"], "Parent": character.get()["Parent"],
            "x": character.get()["Transform"].get()["x"], "y": character.get()["Transform"].get()["y"],
            "z": character.get()["Transform"].get()["z"],
            "Rx": character.get()["Rotation"].get()["Rx"], "Ry": character.get()["Rotation"].get()["Ry"],
            "Rz": character.get()["Rotation"].get()["Rz"], "Scale": character.get()["Scale"],
            "id": character.get()["id"],"item_path":character.get()["item_path"]}
    meta_write(meta,src)
    return character

def set_audio(src,parent=None,StartTime=None,EndTime=None,Loop=None,Speed=None,item_path=None):
    meta=meta_read(src=src)
    audio=Audio(src=src,parent=meta["Parent"],StartTime=meta["StartTime"],EndTime=meta["EndTime"],Loop=meta["Loop"],Speed=meta["Speed"],item_path=meta["item_path"])
    if(parent):
        audio.set(parent=parent)
    if(StartTime):
        audio.set(StartTime=StartTime)
    if(EndTime):
        audio.set(EndTime=EndTime)
    if(Loop):
        audio.set(Loop=Loop)
    if(Speed):
        audio.set(Speed=Speed)
    if(item_path):
        audio.set(item_path=item_path)
    meta = {"type": "Audio", "src": audio.get()["src"], "Parent": audio.get()["Parent"],"StartTime":audio.get()["StartTime"],"EndTime":audio.get()["EndTime"],
            "Loop":audio.get()["Loop"],"Speed":audio.get()["Speed"], "item_path":audio.get()["item_path"]
            }
    meta_write(meta,src)
    return audio

def set_texture(src,parent=None,scale=None,item_path=None):
    meta=meta_read(src=src)
    texture=Texture(src=src,parent=meta["Parent"],scale=meta["Scale"],item_path=meta["item_path"])
    if(parent):
        texture.set(parent=parent)
    if(scale):
        texture.set(scale=scale)
    if(item_path):
        texture.set(item_path=item_path)
    meta_write(meta,src)
    meta = {"type": "Texture", "src": texture.get()["src"], "Parent": texture.get()["Parent"],
            "item_path": texture.get()["item_path"]}
    return meta

def create_character(src,parent=None,x=None,y=None,z=None,Rx=None,Ry=None,Rz=None,scale=None,id=None,item_path=None):
    transform = Transform(x=x, y=y, z=z)
    rotation = Rotation(x=Rx, y=Ry, z=Rz)
    if(id):
        character=Character(src=src,parent=parent,transform=transform,rotation=rotation,scale=scale,id=id,item_path=item_path)
        meta={"type":"Character","src":character.get()["src"],"Parent":character.get()["Parent"],"x":character.get()["Transform"].get()["x"],"y":character.get()["Transform"].get()["y"],"z":character.get()["Transform"].get()["z"],
          "Rx":character.get()["Rotation"].get()["Rx"],"Ry":character.get()["Rotation"].get()["Ry"],"Rz":character.get()["Rotation"].get()["Rz"],"Scale":character.get()["Scale"],"id":character.get()["id"],"item_path":character.get()["item_path"]}
    else:
        id=random.randint(100000,999999)
        character = Character(src=src, parent=parent, transform=transform, rotation=rotation, scale=scale, id=id,item_path=item_path)
        meta = {"type": "Character", "src": character.get()["src"], "Parent": character.get()["Parent"],
                "x": character.get()["Transform"].get()["x"], "y": character.get()["Transform"].get()["y"],
                "z": character.get()["Transform"].get()["z"],
                "Rx": character.get()["Rotation"].get()["Rx"], "Ry": character.get()["Rotation"].get()["Ry"],
                "Rz": character.get()["Rotation"].get()["Rz"], "Scale": character.get()["Scale"],
                "id": id,"item_path":character.get()["item_path"]}

    meta_write(meta,src)
    return character

def create_audio(src,parent=None,StartTime=None,EndTime=None,Loop=None,Speed=None,id=None,item_path=None):


    if(id):
        id=random.randint(100000,999999)
        audio = Audio(src=src, parent=parent, StartTime=StartTime, EndTime=EndTime, Loop=Loop, Speed=Speed,id=id,item_path=item_path)
        meta = {"type": "Audio", "src": audio.get()["src"], "Parent": audio.get()["Parent"],
              "StartTime":audio.get()["StartTime"],"Endtime":audio.get()["EndTime"],"Loop":audio.get()["Loop"],"Speed":audio.get()["Speed"],"id":audio.get()["id"],"item_path":audio.get()["item_path"]}
    else:
        audio = Audio(src=src, parent=parent, StartTime=StartTime, EndTime=EndTime, Loop=Loop, Speed=Speed,id=id,item_path=item_path)
        meta = {"type": "Audio", "src": audio.get()["src"], "Parent": audio.get()["Parent"],
                "StartTime": audio.get()["StartTime"], "Endtime": audio.get()["EndTime"], "Loop": audio.get()["Loop"],
                "Speed": audio.get()["Speed"],"id":audio.get()["id"],"item_path":audio.get()["item_path"]}
    meta_write(meta, src)
    return audio

def create_texture(src,parent=None,item_path=None,scale=None):
    texture=Texture(src=src,parent=parent,scale=scale,item_path=item_path)
    meta={"type":"Texture","src":src,"parent":parent,"scale":scale,"item_path":item_path}
    meta_write(meta,src)
    return texture

if __name__=="__main__":
   create_character(src="./asset/character_example.json",item_path="fireman.obj",x=0,y=0,z=0,Rx=0,Ry=0,Rz=0,scale=0,id=10080)
   create_audio(src="./asset/audio_example.json",item_path="Mojave.mp3", StartTime=0.0,EndTime=3.45,Loop=True,Speed=1.0,id=10010,parent=10080)
   create_texture(src="./asset/texture_example.json",item_path="Test.png",scale=1.0,parent=10080)

