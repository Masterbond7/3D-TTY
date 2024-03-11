
import time
file_name = input("Path to and name of file: ")
out_file_name = "." + file_name.split(".")[1] + ".3dtty"

file = open(file_name, 'r')
out_file = open(out_file_name, 'w')
time.sleep(1)
out_file.write(str(len(file.readlines())// 8)+"\n")
file.close()
file = open(file_name, 'r')

data = {1: [0,0,0], 2: [0,0,0], 3: [0,0,0], "n": [0,0,0]}
v_count = 0
for line in file:
    if " facet " in line:
        data[1] = [0,0,0]
        data[2] = [0,0,0]
        data[3] = [0,0,0]
        data["n"] = [0,0,0]
        v_count = 0

        entry = line.split(" ")
        data["n"][0] = float(entry[3])
        data["n"][1] = float(entry[4])
        data["n"][2] = float(entry[5])
    
    if " vertex " in line:
        v_count += 1
        entry = line.split(" ")
        data[v_count][0] = float(entry[3])
        data[v_count][1] = float(entry[4])
        data[v_count][2] = float(entry[5])
    
    if " endloop" in line:
        for vertex in data:
            for point in data[vertex]:
                out_file.write(str(point))
                out_file.write(" ")
        out_file.write("\n")
        print(data)
out_file.close()
