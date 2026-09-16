using System.ServiceModel;
namespace WCFKalkulator
{
    public interface IKalkulatorCallback
    {
        [OperationContract(IsOneWay = true)]
        void Rezultat(Rezultat r);
    }
}
