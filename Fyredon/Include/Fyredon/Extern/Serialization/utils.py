import json
def meta_read(src):
    meta = json.load(open(src, "r"))
    return meta
def meta_write(meta,src):
    with open(src,"w") as f:
        json.dump(meta,f)

