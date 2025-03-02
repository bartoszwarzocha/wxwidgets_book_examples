import os
import re

# Please set your wxWidgets versions
WX_VERSION_OLD_MAKEFILE = "3.1"
WX_VERSION_NEW_MAKEFILE = "3.2"

WX_VERSION_OLD_VS_PROJECT = "31"
WX_VERSION_NEW_VS_PROJECT = "33"

# Please don't edit here...
def replace_wxwidgets_version_in_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        content = file.read()

    # Visual Studio
    content = re.sub(rf'wx(msw|base){WX_VERSION_OLD_VS_PROJECT}(u|ud)?', rf'wx\1{WX_VERSION_NEW_VS_PROJECT}\2', content)

    # Makefile
    content = re.sub(rf'WX\s*=\s*{re.escape(WX_VERSION_OLD_MAKEFILE)}', f'WX = {WX_VERSION_NEW_MAKEFILE}', content)

    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(content)

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
