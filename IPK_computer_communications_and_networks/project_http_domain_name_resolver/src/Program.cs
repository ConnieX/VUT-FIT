using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Linq;

namespace proj1
{
    public class Parser{
        public string Body {get; set;} //variable for body of request
        public string  Method {get; set;} //variable for name of method of request (GET/POST)
        public string URL {get; set;} //variable for URL of request
        public Dictionary<string, string> Params {get; set;} = new Dictionary<string, string>(); //variable for parameters of request (name, type)

        //function for request parsing
        public Parser(string req){
            var lines = req.Split("\n"); //spliting to lines
            var block = lines[0].Split(" "); //spliting first line to method, URL, http
            Method = block[0];
            var path = block[1].Split("?"); //get "/resolve" and "?..." or "/dns-query"
            URL = path[0]; //(not actually URL but I have no clue how to name it - contains "/resolve")
            //if we have required params for GET
            if(path.Length > 1){
                //splitting individual parts of query
                foreach(var item in path[1].Split("&")){
                    var parts = item.Split("="); //splitting to key and value
                    Params.Add(parts[0], parts[1]);
                }
            }
            Body = req.Split("\r\n\r\n")[1]; //get body of request
       }

       //switching method
        public (string, int) GetResponse(){
            switch(Method){
                case "GET":
                    return Get();
                case "POST":
                    return Post();
                default:
                    break;            
            }
            return (null, 405); //Bad Method
        }

        //method GET
        private (string, int) Get(){
            //in "URL" has to be "/resolve", have to have 2 keys - name and type
            if(URL != "/resolve" || Params.Keys.Count != 2 || !Params.Keys.Contains("name") || !Params.Keys.Contains("type")){
               return (null, 400); //Bad Request
            }
            IPHostEntry entry;
            try{
                entry = Dns.GetHostEntry(Params["name"]); //get host name from ip adress or get ip adress from host name
            }
            catch(Exception){
                return (null, 404);
            }
            string result = "";
            //if I got host name -> find IP adress
            if(Params["type"] != "PTR"){
                var ip = entry.AddressList.First(x=>x.AddressFamily == AddressFamily.InterNetwork); //find IPv4 adress in gained adresses
                result = Params["name"] + ":" + Params["type"] + "=" + ip;
            }
            //else I got IP adress -> find host name
            else{
                result = Params["name"] + ":" + Params["type"] + "=" + entry.HostName;
            }
            return (result, 200); //success
        }

        //method POST
        private (string, int) Post(){
            //in "URL" has to be "/dns-query", has to have 0 keys
            if(URL != "/dns-query" || Params.Keys.Count != 0){
               return (null, 400); //Bad Request
            }
            string result = "";
            //split body to lines
            foreach(var line in Body.Split("\n")){
                if(string.IsNullOrEmpty(line)){
                    continue;
                }
                var parts = line.Split(":"); //splitting tot URL and type
                IPHostEntry entry;
                try{
                    entry = Dns.GetHostEntry(parts[0]); //get host name from ip adress or get ip adress from host name
                }
                catch(Exception){
                    continue;
                }
                //if I got host name -> fing IP adress
                if(parts[1] == "A"){
                    var ip = entry.AddressList.First(x=>x.AddressFamily == AddressFamily.InterNetwork); //find IPv4 adress in gained adresses
                    result += parts[0] + ":" + parts[1] + "=" + ip + "\n";
                }
                //else I got IP adress -> find host name
                else if(parts[1] == "PTR"){
                    result += parts[0] + ":" + parts[1] + "=" + entry.HostName + "\n";
                }
                else{
                    return (null, 400);
                }
            }       
            return (result, 200); //success
        }

       public string ComposeResponse(string body, int status){
            string statusText = "";
            switch(status){
                case 200:
                    statusText = " OK\n\n";
                    break;
                case 400:
                    statusText = " Bad Request\n\n";
                    break;
                case 404:
                    statusText = " Not Found\n\n";
                    break;
                case 405:
                    statusText = " Bad Method\n\n";
                    break;
                default:
                    break;
            }
            return "HTTP/1.1 " + status + statusText + body;
        }
    }
    class Program
    {
        public static void StartListening(int port) {
            var bytes = new byte[8192]; //array for request
            string data; //for converting input data to text
            var ipAddress = IPAddress.Parse("127.0.0.1"); //local host
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, port);  

            // create a TCP/IP socket
            Socket listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);  
    
            // bind the socket to the local endpoint and listen for incoming connections
            try {  
                listener.Bind(localEndPoint); //use specified adress and port for communication
                listener.Listen(8); //max count of waiting devices (probably only one while testing, but why don't allow more)
    
                //waiting for connections
                while (true) {  
                    Socket handler = listener.Accept(); //create connection
                    data = "";  

                    var bytesRec = handler.Receive(bytes); //accepting bytes from connection
                    data = Encoding.ASCII.GetString(bytes, 0, bytesRec); //converting bytes to text  
                    Parser parsedData = new Parser(data); //creating new object Parser - get parsed data
                    var result = parsedData.GetResponse(); //calling requeried method
                    var httpResponse = parsedData.ComposeResponse(result.Item1, result.Item2); //return value
                    byte[] msg = Encoding.ASCII.GetBytes(httpResponse); //coverting text to bytes
  
                    handler.Send(msg); 
                    handler.Shutdown(SocketShutdown.Both); //close socket
                    handler.Close(); //close connection
                }  
    
            }
            //if port is already used
            catch (Exception e) {
                Console.WriteLine(e.ToString());
            }
        }

        static void Main(string[] args){
            try{
            //need (at least) one argument - port
            if(args.Length < 1){
                Console.Error.WriteLine("Please specify port.\n");
                return;
            }
            var port = 0;
            //try convert first argument to int
            if (!int.TryParse(args[0], out port)){
                Console.Error.WriteLine("Please specify port (first argument).\n");
                return;
            }
            if(port < 0 || port > 65535){
                Console.Error.WriteLine("Port has to be in range 0 to 65535.\n");
                return;
            }
            StartListening(port);
            }
            catch (Exception e){
                Console.WriteLine(e.ToString());
            }
        }
    }
}