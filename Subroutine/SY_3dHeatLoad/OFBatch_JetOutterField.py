import os, shutil
import numpy as np

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
    print('ClearDone')


def copytree(src, dst, symlinks=False, ignore=None):
    print("copying")
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)
    print('copy done!')


def replace_a_line(new_line, flags, filein, fileout):    
    with open(filein) as fin, open(fileout, 'w') as fout:
        for line in fin:
            lineout = line
            if flags in line: # .strip() == original_line:
                lineout = f"{new_line}\n"
                print('line replaced ',new_line)
            fout.write(lineout)

try:
    with open('Automated\\Sim_OutterField\\setupOutter.ini', 'r', encoding='utf-8') as file:
        # read text files
        
        content_ini = file.read().split('\n')
        fn_polymesh = content_ini[1]
        dst = content_ini[3]+'\\'

        tmp_case_setting = content_ini[5].split(',')
        vs_list = list(map(float,tmp_case_setting))
        v0_min = min(vs_list)
        v0_max = max(vs_list)
        npts = len(vs_list)
        relex_factor = float(content_ini[7])
        
except FileNotFoundError:
    print("配置文件未找到，请检查文件路径。")
    exit()

src = "Automated\\Sim_OutterField\\Jet_rhosimple"

ClearFolder(dst)
ClearFolder('Automated\\Sim_OutterField\\Jet_rhosimple\\constant\\polyMesh')
print('clear')
copytree(fn_polymesh,'Automated\\Sim_OutterField\\Jet_rhosimple\\constant\\polyMesh')

print(v0_min,v0_max,npts)

for id_,v0_ in enumerate(np.array(vs_list)):
    print(id_,v0_)
    # line_new = '        value           uniform ('+ str(v0_) +' 0 0);'
    line_new = 'Uinlet          (0 0 -'+str(v0_)+');'
    dst_case = dst+'case'+str(10**6+id_)
    copytree(src,dst_case)
    fn_tmp = src+"\\0\\U"
    fn_dst = dst_case+"\\0\\U"
    replace_a_line(line_new,'(0 2.71828 3.14159);',fn_tmp,fn_dst)
    fn_tmp_ = src+"\\system\\fvSolution"
    fn_dst_ = dst_case+"\\system\\fvSolution"
    line_new_ = '        U               '+str(relex_factor)
    replace_a_line(line_new_,'0.9931415926;//factor',fn_tmp_,fn_dst_)

with open(dst+'\\testbash.bat', 'w') as fout:
    for i in range(npts):
        fout.write('cd case'+str(10**6+i)+'\n')
        fout.write('rhoSimpleFoam\n')
        fout.write('cd ..\n')
print('all done')
print('END_FLAG')
