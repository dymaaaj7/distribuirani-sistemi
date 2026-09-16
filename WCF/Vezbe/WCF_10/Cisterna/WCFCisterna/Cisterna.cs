namespace WCFCisterna
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single)]
    public class Cisterna : ICisterna
    {
        private float v = 0;
        private float ro = 0;
        private List<string> promene = new List<string>();


        public void Dodaj(Materijal m)
        {
            float vStaro = v;
            v = vStaro + m.V;
            ro = (ro * vStaro + m.Ro * m.V) / v;
            promene.Add($"Dodato {m.V} m³ materijala {m.Naziv} (gustina {m.Ro})");
        }

        public void Ispusti(float kolicina)
        {
            if (v < kolicina)
                v = 0;
            else
                v -= kolicina;

            promene.Add($"Ispusteno {kolicina} metara kubnih");
        }

        public Materijal Stanje()
        {
            return new Materijal
            {
                Naziv = "mesavina",
                V = v,
                Ro = ro
            };
        }

        public List<string> Promene() { return promene; }
    }
}
