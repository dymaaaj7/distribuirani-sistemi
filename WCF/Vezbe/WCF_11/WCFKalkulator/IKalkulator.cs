using System.ServiceModel;
namespace WCFKalkulator
{
    [ServiceContract(SessionMode = SessionMode.Required, CallbackContract = typeof(IKalkulatorCallback))]
    public interface IKalkulator
    {
        [OperationContract(IsOneWay = true)]
        void Dodaj(decimal a);

        [OperationContract(IsOneWay = true)]
        void Oduzmi(decimal a);

        [OperationContract(IsOneWay = true)]
        void Pomnozi(decimal a);

        [OperationContract(IsOneWay = true)]
        void Podeli(decimal a);
    }

    [DataContract]
    public class Rezultat
    {
        decimal vrednost = 0.0M;
        string izraz = "";

        [DataMember]
        public decimal Vrednost
        {
            get { return vrednost; }
            set { this.vrednost = value; }
        }

        [DataMember]
        public string Izraz
        {
            get { return izraz; }
            set { this.izraz = value; }
        }
    }
}
