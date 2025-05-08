import os, shutil
import numpy as np
print(os.getcwd())
try:
    with open('Automated\\InterpBC\\InterpBC.ini', 'r', encoding='utf-8') as file:
        # read text files
        content_ini = file.read().split('\n')
        fn_mesh_outter = content_ini[1]
        tmp_mesh_info_outter = content_ini[3].split(',')
        nPoints_outter = int(tmp_mesh_info_outter[0].split(':')[-1])
        nCells_outter = int(tmp_mesh_info_outter[1].split(':')[-1])
        nFaces_outter = int(tmp_mesh_info_outter[2].split(':')[-1])
        tmp_surf_outter = content_ini[5].split(',')
        startFace_outter = int(tmp_surf_outter[0].split(':')[-1])
        nFaces_surf_outter = int(tmp_surf_outter[1].split(':')[-1])
        
        fn_mesh_inner = content_ini[7]
        tmp_mesh_info_inner = content_ini[9].split(',')
        nPoints_inner = int(tmp_mesh_info_inner[0].split(':')[-1])
        nCells_inner = int(tmp_mesh_info_inner[1].split(':')[-1])
        nFaces_inner = int(tmp_mesh_info_inner[2].split(':')[-1])
        tmp_surf_inner = content_ini[11].split(',')
        startFace_inner = int(tmp_surf_inner[0].split(':')[-1])
        nFaces_surf_inner = int(tmp_surf_inner[1].split(':')[-1])

        fn_OF_outter = content_ini[13]
        fn_OF_inner = content_ini[15]
        
except FileNotFoundError:
    print("配置文件未找到，请检查文件路径。")
    exit()

try:
    with open(fn_mesh_outter+'\\points', 'r') as f:
        flag_ = False
        pts_lines_outter = []
        for line in f:
            if line == str(nPoints_outter)+'\n':
                flag_ = True
            if flag_:
                pts_lines_outter.append(line)
        pts_lines_outter = pts_lines_outter[2:2+nPoints_outter]

    xyz_outter = []
    for xyz in pts_lines_outter:
        xyz = xyz[1:-2]
        xyz_outter.append(list(map(float,xyz.split(' '))))

except FileNotFoundError:
    print("外场网格文件points未找到，请检查文件路径。")
    exit()

try:
    with open(fn_mesh_outter+'\\faces', 'r') as f:
        flag_ = False
        faces_lines_outter = []
        for line in f:
            if line == str(nFaces_outter)+'\n':
                flag_ = True
            if flag_:
                faces_lines_outter.append(line)
        faces_lines_outter = faces_lines_outter[2:2+nFaces_outter]
        faces_lines_outter = faces_lines_outter[startFace_outter:startFace_outter+nFaces_surf_outter]

    faces_vetex_outter = []
    for face in faces_lines_outter:
        face = face[2:-2]
        faces_vetex_outter.append(list(map(int,face.split(' '))))

except FileNotFoundError:
    print("外场网格文件faces未找到，请检查文件路径。")
    exit()

try:
    with open(fn_mesh_inner+'\\points', 'r') as f:
        flag_ = False
        pts_lines_inner = []
        for line in f:
            if line == str(nPoints_inner)+'\n':
                flag_ = True
            if flag_:
                pts_lines_inner.append(line)
        pts_lines_inner = pts_lines_inner[2:2+nPoints_inner]

    xyz_inner = []
    for xyz in pts_lines_inner:
        xyz = xyz[1:-2]
        xyz_inner.append(list(map(float,xyz.split(' '))))

except FileNotFoundError:
    print("内场网格文件points未找到，请检查文件路径。")
    exit()

try:
    with open(fn_mesh_inner+'\\faces', 'r') as f:
        flag_ = False
        faces_lines_inner = []
        for line in f:
            if line == str(nFaces_inner)+'\n':
                flag_ = True
            if flag_:
                faces_lines_inner.append(line)
        faces_lines_inner = faces_lines_inner[2:2+nFaces_inner]
        faces_lines_inner = faces_lines_inner[startFace_inner:startFace_inner+nFaces_surf_inner]

    faces_vetex_inner = []
    for face in faces_lines_inner:
        face = face[2:-2]
        faces_vetex_inner.append(list(map(int,face.split(' '))))

except FileNotFoundError:
    print("内场网格文件faces未找到，请检查文件路径。")
    exit()


xyz_inner = np.array(xyz_inner)
xyz_surf_inner = []
for vertex in faces_vetex_inner:
    xyz_tmp = xyz_inner[vertex]
    xyz_ = np.average(xyz_tmp,axis=0)
    xyz_surf_inner.append(xyz_)
xyz_surf_inner = np.array(xyz_surf_inner)

xyz_outter = np.array(xyz_outter)
xyz_surf_outter = []
for vertex in faces_vetex_outter:
    xyz_tmp = xyz_outter[vertex]
    xyz_ = np.average(xyz_tmp,axis=0)
    xyz_surf_outter.append(xyz_)
xyz_surf_outter = np.array(xyz_surf_outter)

try:
    with open(fn_mesh_outter+'\\owner', 'r') as f:
        flag_ = False
        owner_lines_outter = []
        for line in f:
            if line == str(nFaces_outter)+'\n':
                flag_ = True
            if flag_:
                owner_lines_outter.append(line)
        owner_lines_outter = owner_lines_outter[2:2+nFaces_outter]
        owner_lines_outter = owner_lines_outter[startFace_outter:startFace_outter+nFaces_surf_outter]
        owners_outter = list(map(int,owner_lines_outter))
        
    
except FileNotFoundError:
    print("外场网格文件owner未找到，请检查文件路径。")
    exit()

subdirectories = []
path = fn_OF_outter #'C:\\OpenFOAMCase\\J20_rhosimple_batch_res'
for item in os.listdir(path):
    item_path = os.path.join(path, item)
    if os.path.isdir(item_path):
        subdirectories.append(item_path+"\\300\\")

dataframe_lines = []
for case_id,f_dir in enumerate(subdirectories):
    fn_T = f_dir+'T'
    fn_P = f_dir+'p'
    if not (os.path.isfile(fn_T) and os.path.isfile(fn_P)):
        print('计算未完成',f_dir)
        continue
    
    v_list = []
    T_list = []
    with open(fn_T, 'r') as f:
        for i in range(21):
            f.readline()
        line = f.readline()
        num_T = int(line)
        f.readline()
        for i in range(num_T):
            line = f.readline()
            T_value = float(line)
            T_list.append(T_value)
            #xyz_list.append([xs[i],ys[i],zs[i]])
    
    p_list = []
    with open(fn_P, 'r') as f:
        for i in range(21):
            f.readline()
        line = f.readline()
        num_p = int(line)
        f.readline()
        for i in range(num_p):
            line = f.readline()
            p_value = float(line)
            p_list.append(p_value)
    
    cells_own_jet_surf = owners_outter
    T_list = list(np.array(T_list)[cells_own_jet_surf])
    p_list = list(np.array(p_list)[cells_own_jet_surf])

    #print(len(T_list),len(p_list),xyz_surf_outter.shape)
    dataframe_lines.append('case'+str(case_id+1)+'\n')
    for i in range(len(p_list)):
        line_data_str = ''
        line_data_str += 'cell:'+str(i)+',xyz=('
        line_data_str += str(xyz_surf_outter[i][0])+' '+str(xyz_surf_outter[i][1])+' '+str(xyz_surf_outter[i][2])+'),'
        line_data_str += 'T=' + str(T_list[i]) + ',p=' + str(p_list[i])+'\n'
        dataframe_lines.append(line_data_str)

with open('Automated\\InterpBC\\Jet_data_select.csv', 'w') as fout:
    for line in dataframe_lines:
        fout.write(line)

data_lines = []
with open('Automated\\InterpBC\\Jet_data_select.csv','r') as f:
    for line in f:
        line = line.replace('\n','')
        data_lines.append(line)

case_num = 0

data_frame = []
for data in data_lines:
    if 'case' in data:
        case_num += 1
    else:
        data_split = data.split(',')
        section_xyz = (data_split[1])[:-1]
        section_xyz = section_xyz.split('(')[-1]
        xyz_ = list(map(float,section_xyz.split(' ')))
        section_T = data_split[2]
        section_p = data_split[3]
        T_ = float(section_T.split('=')[-1])
        p_ = float(section_p.split('=')[-1])
        data_frame.append(xyz_+[T_,p_])

data_block_len = int(len(data_frame)/case_num)
data_case_list = []
for i in range(case_num):
    data_case_list.append(np.array(data_frame[i*data_block_len:(i+1)*data_block_len]))

def Get_TP_value_on_point(point_xyz,data_case_list,case_id):
    xyz_tmp = data_case_list[case_id][:,:3]
    T_tmp = data_case_list[case_id][:,3]
    p_tmp = data_case_list[case_id][:,4]

    xyz_diff = xyz_tmp-point_xyz
    dist = np.linalg.norm(xyz_diff,axis=1)
    weight = 1./dist
    T_res = np.average(T_tmp,weights=weight)
    p_res = np.average(p_tmp,weights=weight)

    return T_res,p_res

surface_coords = xyz_surf_inner.tolist()
for case_id,data_case in enumerate(data_case_list):
    print(case_id)
    surface_T = []
    surface_p = []
    for i in range(len(surface_coords)):
        T,p = Get_TP_value_on_point(surface_coords[i],data_case_list,case_id)
        surface_T.append(T)
        surface_p.append(p)
    print(len(surface_p),len(surface_T),len(surface_coords))

    with open('Automated\\InterpBC\\DataInterped\\Jet_surface_Tp_case'+str(1000+case_id)[1:]+'.csv', 'w') as fout:
        for i in range(len(surface_coords)):
            x,y,z = surface_coords[i]
            T = surface_T[i]
            p = surface_p[i]
            line_ = str(x)+','+str(y)+','+str(z)+','+str(T)+','+str(p)+'\n'
            fout.write(line_)
print('END_FLAG')
