namespace WCFCisterna
{
    [ServiceContract]
    public interface ICisterna
    {
        [OperationContract]
        void Dodaj(Materijal m);

        [OperationContract]
        void Ispusti(float kolicina);

        [OperationContract]
        Materijal Stanje();

        [OperationContract]
        List<string> Promene();
    }

    [DataContract]
    public class Materijal
    {
        [DataMember]
        public string Naziv { get; set; }

        [DataMember]
        public float V { get; set; }
        [DataMember]
        public float Ro { get; set; }
    }

}
