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
                ns.Read(buff, 0, buff.Length);
                var s = Encoding.UTF8.GetString(buff);
                Console.WriteLine(s);
            }

            //Console.WriteLine("Available serial ports:");
            //foreach (var pn in SerialPort.GetPortNames())
            //{
            //    Console.WriteLine(pn);
            //}

            //Console.WriteLine("Type name of the port to communicate with.");
            //var portName = Console.ReadLine();

            //var sp = new SerialPort(portName, 9600, Parity.None)
            //{
            //    DataBits = 8,
            //    StopBits = StopBits.One,
            //    Handshake = Handshake.XOnXOff,
            //    DtrEnable = true,
            //    RtsEnable = true,
            //};
            //sp.DataReceived += Sp_DataReceived;
            //sp.Open();
            //Console.WriteLine("Connected");

            //Console.ReadKey();
        }

        //private static void Sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        //{
        //    var sp = sender as SerialPort;
        //    Console.WriteLine(sp.ReadLine());
        //}
    }
}
