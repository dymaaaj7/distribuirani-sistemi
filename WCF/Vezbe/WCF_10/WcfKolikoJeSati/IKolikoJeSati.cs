using System.Runtime.Serialization;

namespace WcfKolikoJeSati
{
    [ServiceContract(SessionMode = SessionMode.Allowed)]
    public interface IKolikoJeSati
    {
        [OperationContract]
        DateTime VratiVreme();

        [OperationContract]
        PristupLog ZadnjiPristup();

        [OperationContract]
        decimal ObaviOperaciju(decimal op1, decimal op2, string op);
    }

    [DataContract]
    public class PristupLog
    {
        [DataMember]
        public DateTime Timestamp { get; set; }

        [DataMember]
        public string HostName { get; set; }
    }
}