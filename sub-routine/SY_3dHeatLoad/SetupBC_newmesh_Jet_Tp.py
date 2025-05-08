import numpy as np
import os, shutil

def ClearFolder(folder):
    for filename in os.listdir(folder):
        file_path = os.path.join(folder, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
        except Exception as e:
            print('Failed to delete %s. Reason: %s' % (file_path, e))
    pass


def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)


try:
    with open('Automated\\Sim_InnerField\\SetupInner.ini', 'r', encoding='utf-8') as file:
        # read text files
        content_ini = file.read().split('\n')
        
        fn_Interp = content_ini[1]
        fn_mesh_inner = content_ini[7]
        fn_OF_inner = content_ini[9]
        sim_dT = int(content_ini[5])
        print(sim_dT)
        
except FileNotFoundError:
    print("配置文件未找到，请检查文件路径。")
    exit()

# set controlDict file list
lines_ctrDict = []
with open('Automated\\Sim_InnerField\\Jet_inner_case_template\\system\\controlDict','r') as f:
    for line in f:
        if ('endTime' in line) and not ('stopAt' in line) and not ('\\' in line):
            lines_ctrDict.append('REP endTime')
        else:
            lines_ctrDict.append(line)

# set boundary conditions and save to file list
# and file names of all prepared controlDict and T boundary conditions
directory = fn_Interp+'\\DataInterped'
BC_csv_list = []
fn_list_T = []
fn_list_ctrDict = []

for root, dirs, files in os.walk(directory):
    for file in files:
        BC_csv_list.append(os.path.join(root, file))

ClearFolder('Automated\\Sim_InnerField\\TandCD\\T0s')
ClearFolder('Automated\\Sim_InnerField\\TandCD\\contrlDicts')

for case_id,f_csv in enumerate(BC_csv_list):
    print(f_csv)

    xs = []
    ys = []
    zs = []
    Ts = []
    ps = []
    with open(f_csv,'r') as f:
        for line in f:
            line = line.replace('\n','')
            data = list(map(float,line.split(',')))
            xs.append(data[0])
            ys.append(data[1])
            zs.append(data[2])
            Ts.append(data[3])
            ps.append(data[4])

    T_line_orig = []
    with open('Automated\\Sim_InnerField\\Jet_inner_case_template\\0\\T_orig','r') as f:
        for line in f:
            T_line_orig.append(line)

    T_line_head = T_line_orig[:40]
    T_line_tail = T_line_orig[40:]
    T_line_values = []
    for t in Ts:
        T_line_values.append(str(t)+'\n')
    T_lines = T_line_head+T_line_values+T_line_tail
    with open('Automated\\Sim_InnerField\\TandCD\\T0s\\T_'+str(100+case_id)[1:], 'w') as fout:
        fout.writelines(T_lines)
        fn_list_T.append('T0s\\T_'+str(100+case_id)[1:])

    with open('Automated\\Sim_InnerField\\TandCD\\contrlDicts\\controlDict_'+str(100+case_id)[1:], 'w') as fout:
        # endTime         250;
        fn_list_ctrDict.append('contrlDicts\\controlDict_'+str(100+case_id)[1:])
        for line in lines_ctrDict:
            if 'REP' in line and 'endTime' in line:
                tmp_str = 'endTime         '+str((case_id+1)*sim_dT)+';\n'
                fout.write(tmp_str)
            else:
                fout.write(line)

# write out shell script to run
# buoyantBoussinesqPimpleFoam
lines_shell = ['cd '+fn_OF_inner+'\n']
for fn_t,fn_cd in zip(fn_list_T,fn_list_ctrDict):
    #fn_t = fn_t.replace('\\','/')
    #fn_cd = fn_cd.replace('\\','/')

    str_cmd_cpT = 'copy '+fn_t+' 0\\T\n'
    str_cmd_cpCD = 'copy '+fn_cd+' system\\controlDict\n'
    lines_shell.append(str_cmd_cpT)
    lines_shell.append(str_cmd_cpCD)
    lines_shell.append('buoyantBoussinesqPimpleFoam\n')

with open('Automated\\Sim_InnerField\\TandCD\\testsh.bat', 'w') as fout:
    fout.writelines(lines_shell)

copytree(fn_mesh_inner,'Automated\\Sim_InnerField\\Jet_inner_case_template\\constant\\polyMesh')
ClearFolder(fn_OF_inner)
print(fn_OF_inner,'Cleared')
copytree('Automated\\Sim_InnerField\\TandCD',fn_OF_inner)
copytree('Automated\\Sim_InnerField\\Jet_inner_case_template',fn_OF_inner)

print('all done')
print('END_FLAG')
