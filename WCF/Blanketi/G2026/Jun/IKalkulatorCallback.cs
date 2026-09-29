using System.ServiceModel;
namespace Blanketi.G2026.Jun
{
    public interface IKalkulatorCallback
    {
        [OperationContract(IsOneWay=true)]
        void PokaziIzraz(string izraz);
    }
}
