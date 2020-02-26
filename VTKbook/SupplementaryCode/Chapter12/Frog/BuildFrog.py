import os

tissues = ["blood.txt", "brain.txt", "brainbin.txt frog.txt",
           "duodenum.txt", "eye_retna.txt", "eye_white.txt",
           "heart.txt", "ileum.txt", "kidney.txt", "l_intestine.txt",
           "liver.txt", "lung.txt", "nerve.txt", "skeleton.txt",
           "spleen.txt", "stomach.txt"]
for t in tissues:
    print("Processing: ", t, os.system("python segmented8.py -t " + t))
print("Processing: ", "skin.txt", os.system("python marching8.py -t skin.txt"))
print("Rendering. ", os.system("python renactors.py"))
