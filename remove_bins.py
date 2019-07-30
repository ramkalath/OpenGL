import os

path = './'

files = []
# r=root, d=directories, f = files
for r, d, f in os.walk(path):
    for file in f:
            try:
                if(r[-3:] == "bin"):
                    files.append(os.path.join(r, file))

            except:
                pass

for f in files:
    print(f)
    os.remove(f)
