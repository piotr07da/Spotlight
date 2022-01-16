using System;
using System.Net.Sockets;
using System.Text;

namespace SpotlightCommunicator
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            var tcpClient = new TcpClient("192.168.0.45", 33334);
            var ns = tcpClient.GetStream();
            var buff = new byte[1024];
            while (ns.CanRead)
            {
                var bytesRead = ns.Read(buff);
                var s = Encoding.UTF8.GetString(buff.AsSpan(0, bytesRead));
                Console.WriteLine(s);
            }
        }
    }
}
