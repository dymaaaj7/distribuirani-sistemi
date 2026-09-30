using System.ServiceModel;

namespace Blanketi.G2026.Jun
{
    public class KlijentCallback : IKalkulatorCallback
    {
        public void PokaziIzraz(string izraz)
        {
            Console.WriteLine("Callback — izraz: " + izraz);
        }
    }

    // "generisana" proxy klasa — ono što bi napravio Add Service Reference,
    // na ispitu se piše ručno: nasleđuje DuplexClientBase i delegira na Channel
    public class KalkulatorClient : DuplexClientBase<IKalkulator>, IKalkulator
    {
        public KalkulatorClient(InstanceContext instance) : base(instance) { }

        public decimal Obrisi() { return Channel.Obrisi(); }
        public decimal Dodaj(decimal d) { return Channel.Dodaj(d); }
        public decimal Oduzmi(decimal d) { return Channel.Oduzmi(d); }
        public decimal Podeli(decimal d) { return Channel.Podeli(d); }
        public decimal Pomnozi(decimal d) { return Channel.Pomnozi(d); }
    }

    public class Klijent
    {
        public static void Main(string[] args)
        {
            InstanceContext instance = new InstanceContext(new KlijentCallback());
            KalkulatorClient proxy = new KalkulatorClient(instance);

            Console.WriteLine("Dodaj(2): " + proxy.Dodaj(2));
            Console.WriteLine("Dodaj(3): " + proxy.Dodaj(3));
            Console.WriteLine("Oduzmi(5): " + proxy.Oduzmi(5));
            Console.WriteLine("Pomnozi(7): " + proxy.Pomnozi(7));
            Console.WriteLine("Podeli(2): " + proxy.Podeli(2));
            Console.WriteLine("Obrisi(): " + proxy.Obrisi());
        }
    }
}
