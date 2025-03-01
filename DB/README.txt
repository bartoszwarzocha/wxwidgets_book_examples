===========================================================
Installation Guide: SOCI and Firebird on Linux
===========================================================

1. Install Firebird 3.0 (or other)
   - Update package lists: `sudo apt update`
   - Install the Firebird server and tools: `sudo apt install firebird3.0-server firebird3.0-utils`
   - Start Firebird and enable it to run on boot:
     - `sudo systemctl start firebird3.0`
     - `sudo systemctl enable firebird3.0`

2. Install SOCI
   - Install SOCI and its dependencies: `sudo apt install libsoci-dev`
   - If using vcpkg, install SOCI via: `./vcpkg install soci:x64-linux`

3. Verify Installation
   - Check if Firebird is running: `sudo systemctl status firebird3.0`
   - Check if SOCI headers are installed: `ls /usr/include/soci/`

===========================================================
Create database
===========================================================

1. Install FalemRobin tool (available on Windows and Linux)

2. Register new SHOP.fdb database:
   - user: SYSDBA
   - passwd: masterkey
   - charset: UTF-8
   - role: [set empty]
   - page-size: 4096
   
3. Use DB_SETUP.sql file to create database structure

Have fun!