using System.ServiceModel;
namespace Blanketi.G2026.Jun
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession)]
    public class Kalkulator : IKalkulator
    {
        decimal rezultat = 0.0M;
        string izraz = "";
        private IKalkulatorCallback callback;

        public Kalkulator()
        {
            callback = OperationContext.Current.GetCallbackChannel<IKalkulatorCallback>();
        }

        public decimal Obrisi()
        {
            rezultat = 0.0M;
            izraz = "";
            callback.PokaziIzraz(izraz);
            return rezultat;
        }

        public decimal Dodaj(decimal d)
        {
            rezultat += d;
            izraz += (izraz == "" ? "" : "+") + d;
            callback.PokaziIzraz(izraz);
            return rezultat;
        }

        public decimal Oduzmi(decimal d)
        {
            rezultat -= d;
            izraz += (izraz == "" ? "" : "-") + d;
            callback.PokaziIzraz(izraz);
            return rezultat;
        }

        public decimal Podeli(decimal d)
        {
            rezultat /= d;
            izraz += (izraz == "" ? "" : "/") + d;
            callback.PokaziIzraz(izraz);
            return rezultat;
        }

        public decimal Pomnozi(decimal d)
        {
            rezultat *= d;
            izraz += (izraz == "" ? "" : "*") + d;
            callback.PokaziIzraz(izraz);
            return rezultat;
        }
    }
}
