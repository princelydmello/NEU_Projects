#!/usr/bin/python

from bs4 import BeautifulSoup
from httprequest import HTTPrequest
from httpresponse import HTTPresponse
from urlparse import *
import socket
from threading import Thread, Lock
import threading
import argparse

parser for parsing commandline
parser = argparse.ArgumentParser(description='crawls fakebook')

parser.add_argument("username", help="username of fakebook") # host address to connect to
parser.add_argument("password", help="password of fakebook") # the students id


args = parser.parse_args()

username = args.username
password = args.password
link_queue = []
visited = []
default_netloc = "data.cityofboston.gov"
host = "data.cityofboston.gov"
login_link = "https://data.cityofboston.gov/api/views/awu8-dc52/rows.json"
secret_flags = []

def sendRequest(host,request):

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host,80))

    s.send(str(request))

    response = ""
    while True:
        received = s.recv(1000)
        response = response + received
        if received == "":
            break

    s.close()
    httpresponse = HTTPresponse()
    httpresponse.parse(response)
    return httpresponse


def getCSRF(login_link):

    parsedLink = urlparse(login_link)

    request = HTTPrequest()

    request.type = "GET"
    request.host = parsedLink.netloc
    request.path = parsedLink.path
    request.version = "1.1"
    request.connection = "close"

    httpresponse = sendRequest(parsedLink.netloc,request)
    return httpresponse.csrf



def login_to_fakebook(login_link,username,password,csrf):

    parsedLink = urlparse(login_link)

    loginrequest = HTTPrequest()
    loginrequest.type = "POST"
    loginrequest.version = "1.1"
    loginrequest.host = parsedLink.netloc
    loginrequest.path = parsedLink.path
    loginrequest.connection = "Keep-Alive"
    loginrequest.cookies['csrf'] = csrf
    loginrequest.content_type = "application/x-www-form-urlencoded"
    loginrequest.content = "username="+username+"&password="+password+"&csrfmiddlewaretoken="+csrf

    loginresponse = sendRequest(parsedLink.netloc,loginrequest)
    return loginresponse


csrf = getCSRF(login_link)

response = login_to_fakebook(login_link,username,password,csrf)
session_id = response.session
max_connections = response.max_connections

link_queue.append(response.location)

default_netloc = response.location

mutex = Lock()


def crawler_thread(csrf,session_id):


    sent_links = []


    def sendRequestOnly(request,s):
        s.send(str(request))

    def sendRequests(csrf,session,s):

        while link_queue.__len__() > 0:


            mutex.acquire()


            try:
                link = link_queue.pop(0)
            except IndexError:
                mutex.release()
                break
            if link not in visited:
                #print sent_links.__len__()

                parsed = urlparse(link)
                linkrequest = HTTPrequest()
                linkrequest.type = "GET"
                linkrequest.version = "1.1"
                linkrequest.host = parsed.netloc
                linkrequest.path = parsed.path
                linkrequest.cookies['csrf'] = csrf
                linkrequest.cookies['sessionid'] = session
                linkrequest.connection = "Keep-Alive"
                #linkrequest.encoding = "gzip"

                try:
                    sendRequestOnly(linkrequest,s)
                    sent_links.append(link)
                    print(link + " "+ str(secret_flags.__len__()))

                except socket.error as err:
                    link_queue.insert(0,link)
                    mutex.release()
                    #s.close()
                    break
            mutex.release()

    def process_response(response):


        if 300 <= response.status < 400:
            visited.append(sent_links.pop(0))
            link_queue.insert(0,response.location)
        elif 500 <= response.status < 600:
            link = sent_links.pop(0)
            link_queue.insert(0,link)
        elif response.status == 200 and response.status_message == 'OK':
            visited.append(sent_links.pop(0))
            soup = BeautifulSoup(response.content,"html.parser")

            a_tags = soup.find_all('a')
            h2_tags = soup.find_all('h2',{'class':'secret_flag'})

            for h2_tag in h2_tags:
                if h2_tag.contents[0].split(" ")[1] not in secret_flags:
                    secret_flags.append(h2_tag.contents[0].split(" ")[1])

            for a_tag in a_tags:
                if urljoin(default_netloc,a_tag['href']) not in visited:
                    parsed = urlparse(a_tag['href'])
                    if (parsed.netloc == '' or parsed.netloc == urlparse(default_netloc).netloc) and \
                        (parsed.scheme == 'http' or parsed.scheme == ''):
                        if parsed.netloc == '':
                            link_queue.append(urljoin(default_netloc,a_tag['href']))
                        else:
                            link_queue.append(a_tag['href'])

    def receive_responses(s):
        response = ""
        while True:
            try:
                received = s.recv(1000)
            except socket.error:
                break
            response = response + received
            mutex.acquire()
            while response.count("HTTP/1.1") >= 2:
                httpresponse = HTTPresponse()
                second_http_index = response.find("HTTP/1.1","HTTP/1.1".__len__())
                one_response = response[0:second_http_index]
                if not one_response == "":
                    httpresponse.parse(one_response)

                    process_response(httpresponse)

                response = response[second_http_index:]
            mutex.release()

            if received == "":
                break

        mutex.acquire()
        while response.count("HTTP/1.1") >= 2:
                httpresponse = HTTPresponse()
                second_http_index = response.find("HTTP/1.1","HTTP/1.1".__len__())
                one_response = response[0:second_http_index]
                if not one_response == "":
                    httpresponse.parse(one_response)

                    process_response(httpresponse)
                response = response[second_http_index:]

        if not response == "":
            httpresponse = HTTPresponse()
            httpresponse.parse(response)
            process_response(httpresponse)

        if sent_links.__len__() > 0:
            for sent_link in sent_links:
                link_queue.insert(0,sent_link)
                if sent_links in visited:
                    visited.remove(sent_link)
        s.close()
        mutex.release()

    thread_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

    thread_socket.connect((host,80))

    sending = Thread(target=sendRequests,args=(csrf,session_id,thread_socket))
    sending.setDaemon(True)
    sending.setName("Sending Thread for "+threading.current_thread.__name__)
    sending.start()

    receiving = Thread(target=receive_responses, args=(thread_socket,))
    receiving.setDaemon(True)
    receiving.setName("Receiving Thread for "+threading.current_thread.__name__)
    receiving.start()

while True:

    if threading.enumerate().__len__() <= max_connections*2 and link_queue.__len__() > 0:


        t = Thread(target=crawler_thread,args=(csrf,session_id))
        t.setDaemon(True)
        t.start()

    if secret_flags.__len__() == 5:
        break

for secret_flag in secret_flags:
    print secret_flag