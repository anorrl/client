import os
from pathlib import Path

cmakelists = list(Path(".").rglob("CMakeLists.txt"))
cmakelists.pop(0) # skip the root
cmakelists_contents = {}

for item in cmakelists:
    with open(str(item)) as fh:
        contents = fh.read()
        if (".c" in contents or ".h" in contents) and not "Android" in str(item): #skip android
            cmakelists_contents[str(item)] = contents
        

for lst in cmakelists_contents:
    contents = cmakelists_contents[lst]
    directory = os.path.dirname(lst)

    files = list(Path(directory).rglob("*.cpp"))
