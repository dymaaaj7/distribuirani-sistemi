namespace WCFKalkulator
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession)]
    public class Kalkulator : IKalkulator
    {
        decimal r = 0.0M;
        string izraz = "";
        private IKalkulatorCallback callback;
        public Kalkulator()
        {
            callback = OperationContext.Current.GetCallbackChannel<IKalkulatorCallback>();
        }

        public void Dodaj(decimal a)
        {
            r += a;
            izraz += $"+ {a}";

            callback.Rezultat(new Rezultat()
            {
                Izraz = izraz,
                Vrednost = r
            });
        }
        public void Oduzmi(decimal a)
        {
            r -= a;
            izraz += $"- {a}";

            callback.Rezultat(new Rezultat()
            {
                Izraz = izraz,
                Vrednost = r
            });
        }
        public void Pomnozi(decimal a)
        {
            r *= a;
            izraz += $"* {a}";

            callback.Rezultat(new Rezultat()
            {
                Izraz = izraz,
                Vrednost = r
            });
        }
        public void Podeli(decimal a)
        {
            if (a == 0.0M)
            {
                r = 0.0M;
                izraz = "Deljenje 0";
            }
            else
            {
                r /= a;
                izraz += $"/ {a}";
            }
            callback.Rezultat(new Rezultat()
            {
                Izraz = izraz,
                Vrednost = r
            });
        }
    }
}
