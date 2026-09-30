namespace Septembar
{
    // "generisana" proxy klasa (Add Service Reference forа) — na ispitu ručno:
    // ClientBase<T> + delegiranje na Channel
    public class TacnoVremeClient : ClientBase<ITacnoVreme>, ITacnoVreme
    {
        public TacnoVremeClient() : base() { }
        public TacnoVremeClient(System.ServiceModel.Channels.Binding binding,
                                System.ServiceModel.EndpointAddress adresa)
            : base(binding, adresa) { }

        public DateTime TrenutnoVreme() { return Channel.TrenutnoVreme(); }
        public int BrojPoziva() { return Channel.BrojPoziva(); }
    }

    public class Klijent
    {
        public static void Main(string[] args)
        {
            // binding i adresa direktno u konstruktoru — config za klijenta nije potreban
            TacnoVremeClient proxy = new TacnoVremeClient(
                new System.ServiceModel.WSHttpBinding(),
                new System.ServiceModel.EndpointAddress("http://localhost:8000/TacnoVreme"));

            Console.WriteLine("Vreme: " + proxy.TrenutnoVreme());
            Console.WriteLine("Vreme: " + proxy.TrenutnoVreme());
            Console.WriteLine("Vreme: " + proxy.TrenutnoVreme());
            Console.WriteLine("Broj poziva: " + proxy.BrojPoziva());   // 3 — isti proxy = ista sesija
        }
    }
}
