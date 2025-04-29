import os

def extract_lib_filenames(directory):
    # 获取当前目录下所有文件
    files = os.listdir(directory)
    # 过滤出所有.lib文件
    lib_files = [f for f in files if f.endswith('.lib')]
    
    # 将文件名写入txt文件
    with open('lib_filenames.txt', 'w') as file:
        for lib_file in lib_files:
            file.write(lib_file + '\n')

if __name__ == "__main__":
    # 获取当前目录
    current_directory = os.getcwd()
    extract_lib_filenames(current_directory)
