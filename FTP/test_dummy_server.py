from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer

FTP_DIRECTORY = "c:/wxwidgets_book_examples/FTP/"  # Published folder FTP (Windows)
# FTP_DIRECTORY = "/home/user/ftp"  # Published folder FTP (Linux)

USERNAME = "test"
PASSWORD = "ftp2017"

# User config
authorizer = DummyAuthorizer()
authorizer.add_user(USERNAME, PASSWORD, FTP_DIRECTORY, perm="elradfmw")  # All permissions
authorizer.add_anonymous(FTP_DIRECTORY, perm="elr")  # Anonymous acces for read only

# Server config
handler = FTPHandler
handler.authorizer = authorizer

# Run server at 127.0.0.1:21
server = FTPServer(("127.0.0.1", 21), handler)
print(f"Serwer FTP działa na ftp://127.0.0.1:21")
server.serve_forever()
