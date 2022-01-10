using System;
using System.IO.Ports;

namespace SpotlightCommunicator
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            Console.WriteLine("Available serial ports:");
            foreach (var pn in SerialPort.GetPortNames())
            {
                Console.WriteLine(pn);
            }

            Console.WriteLine("Type name of the port to communicate with.");
            var portName = Console.ReadLine();

            var sp = new SerialPort(portName, 9600, Parity.None)
            {
                DataBits = 8,
                StopBits = StopBits.One,
                Handshake = Handshake.XOnXOff,
                DtrEnable = true,
                RtsEnable = true,
            };
            sp.DataReceived += Sp_DataReceived;
            sp.Open();
            Console.WriteLine("Connected");

            Console.ReadKey();
        }

        private static void Sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var sp = sender as SerialPort;
            Console.WriteLine(sp.ReadLine());
        }
    }
}
