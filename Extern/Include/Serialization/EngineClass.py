class Transform():
    def __init__(self,x,y,z):
        self.x=x
        self.y=y
        self.z=z
    def set(self,x,y,z):
        self.x=x
        self.y=y
        self.z=z
    def get(self):
        return {"x":self.x,"y":self.y,"z":self.z}

class Rotation():
    def __init__(self,x,y,z):
        self.x=x
        self.y=y
        self.z=z
    def set(self,x,y,z):
        self.x=x
        self.y=y
        self.z=z
        return {"Rx": self.x, "Ry": self.y, "Rz": self.z}
    def get(self):
        return {"Rx":self.x,"Ry":self.y,"Rz":self.z}

class Character():
    def __init__(self,parent,src,transform,rotation,scale,id,item_path):
        self.type="character"
        self.parent=parent
        self.src=src
        self.Transform=transform
        self.Rotation=rotation
        self.scale=scale
        self.id=id
        self.item_path=item_path
    def set(self,parent=None,src=None,transform=None,rotation=None,scale=None,item_path=None):
        if(parent):
            self.parent=parent
        if(src):
            self.src=src
        if(transform):
            self.Transform=transform
        if(rotation):
            self.Rotation=rotation
        if(scale):
            self.scale=scale
        if(item_path):
            self.item_path=item_path
    def get(self):
        return {"type":self.type,"Parent":self.parent,"src":self.src,"Transform":self.Transform,"Rotation":self.Rotation,"Scale":self.scale,"id":self.id,"item_path":self.item_path}

class Texture():
    def __init__(self,parent,src,scale,item_path):
        self.type="texture"
        self.parent=parent
        self.src=src
        # self.Transform=transform
        # self.Rotation=rotation
        self.scale=scale
        self.item_path=item_path
    def set(self,parent=None,src=None,scale=None,item_path=None):
        if(parent):
            self.parent=parent
        if(src):
            self.src=src
        if(scale):
            self.scale=scale
        if(item_path):
            self.item_path=item_path

    def get(self):
        return {"type":self.type,"parent":self.parent,"src":self.src,"scale":self.scale,"item_path":self.item_path}

class Audio():
    def __init__(self,parent,src,StartTime,EndTime,Loop,Speed,id,item_path):
        self.type="audio"
        self.parent=parent
        self.src=src
        self.StartTime=StartTime
        self.EndTime=EndTime
        self.Loop=Loop
        self.speed=Speed
        self.id=id
        self.item_path=item_path
    def set(self,parent=None,src=None,StartTime=None,EndTime=None,Loop=False,Speed=None,item_path=None):
        if(parent):
            self.parent=parent
        if(src):
            self.src=src
        if(StartTime):
            self.StartTime=StartTime
        if(EndTime):
            self.Rotation=EndTime
        if (Loop):
            self.Loop = Loop
        if(Speed):
            self.speed=Speed
        if(item_path):
            self.item_path=item_path
    def get(self):
        return {"type":self.type,"Parent":self.parent,"src":self.src,"StartTime":self.StartTime,"EndTime":self.EndTime,"Loop":self.Loop,"Speed":self.speed,"id":self.id,"item_path":self.item_path}