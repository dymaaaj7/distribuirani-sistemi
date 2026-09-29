namespace Septembar
{
    [ServiceBehavior(InstanceContextMode=InstanceContextMode.PerSession)]
    public class TacnoVreme : ITacnoVreme
    {
        int brojac=0;
        public DateTime TrenutnoVreme()
        {
            brojac++;
            return DateTime.Now;
        }

        public int BrojPoziva()
        {
            return brojac;
        }
    }
}
