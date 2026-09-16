namespace WCFKalkulatorClient
{
    // duplex klijent — minimalna ispitna verzija:
    // 1. callback klasa  2. InstanceContext  3. DuplexChannelFactory  4. pozivi
    public class KalkulatorCallback : WCFKalkulator.IKalkulatorCallback
    {
        public void Rezultat(WCFKalkulator.Rezultat r)
        {
            Console.WriteLine($"{r.Izraz} = {r.Vrednost}");
        }
    }

    public class Client
    {
        public static void Main(string[] args)
        {
            var factory = new System.ServiceModel.DuplexChannelFactory<WCFKalkulator.IKalkulator>(
                new System.ServiceModel.InstanceContext(new KalkulatorCallback()),
                new System.ServiceModel.WSDualHttpBinding(),
                new System.ServiceModel.EndpointAddress("http://localhost:8000/Kalkulator"));

            WCFKalkulator.IKalkulator proxy = factory.CreateChannel();

            proxy.Dodaj(5);
            proxy.Oduzmi(3);
            proxy.Pomnozi(4);
            proxy.Podeli(2);
        }
    }
}
