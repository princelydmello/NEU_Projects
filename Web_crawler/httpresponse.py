
import gzip
from StringIO import StringIO
from io import BytesIO
import warnings


class HTTPresponse():

    def __init__(self):
        self.version = ""
        self.status = 0
        self.status_message = ""
        self.csrf = ""
        self.session = ""
        self.location = ""
        self.content_length = 0
        self.max_connections = 0
        self.content_encoding = ""
        self.transfer_encoding = ""
        self.content = ""

    def  ishex(self,string):
        try:
            int(string, 16)
            return True
        except ValueError:
            return False

    def parse(self,response):
        lines = response.split('\r\n')

        status_line = lines[0]

        status_parts = status_line.split(" ")


        self.version = status_parts[0][-3:]
        self.status = int(status_parts[1])
        self.status_message = status_parts[2]

        cookies = filter(lambda line: line[0:10] == "Set-Cookie",lines)

        if cookies.__len__() == 2:
            csrfcookie = cookies[0]
            sessioncookie = cookies[1]

            self.csrf = csrfcookie[csrfcookie.index('=')+1:csrfcookie.index(';')]
            self.session = sessioncookie[sessioncookie.index('=')+1:sessioncookie.index(';')]

        elif cookies.__len__() == 1:
            sessioncookie = cookies[0]
            self.session = sessioncookie[sessioncookie.index('=')+1:sessioncookie.index(';')]

        lines_len_14 = filter(lambda line: line.__len__() >= 14,lines)

        content_length_lines = filter(lambda line: line[0:14] == "Content-Length",lines_len_14)
        content_encoding_lines = filter(lambda line: line[0:16] == "Content-Encoding",lines_len_14)

        keep_alive_lines = filter(lambda line: line[0:10] == "Keep-Alive",lines)
        transfer_encoding_lines = filter(lambda line: line[0:17] == "Transfer-Encoding",lines_len_14)

        if transfer_encoding_lines.__len__() >= 1:
            self.transfer_encoding = transfer_encoding_lines[0].split(' ')[1]

        if keep_alive_lines.__len__() >= 1:
            self.max_connections = int(keep_alive_lines[0].split(' ')[2].split('=')[1])

        if content_encoding_lines.__len__() >= 1:
            self.content_encoding = content_encoding_lines[0].split(' ')[1]

        if self.status >= 300 and self.status < 400:
            location_lines = filter(lambda line: line[0:8] == "Location",lines)
            self.location = location_lines[0].split(' ')[1]

        if content_length_lines.__len__() >= 1:
            self.content_length = int(content_length_lines[0].split(' ')[1])

        blank_line = False

        for line in lines:

            if blank_line:
                if self.transfer_encoding == 'chunked':
                        if line != '' and not self.ishex(line.strip()):
                            self.content = self.content + line
                else:
                    self.content = self.content + line

            if line == '':
                blank_line = True

        if self.content_encoding == "gzip" and not self.content == "":
            print response

            try:
                gf = gzip.GzipFile('', 'rb', 9, BytesIO(bytes(self.content)))
                print gf.read().decode('utf-8')
            except:
                print gf.extrabuf.decode('utf-8')

            print self.content
            #print html_data
            #self.content = zlib.decompress(StringIO(self.content).read(),31)

