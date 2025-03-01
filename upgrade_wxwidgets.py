import os
import re

def replace_wxwidgets_version_in_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        content = file.read()

    updated_content = content.replace('wxmsw31u', 'wxmsw33u') \
                             .replace('wxbase31u', 'wxbase33u') \
                             .replace('wxmsw31d', 'wxmsw33d') \
                             .replace('wxbase31d', 'wxbase33d')

    # Replace WX version in Makefile
    updated_content = re.sub(r'WX\s*=\s*3\.1', 'WX = 3.2', updated_content)

    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(updated_content)

def update_versions_in_directory(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.vcxproj') or file.endswith('.sln') or file == 'Makefile':
                file_path = os.path.join(root, file)
                replace_wxwidgets_version_in_file(file_path)
                print(f'Updated wxWidgets version in {file_path}')

if __name__ == "__main__":
    directory = input("Enter the directory path containing the project files: ")
    update_versions_in_directory(directory)
    print("Finished updating wxWidgets versions.")